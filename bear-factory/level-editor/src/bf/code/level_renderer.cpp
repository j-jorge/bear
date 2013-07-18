/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level_renderer class.
 * \author Julien Jorge
 */
#include "bf/level_renderer.hpp"

#include "bf/drag_info.hpp"
#include "bf/gui_level.hpp"
#include "bf/item_class.hpp"
#include "bf/item_instance.hpp"
#include "bf/slope.hpp"
#include "bf/sprite_image_cache.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/wx_type_cast.hpp"

/*----------------------------------------------------------------------------*/
const wxCoord bf::level_renderer::s_grip_size = 10;




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The level.
 */
bf::level_renderer::level_renderer( const gui_level& lvl )
  : m_level( lvl ), m_view(0, 0, 1, 1), m_drag_info(NULL),
    m_wireframe_drawing(true), m_graphic_drawing(true), m_display_grid(false),
    m_display_id(true), m_display_relationship(true),
    m_bright_background(false), m_display_continuity_hint(false),
    m_image_cache(new sprite_image_cache),
    m_zoom(100)
{

} // level_renderer::level_renderer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::level_renderer::~level_renderer()
{
  delete m_image_cache;
} // level_renderer::~level_renderer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the size of the handles.
 */
wxCoord bf::level_renderer::get_grip_size() const
{
  return s_grip_size;
} // level_renderer::get_grip_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the view in a given layer.
 * \param index The index of the layer in which the position is computed.
 */
wxPoint bf::level_renderer::compute_local_view_position
( unsigned int index ) const
{ 
  const layer& the_layer( get_level().get_layer( index ) );

  return wxPoint
    ( (double)m_view.x
      * (double)( the_layer.get_width() - get_view_size().x )
      / (double)( get_level().get_width() - get_view_size().x ),
      (double)m_view.y
      * (double)( the_layer.get_height() - get_view_size().y )
      / (double)( get_level().get_height() - get_view_size().y ) );
} // level_renderer::compute_local_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the view in the current layer.
 */
wxPoint bf::level_renderer::compute_local_view_position() const
{
  if ( get_level().empty() )
    return wxPoint(0, 0);
  else
    return compute_local_view_position( get_level().get_active_layer_index() );
} // level_renderer::compute_local_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the view according to a position in the
 * current layer.
 * \param index The index of the layer in which the position is computed.
 * \param view_position The position of a virtual view.
 */
wxPoint bf::level_renderer::compute_global_view_position
( unsigned int index, wxPoint view_position ) const
{ 
  const layer& the_layer( get_level().get_layer( index ) );

  return wxPoint
    ( (double)view_position.x
      / (double)( the_layer.get_width() - get_view_size().x )
      * (double)( get_level().get_width() - get_view_size().x ),
      (double)view_position.y
      / (double)( the_layer.get_height() - get_view_size().y )
      * (double)( get_level().get_height() - get_view_size().y ) );
} // level_renderer::compute_global_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the view according to a position in the
 * current layer.
 * \param view_position The position of a virtual view.
 */
wxPoint bf::level_renderer::compute_global_view_position
( wxPoint view_position ) const
{ 
  if ( get_level().empty() )
    return wxPoint(0, 0);
  else
    return compute_global_view_position
      ( get_level().get_active_layer_index(), view_position );
} // level_renderer::compute_global_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute a position according to the a given layer.
 * \param position The considered position.
 * \param index The layer in which we compute view position.
 * \param height The height of the item.
 */
wxPoint bf::level_renderer::get_position_in_layer
( wxPoint position, unsigned int index, wxCoord height ) const
{
  CLAW_PRECOND( index < get_level().layers_count() );
  
  wxPoint pos_view( compute_local_view_position( index ) );

  return 
    wxPoint( zoom((wxCoord)position.x) - pos_view.x,
             pos_view.y + m_view.height - zoom((wxCoord)position.y) 
             - zoom((wxCoord)height) );
} // level_renderer::get_position_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the visible part of the level.
 */
wxSize bf::level_renderer::get_view_size() const
{
  return zoom( m_view.GetSize() );
} // level_renderer::get_view_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the zoom.
 */
unsigned int bf::level_renderer::get_zoom() const
{
  return m_zoom;
} // level_renderer::get_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the zoom.
 * \param z The new value of the zoom.
 */
void bf::level_renderer::set_zoom(unsigned int z)
{
  m_zoom = z;
} // level_renderer::set_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show/hide the grid.
 * \param v Tell is the grid is visible.
 */
void bf::level_renderer::show_grid( bool v )
{
  m_display_grid = v;
} // level_renderer::show_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the grid is visible or not.
 */
bool bf::level_renderer::get_grid_visibility() const
{
  return m_display_grid;
} // level_renderer::get_grid_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Use a bright bacground.
 * \param v Use it or not.
 */
void bf::level_renderer::set_bright_background( bool v )
{
  m_bright_background = v;
} // level_renderer::set_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we use a bright bacground.
 */
bool bf::level_renderer::get_bright_background() const
{
  return m_bright_background;
} // level_renderer::get_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggles the visibility of the continuity hints.
 */
void bf::level_renderer::toggle_continuity_hints()
{
  m_display_continuity_hint = !m_display_continuity_hint;
} // level_renderer::toggle_continuity_hints()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the continuity hints are displayed..
 */
bool bf::level_renderer::get_continuity_hints() const
{
  return m_display_continuity_hint;
} // level_renderer::get_continuity_hints()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show/hide the id.
 * \param v Tell is the id is visible.
 */
void bf::level_renderer::set_id_visibility( bool v )
{
  m_display_id = v;
} // level_renderer::set_id_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the id is visible or not.
 */
bool bf::level_renderer::get_id_visibility() const
{
  return m_display_id;
} // level_renderer::get_id_visibity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the relationship drawings.
 */
void bf::level_renderer::toggle_relationship_drawing()
{
  m_display_relationship = !m_display_relationship;
} // level_renderer::toggle_relationship_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the drawing of the relationships is enabled.
 */
bool bf::level_renderer::get_relationship_drawing() const
{
  return m_display_relationship;
} // level_renderer::get_relationship_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the graphic drawing.
 */
void bf::level_renderer::toggle_graphic_drawing()
{
  m_graphic_drawing = !m_graphic_drawing;
} // level_renderer::toggle_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the graphic drawing is enabled.
 */
bool bf::level_renderer::get_graphic_drawing() const
{
  return m_graphic_drawing;
} // level_renderer::get_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the wireframe drawing.
 */
void bf::level_renderer::toggle_wireframe_drawing()
{
  m_wireframe_drawing = !m_wireframe_drawing;
} // level_renderer::toggle_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the wireframe drawing is enabled.
 */
bool bf::level_renderer::get_wireframe_drawing() const
{
  return m_wireframe_drawing;
} // level_renderer::get_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the grid.
 */
const bf::grid& bf::level_renderer::get_grid() const
{
  return m_grid;
} // level_renderer::get_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the grid.
 * \param g The new grid.
 */
void bf::level_renderer::set_grid( const grid& g )
{
  m_grid = g;
} // level_renderer::set_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites of the items located in layer having the same size
 *        than the level. Sprites are ordered by increasing z order.
 * \param s (out) The sprites.
 */
void bf::level_renderer::get_structure_sprites
( std::list<sprite_with_position>& s ) const
{
  layer::item_iterator it;

  for (unsigned int i=0; i!=get_level().layers_count(); ++i)
    if ( get_level().layer_is_visible(i) )
      {
        const layer& the_layer = get_level().get_layer(i);

        if ( (the_layer.get_width() == get_level().get_width())
             && (the_layer.get_height() == get_level().get_height()) )
          {
            std::multimap<int, const item_instance*> z_order;

            for (it=the_layer.item_begin(); it!=the_layer.item_end(); ++it)
              if ( it->get_rendering_parameters().has_sprite() )
                z_order.insert
                  ( std::pair<int, const item_instance*>
                    (it->get_rendering_parameters().get_pos_z(), &(*it)) );

            std::multimap<int, const item_instance*>::const_iterator zit;

            for (zit=z_order.begin(); zit!=z_order.end(); ++zit)
              {
                sprite_with_position spr;

                spr.spr = zit->second->get_rendering_parameters().get_sprite();
                if (zit->second->get_rendering_parameters().is_mirrored() )
                  spr.pos.x =
                    (wxCoord)
                    (zit->second->get_rendering_parameters().get_left()
                     - zit->second->get_rendering_parameters().get_gap_x());
                else
                  spr.pos.x =
                    (wxCoord)
                    (zit->second->get_rendering_parameters().get_left()
                     + zit->second->get_rendering_parameters().get_gap_x());

                if ( zit->second->get_rendering_parameters().is_flipped() )
                  spr.pos.y =
                    (wxCoord)
                    ( get_level().get_height()
                      - zit->second->get_rendering_parameters().get_bottom()
                      - zit->second->get_rendering_parameters().get_height()
                      - zit->second->get_rendering_parameters().get_gap_y() );
                else
                  spr.pos.y =
                    (wxCoord)
                    ( get_level().get_height()
                      - zit->second->get_rendering_parameters().get_bottom()
                      - zit->second->get_rendering_parameters().get_height()
                      + zit->second->get_rendering_parameters().get_gap_y() );

                s.push_back( spr );
              }
          }
      }
} // level_renderer::get_structure_sprites()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render all items of the level.
 * \param dc The dc on which we draw.
 * \param part The visible part of the level.
 * \param check_result Tells which items are invalid.
 * \param d The description of the current drag & drop, if any.
 */
void bf::level_renderer::render
( wxGCDC& dc, wxRect part, level_check_result check_result, drag_info const* d )
{
  m_view = part;
  m_check_result = check_result;
  m_drag_info = d;

  wxFont font(dc.GetFont());
  font.SetPointSize(8);
  dc.SetFont(font);
  dc.SetTextForeground(*wxWHITE);
  dc.SetTextBackground(*wxBLACK);
  dc.SetBackgroundMode(wxSOLID);

  if (m_bright_background)
    dc.SetBackground(*wxGREY_BRUSH);
  else
    dc.SetBackground(*wxBLACK_BRUSH);

  dc.Clear();

  render_layers(dc);
  render_grid(dc);
} // level_renderer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a point.
 * \param v The value to transform.
 */
wxPoint bf::level_renderer::zoom( wxPoint v ) const
{
  return wxPoint( zoom(v.x), zoom(v.y) );
} // level_renderer::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a size.
 * \param v The value to transform.
 */
wxSize bf::level_renderer::zoom( wxSize v ) const
{
  return wxSize( zoom(v.x), zoom(v.y) );
} // level_renderer::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed coordinate.
 * \param v The value to transform.
 */
wxCoord bf::level_renderer::unzoom( wxCoord v ) const
{
  return v * 100 / m_zoom;
} // level_renderer::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed point.
 * \param v The value to transform.
 */
wxPoint bf::level_renderer::unzoom( wxPoint v ) const
{
  return wxPoint( unzoom(v.x), unzoom(v.y) );
} // level_renderer::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed size.
 * \param v The value to transform.
 */
wxSize bf::level_renderer::unzoom( wxSize v ) const
{
  return wxSize( unzoom(v.x), unzoom(v.y) );
} // level_renderer::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer on which we are working.
 */
const bf::layer& bf::level_renderer::get_active_layer() const
{
  return get_level().get_active_layer();
} // level_renderer::get_active_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level on which we are working.
 */
const bf::gui_level& bf::level_renderer::get_level() const
{
  return m_level;
} // level_renderer::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render all layers.
 * \param dc The device context for the drawings.
 */
void bf::level_renderer::render_layers( wxGCDC& dc ) const
{
  const wxRect vis_box
    ( unzoom(m_view.GetPosition()), unzoom(m_view.GetSize()) );

  for (unsigned int i=0; i!=get_level().layers_count(); ++i)
    if ( get_level().layer_is_visible(i) )
      {
        const layer& the_layer = get_level().get_layer(i);
        layer::item_iterator it;
        std::multimap<int, item_instance*> z_order;

        for (it=the_layer.item_begin(); it!=the_layer.item_end(); ++it)
          {
            wxRect box = get_presence_box(*it);
            box.SetPosition
              ( get_position_in_layer
                (box.GetPosition(), i,
                 box.GetSize().y) + unzoom(m_view.GetPosition()) );

            if ( box.Intersects(vis_box)  )
              z_order.insert
                ( std::pair<int, item_instance*>
                  (it->get_rendering_parameters().get_pos_z(), &(*it)) );
          }

        render_items( dc, z_order, i );

        if ( i == get_level().get_active_layer_index() )
          render_drag(dc, z_order, i);
      }
} // level_renderer::render_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a set of items.
 * \param dc The device context for the drawings.
 * \param z_order The items.
 * \param i The index of the layer in which the items are.
 */
void bf::level_renderer::render_items
( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order,
  unsigned int i ) const
{
  std::multimap<int, item_instance*>::const_iterator it;
  std::list<const item_instance*> wireframe_forced;

  if ( m_graphic_drawing )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      if ( has_visual(*(it->second), z_order) )
        render_item_sprite(dc, *it->second, z_order,i);
      else
        {
          render_item_filled(dc, *it->second, i);
          wireframe_forced.push_back(it->second);
        }

  if( m_wireframe_drawing || !m_graphic_drawing )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_item_wireframe(dc, *it->second, i );
  else
    for ( ; !wireframe_forced.empty(); wireframe_forced.pop_front() )
      render_item_wireframe(dc, *wireframe_forced.front(), i );

  for (it=z_order.begin(); it!=z_order.end(); ++it)
    if ( m_check_result.contains(&get_level().get_layer(i), it->second) )
      render_non_valid_item(dc, *it->second,i);

  if ( m_display_id )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_item_id(dc, *it->second, i);

  if ( m_display_relationship )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_relationship( dc, *it->second, z_order, i );

  if ( m_display_continuity_hint )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_continuity_hint( dc, *it->second, i );
} // level_renderer::render_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the relationship among items.
 * \param dc The device context for the drawings.
 * \param item The center item, for which we draw the relationship.
 * \param z_order The items.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_relationship
( wxDC& dc, const item_instance& item,
  const std::multimap<int, item_instance*>& z_order,
  unsigned int index ) const
{
  std::list<item_class const*> h;
  item_class::field_iterator itf;

  item.get_class().find_hierarchy(h);

  for ( ; !h.empty(); h.pop_front() )
    for ( itf=h.front()->field_begin(); itf!=h.front()->field_end(); ++itf )
      if ( itf->get_field_type() == type_field::item_reference_field_type )
        if ( item.has_value(*itf) )
          {
            std::list<item_reference_type> ref;

            if ( itf->is_list() )
              item.get_value(*itf, ref);
            else
              {
                item_reference_type r;
                item.get_value(*itf, r);
                ref.push_front(r);
              }

            std::multimap<int, item_instance*>::const_iterator itz;

            for ( ; !ref.empty(); ref.pop_front() )
              for (itz=z_order.begin(); itz!=z_order.end(); ++itz)
                if ( itz->second->get_id() == ref.front().get_value() )
                  render_relationship(dc, item, *itz->second, index);
          }
} // level_renderer::render_relationship()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the relationship among items.
 * \param dc The device context for the drawings.
 * \param ref The center item, for which we draw the relationship.
 * \param rel An item in relation with \a ref.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_relationship
( wxDC& dc, const item_instance& ref, const item_instance& rel, 
  unsigned int index ) const
{
  const wxPoint center_ref =
    get_position_in_layer
    ( wxPoint( ref.get_rendering_parameters().get_left()
               + ref.get_rendering_parameters().get_width() / 2,
               ref.get_rendering_parameters().get_bottom()
               + ref.get_rendering_parameters().get_height() / 2 ),
      index, 0);

  const wxPoint center_rel =
    get_position_in_layer
    ( wxPoint( rel.get_rendering_parameters().get_left()
               + rel.get_rendering_parameters().get_width() / 2,
               rel.get_rendering_parameters().get_bottom()
               + rel.get_rendering_parameters().get_height() / 2 ),
      index, 0);

  const wxCoord r( zoom(4) );
  const wxColour clr( std_to_wx_string(ref.get_class().get_color()) );

  dc.SetPen( get_display_pen(ref, index) );
  dc.SetBrush( wxBrush(clr) );

  dc.DrawCircle(center_ref, std::max(r,1));
  dc.DrawCircle(center_rel, std::max(r/2,1));
  dc.DrawLine(center_ref, center_rel);
} // level_renderer::render_relationship()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 * \param index The index of the layer in which the item is rendered.
 * \param z_order The items.
 */
void bf::level_renderer::render_item
( wxGCDC& dc, const item_instance& item, const wxPoint& pos,
  unsigned int index, const std::multimap<int, item_instance*>& z_order ) const
{
  if ( has_visual(item, z_order) )
    render_item_as_sprite(dc, item, pos, z_order);

  render_item_as_wireframe(dc, item, pos, index);

  if ( m_display_id )
    {
      const wxSize size
        ( (int)item.get_rendering_parameters().get_width(),
          (int)item.get_rendering_parameters().get_height() );
      render_item_id(dc, item, pos+size/2);
    }
} // level_renderer::render_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the sprite of an item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param z_order The items.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_item_sprite
( wxDC& dc, const item_instance& item,
  const std::multimap<int, item_instance*>& z_order, unsigned int index ) const
{
  wxPoint pos = 
    get_position_in_layer
    ( wxPoint( (wxCoord)item.get_rendering_parameters().get_left(),
               (wxCoord)item.get_rendering_parameters().get_bottom()), index,
      (wxCoord)item.get_rendering_parameters().get_height());

  render_item_as_sprite(dc, item, pos, z_order);
} // level_renderer::render_item_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a filled box on an item on the screen.
 * \param gc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_item_filled
( wxGCDC& gc, const item_instance& item, unsigned int index ) const
{
  wxPoint pos = 
    get_position_in_layer
    ( wxPoint( (wxCoord)item.get_rendering_parameters().get_left(),
               (wxCoord)item.get_rendering_parameters().get_bottom()), index,
    (wxCoord)item.get_rendering_parameters().get_height() );

  wxSize size
    ( zoom((wxCoord)item.get_rendering_parameters().get_width()),
      zoom((wxCoord)item.get_rendering_parameters().get_height()) );

  wxPoint p[4];

  p[0] = wxPoint( pos.x,  pos.y );
  p[1] = wxPoint( pos.x + size.x - 1, pos.y );
  p[2] = wxPoint( pos.x + size.x - 1, pos.y + size.y - 1 );
  p[3] = wxPoint( pos.x, pos.y + size.y - 1 );

  gc.SetPen( get_display_pen(item, index) );

  const wxColour base_colour( std_to_wx_string(item.get_class().get_color()) );
  unsigned char alpha( wxALPHA_OPAQUE / 2 );

  if ( index != get_level().get_active_layer_index() )
    alpha =
      alpha / 2
      / std::abs( (int)index - (int)get_level().get_active_layer_index() );

  const wxColour c
    ( base_colour.Red(), base_colour.Green(), base_colour.Blue(), alpha );
  wxBrush brush( c );

  gc.SetBrush(brush);

  gc.DrawLines(4, p);
} // level_renderer::render_item_filled()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the box around an item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_item_wireframe
( wxGCDC& dc, const item_instance& item, unsigned int index ) const
{
  wxPoint pos = 
    get_position_in_layer
    ( wxPoint((wxCoord)item.get_rendering_parameters().get_left(),
              (wxCoord)item.get_rendering_parameters().get_bottom()), index,
    item.get_rendering_parameters().get_height() );

  render_item_as_wireframe(dc, item, pos, index);
} // level_renderer::render_item_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the identifier of an item.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer.
 */
void
bf::level_renderer::render_item_id
( wxDC& dc, const item_instance& item, unsigned int index ) const
{
  const rectangle_type box( get_level().get_visual_box(item) );

  const size_box_type size( zoom(box.width()), zoom(box.height()) );

  const wxPoint pos =
    get_position_in_layer
    ( wxPoint( box.left() + unzoom(size.x / 2), box.top()), index, 0 );

  render_item_id(dc, item, pos);
} // level_renderer::render_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Renders the hints to check the continuity of an item and its
 *        neighbors.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_continuity_hint
( wxDC& dc, const item_instance& item, unsigned int index ) const
{
  const rectangle_type box( get_level().get_visual_box(item) );

  const size_box_type size( zoom(box.width()), zoom(box.height()) );
  const wxCoord box_width(10);

  if ( (size.x <= 2 * box_width) || (size.y <= 2 * box_width) )
    return;

  const wxPoint pos =
    get_position_in_layer( wxPoint( box.left(), box.top()), index, 0 );

  const bool mirror( item.get_rendering_parameters().is_mirrored() );
  const wxColor left_color( mirror ? *wxBLACK : *wxWHITE );
  const wxColor right_color( mirror ? *wxWHITE : *wxBLACK );

  const bool flip( item.get_rendering_parameters().is_flipped());
  const wxColor bottom_color( flip ? *wxBLACK : *wxWHITE );
  const wxColor top_color( flip ? *wxWHITE : *wxBLACK );

  const wxCoord little_margin( box_width / 2 );
  const wxCoord h_margin( size.x / 2 - box_width / 2 );
  
  render_continuity_hint
    ( dc, pos.x + h_margin, pos.y + little_margin, box_width, left_color );
  render_continuity_hint
    ( dc, pos.x + size.x - h_margin, pos.y + little_margin, box_width,
      right_color );
  render_continuity_hint
    ( dc, pos.x + h_margin, pos.y + size.y - little_margin, box_width,
      left_color );
  render_continuity_hint
    ( dc, pos.x + size.x - h_margin, pos.y + size.y - little_margin,
      box_width, right_color );

  const wxCoord v_margin( size.y / 2 - box_width / 2 );

  render_continuity_hint
    ( dc, pos.x + little_margin, pos.y + v_margin, box_width, top_color );
  render_continuity_hint
    ( dc, pos.x + little_margin, pos.y + size.y - v_margin, box_width,
      bottom_color );
  render_continuity_hint
    ( dc, pos.x + size.x - little_margin, pos.y + v_margin, box_width,
      top_color );
  render_continuity_hint
    ( dc, pos.x + size.x - little_margin, pos.y + size.y - v_margin,
      box_width, bottom_color );
} // level_renderer::render_continuity_hint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Renders a continuity hint.
 * \param dc The device context for the drawings.
 * \param x The x-coordinate of the center of the hint.
 * \param y The y-coordinate of the center of the hint.
 * \param s The size of the hint.
 * \param c The color of the hint.
 */
void bf::level_renderer::render_continuity_hint
( wxDC& dc, wxCoord x, wxCoord y, wxCoord s, wxColor c ) const
{
  dc.SetPen( wxPen( c ) );
  dc.SetBrush( wxBrush( c ) );

  wxPoint p[4];

  p[0] = wxPoint( x - s / 2,  y - s / 2 );
  p[1] = wxPoint( x + s / 2, y - s / 2 );
  p[2] = wxPoint( x + s / 2, y + s / 2 );
  p[3] = wxPoint( x - s / 2, y + s / 2 );

  dc.DrawPolygon(4, p);
} // level_renderer::render_continuity_hint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the sprite of an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 * \param z_order The items.
 */
void bf::level_renderer::render_item_as_sprite
( wxDC& dc, const item_instance& item, const wxPoint& pos,
  const std::multimap<int, item_instance*>& z_order ) const
{
  wxPoint p(pos);

  // the y coordinate of the level_renderer and the level are opposed
  if ( item.get_rendering_parameters().is_mirrored() )
    p.x -= zoom( wxCoord(item.get_rendering_parameters().get_gap_x()) );
  else
    p.x += zoom( wxCoord(item.get_rendering_parameters().get_gap_x()) );

  if ( item.get_rendering_parameters().has_sprite() )
    p.y += zoom(item.get_rendering_parameters().get_height())
      - zoom(item.get_rendering_parameters().get_sprite().height());
  else
    {
      const item_class& my_class(item.get_class());
      const std::string default_name("item_with_decoration.item_to_mimic");
      item_reference_type mimic;

      if ( my_class.has_field
           ( default_name, type_field::item_reference_field_type ) )
        if ( item.has_value( my_class.get_field(default_name) ) )
          {
            item.get_value( default_name, mimic );

            std::multimap<int, item_instance*>::const_iterator itz;

            for (itz=z_order.begin(); itz!=z_order.end(); ++itz)
              if ( itz->second->get_id() == mimic.get_value() )
                p.y += zoom(item.get_rendering_parameters().get_height())
                  - zoom(itz->second->get_rendering_parameters()
                         .get_sprite().height());
          }
    }

  if ( item.get_rendering_parameters().is_flipped() )
    p.y += zoom(item.get_rendering_parameters().get_gap_y());
  else
    p.y -= zoom(item.get_rendering_parameters().get_gap_y());

  render_sprite(dc, item, p, z_order);
} // level_renderer::render_item_as_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the box of an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_item_as_wireframe
( wxGCDC& dc, const item_instance& item, const wxPoint& pos,
  unsigned int index ) const
{
  wxSize size
    ( zoom((wxCoord)item.get_rendering_parameters().get_width()),
      zoom((wxCoord)item.get_rendering_parameters().get_height()) );

  if ( (size.x == 0) || (size.y == 0) )
    render_item_as_point( dc, item, pos, index );
  else
    render_item_bounding_box( dc, item, pos, size, index );

  if ( get_level().item_is_main_selection(&item) )
    render_grip(dc, index);

  if ( item.get_class().get_class_name() == "bear::slope" )
    {
      render_slope_curve_grip(dc, item, pos, size, index);
      render_slope_steepness(dc, item, pos, size, index);
    }
} // level_renderer::render_item_as_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 */
void bf::level_renderer::render_item_id
( wxDC& dc, const item_instance& item, const wxPoint& pos ) const
{
  wxString s(std_to_wx_string(item.get_id()));

  dc.SetBrush(*wxBLACK_BRUSH);
  dc.DrawText( s, pos.x - dc.GetTextExtent(s).x / 2, pos.y);
} // level_renderer::render_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an item as a point on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the point on the screen.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_item_as_point
( wxDC& dc, const item_instance& item, const wxPoint& pos,
  unsigned int index ) const
{
  dc.SetPen( get_display_pen(item, index) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  const rectangle_type r( get_level().get_visual_box( item ) );

  dc.DrawCircle(pos.x, pos.y, r.width() / 2);

  wxPoint p[2];

  p[0] = wxPoint( pos.x, pos.y - r.height() );
  p[1] = wxPoint( pos.x, pos.y + r.height() );

  dc.DrawPolygon(2, p);

  p[0] = wxPoint( pos.x - r.width(), pos.y );
  p[1] = wxPoint( pos.x + r.width(), pos.y );

  dc.DrawPolygon(2, p);
} // level_renderer::render_item_as_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param z_order The items.
 * \return true if the sprite was rendered.
 */
bool bf::level_renderer::render_sprite
( wxDC& dc, const item_instance& item, const wxPoint& pos,
  const std::multimap<int, item_instance*>& z_order) const
{
  std::pair<wxBitmap, wxPoint> spr( get_item_or_mimic_visual(item, z_order) );

  if ( spr.first.IsOk() )
    dc.DrawBitmap
      ( spr.first, pos.x + spr.second.x, pos.y + spr.second.y, true );

  return spr.first.IsOk();
} // level_renderer::render_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the bounding box of an item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_item_bounding_box
( wxDC& dc, const item_instance& item, const wxPoint& pos, const wxSize& size,
  unsigned int index ) const
{
  dc.SetPen( get_display_pen(item, index) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  wxPoint p[4];

  p[0] = wxPoint( pos.x,  pos.y );
  p[1] = wxPoint( pos.x + size.x - 1, pos.y );
  p[2] = wxPoint( pos.x + size.x - 1, pos.y + size.y - 1 );
  p[3] = wxPoint( pos.x, pos.y + size.y - 1 );

  dc.DrawPolygon(4, p);
} // level_renderer::render_item_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the grip on curve of the slope.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_slope_curve_grip
( wxDC& dc, const item_instance& item, const wxPoint& pos, const wxSize& size,
  unsigned int index ) const
{
  if ( index != get_level().get_active_layer_index() )
    return;

  if ( !get_level().item_is_main_selection( &item ) )
    return;

  dc.SetPen( get_display_pen(item, index) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  slope s;
  s.read_from( item );
  s.scale( zoom( 1.0 ) );
      
  wxPoint p[2];
  
  if ( s.steepness < 0 )
    {
      p[0] = wxPoint( pos.x,  pos.y );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y - s.steepness );
    }
  else
    {
      p[0] = wxPoint( pos.x,  pos.y + s.steepness);
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y );
    }

  render_grip_box
    ( dc,
      p[0].x - s_grip_size / 2 + s.left_handle.x, 
      p[0].y - s_grip_size / 2 - s.left_handle.y );
  dc.DrawLine
    ( p[0].x + s.left_handle.x, p[0].y - s.left_handle.y, p[0].x, p[0].y );
      
  render_grip_box
    ( dc,
      p[1].x - s_grip_size / 2 + s.right_handle.x,
      p[1].y - s_grip_size / 2 - s.right_handle.y );
  dc.DrawLine
    ( p[1].x + s.right_handle.x, p[1].y - s.right_handle.y, p[1].x, p[1].y );
} // level_renderer::render_slope_curve_grip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the steepness of the slope.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::level_renderer::render_slope_steepness
( wxGCDC& dc, const item_instance& item, const wxPoint& pos, const wxSize& size,
  unsigned int index ) const
{
  wxPen pen( get_display_pen( item, index ) );
  dc.SetPen( pen );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  
  slope s;
  s.read_from( item );
  s.scale( zoom( 1.0 ) );

  wxPoint p[2];
  
  if ( s.steepness < 0 )
    {
      p[0] = wxPoint( pos.x,  pos.y );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y - s.steepness );
    }
  else
    {
      p[0] = wxPoint( pos.x,  pos.y + s.steepness );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y );
    }

  wxGraphicsPath path = dc.GetGraphicsContext()->CreatePath();
  
  path.MoveToPoint(p[0]);
  path.AddCurveToPoint
    ( p[0].x + s.left_handle.x, p[0].y - s.left_handle.y,
      p[1].x + s.right_handle.x,  p[1].y - s.right_handle.y,
      p[1].x, p[1].y );

  dc.GetGraphicsContext()->StrokePath(path);
} // level_renderer::render_slope_steepness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a non valid item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_non_valid_item
( wxDC& dc, const item_instance& item, unsigned int index ) const
{
  wxPen pen( *wxRED, 2 );

  dc.SetPen( pen );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  const wxPoint pos =
    get_position_in_layer
    ( wxPoint((wxCoord)item.get_rendering_parameters().get_left(),
              (wxCoord)item.get_rendering_parameters().get_bottom()), 
      index, (wxCoord)item.get_rendering_parameters().get_height() );

  wxSize size
    ( zoom((wxCoord)item.get_rendering_parameters().get_width()),
      zoom((wxCoord)item.get_rendering_parameters().get_height()) );

  if ( (size.x == 0) || (size.y == 0) )
    render_non_valid_item_as_point( dc, item, pos );
  else
    render_non_valid_item_box( dc, item, pos, size );
} // level_renderer::render_non_valid_item()


/*----------------------------------------------------------------------------*/
/**
 * \brief Render a non valid item as point on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 */
void bf::level_renderer::render_non_valid_item_as_point
( wxDC& dc, const item_instance& item, const wxPoint& pos ) const
{
  wxPoint p[2];

  const rectangle_type r( get_level().get_visual_box( item ) );

  p[0] = wxPoint( pos.x - r.width(), pos.y - r.height() );
  p[1] = wxPoint( pos.x + r.width(), pos.y + r.height() );

  dc.DrawPolygon(2, p);

  p[0] = wxPoint( pos.x - r.width(), pos.y + r.height() );
  p[1] = wxPoint( pos.x + r.width(), pos.y - r.height() );

  dc.DrawPolygon(2, p);
} // level_renderer::render_non_valid_item_as_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a non valid item as point on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 */
void bf::level_renderer::render_non_valid_item_box
( wxDC& dc, const item_instance& item,
  const wxPoint& pos, const wxSize& size ) const
{
  wxPoint p[2];

  p[0] = wxPoint( pos.x,  pos.y );
  p[1] = wxPoint( pos.x + size.x - 1, pos.y + size.y - 1 );

  dc.DrawPolygon(2, p);

  p[0] = wxPoint( pos.x + size.x - 1, pos.y );
  p[1] = wxPoint( pos.x, pos.y + size.y - 1 );

  dc.DrawPolygon(2, p);
} // level_renderer::render_non_valid_item_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the grip of the selected item.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_grip( wxDC& dc, unsigned int index ) const
{
  CLAW_PRECOND( get_level().has_selection( index ) );

  const item_instance* main_selection( get_level().get_main_selection(index) );
  wxRect b_box
    ( rectangle_to_wx( get_level().get_visual_box(*main_selection) ) );

  b_box.SetPosition( get_position_in_layer( b_box.GetBottomLeft(), index, 0 ) );

  wxRect box
    ( b_box.GetLeft(), b_box.GetTop(),
      zoom(b_box.GetWidth()),
      zoom(b_box.GetHeight()) );

  dc.SetPen(*wxRED_PEN);
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  render_grip_box
    ( dc, box.GetLeft() - s_grip_size, box.GetTop() - s_grip_size );
  render_grip_box( dc, box.GetRight(), box.GetTop() - s_grip_size );
  render_grip_box( dc, box.GetRight(), box.GetBottom() );
  render_grip_box( dc, box.GetLeft() - s_grip_size, box.GetBottom() );

  if ( (main_selection->get_rendering_parameters().get_height() != 0)
       && (main_selection->get_rendering_parameters().get_width() != 0) )
    {
      render_grip_box
        ( dc, box.GetLeft() - s_grip_size,
          box.GetTop() + box.GetHeight() / 2 - s_grip_size / 2 );
      render_grip_box
        ( dc, box.GetRight(),
          box.GetTop() + box.GetHeight() / 2 - s_grip_size / 2 );
      render_grip_box
        ( dc, box.GetLeft() + box.GetWidth() / 2 - s_grip_size / 2,
          box.GetTop() - s_grip_size );
      render_grip_box
        ( dc, box.GetLeft() + box.GetWidth() / 2 - s_grip_size / 2,
          box.GetBottom() );
    }
} // level_renderer::render_grip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draws the image of an handle.
 * \param dc The device context for the drawings.
 * \param left The left position of the handle.
 * \param top The top position of the handle.
 */
void bf::level_renderer::render_grip_box
( wxDC& dc, wxCoord left, wxCoord top ) const
{
  dc.SetPen(*wxRED_PEN);
  dc.SetBrush( wxBrush( wxColor( wxT("#800000") ) ) );

  dc.DrawRectangle( left, top, s_grip_size, s_grip_size );
} // level_renderer::render_grip_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag.
 * \param dc The device context for the drawings.
 * \param z_order The items.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_drag
( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order,
  unsigned int index ) const
{
  if ( m_drag_info != NULL )
    switch(m_drag_info->drag_mode)
      {
      case drag_info::drag_mode_selection:
        render_drag_mode_selection(dc,index);
        break;
      case drag_info::drag_mode_move:
        render_drag_mode_move(dc, z_order,index);
        break;
      case drag_info::drag_mode_size:
        render_drag_mode_size(dc,index);
        break;      
      case drag_info::drag_mode_slope:
        render_drag_mode_slope(dc, index);
        break;
      default:
        {
          // nothing to do
        }
      }
} // level_renderer::render_drag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of selection.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_drag_mode_selection
( wxDC& dc, unsigned int index ) const
{
  dc.SetPen( wxPen( *wxRED, 1 ) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  wxPoint p[4];
  wxPoint pos_view = compute_local_view_position();
  p[0].x = zoom(m_drag_info->mouse_origin.x) - pos_view.x;
  p[0].y =  m_view.height - zoom(m_drag_info->mouse_origin.y - 1) + pos_view.y;
  
  p[1] = wxPoint( p[0].x + zoom(m_drag_info->delta().x), p[0].y );
  p[2] = wxPoint( p[1].x, p[0].y - zoom(m_drag_info->delta()).y );
  p[3] = wxPoint( p[0].x, p[2].y );

  dc.DrawPolygon(4, p);
} // level_renderer::render_drag_mode_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of selection.
 * \param dc The device context for the drawings.
 * \param z_order The item.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_drag_mode_move
( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order,
  unsigned int index ) const
{
  item_selection::const_iterator it;
  const item_selection& selection( get_level().get_selection( index ) );
  wxPoint pos_view = compute_local_view_position();

  for (it=selection.begin(); it!=selection.end(); ++it)
    {
      wxPoint pos
        ( zoom( (wxCoord)(*it)->get_rendering_parameters().get_left()
                + m_drag_info->delta().x ) - pos_view.x,
          pos_view.y + m_view.height - zoom(m_drag_info->delta().y)
          - zoom((wxCoord)(*it)->get_rendering_parameters().get_bottom())
          - zoom((wxCoord)(*it)->get_rendering_parameters().get_height()) );

      render_item( dc, **it, pos, index, z_order );
    }
} // level_renderer::render_drag_mode_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of sizing.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_drag_mode_size
( wxDC& dc, unsigned int index ) const
{
  dc.SetPen( wxPen( *wxRED, 1 ) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  wxPoint p[4];
  wxPoint pos_view = compute_local_view_position();

  // m_drag_info->mouse_origin is the immobile corner
  // m_drag_info->mouse_position is the moving corner
  p[0].x = zoom(m_drag_info->mouse_origin.x) - pos_view.x;
  p[0].y = m_view.height - zoom(m_drag_info->mouse_origin.y - 1) + pos_view.y;

  p[2].x = zoom(m_drag_info->mouse_position.x) - pos_view.x;
  p[2].y = m_view.height - zoom(m_drag_info->mouse_position.y - 1) + pos_view.y;

  p[1] = wxPoint( p[2].x, p[0].y );
  p[3] = wxPoint( p[0].x, p[2].y );

  dc.DrawPolygon(4, p);
} // level_renderer::render_drag_mode_size()


/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of changing slope curve.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::level_renderer::render_drag_mode_slope
( wxGCDC& dc, unsigned int index ) const
{
  wxPen pen
    ( wxColour
      ( std_to_wx_string(m_drag_info->picked_item->get_class().get_color()) ), 
      1, wxSOLID );
  dc.SetPen( pen );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  
  wxPoint gap
    ( m_drag_info->mouse_position.x - m_drag_info->mouse_origin.x,
      m_drag_info->mouse_position.y - m_drag_info->mouse_origin.y );
  wxPoint pos_view = compute_local_view_position();
  wxPoint pos
    ( zoom( (wxCoord)m_drag_info->picked_item->
            get_rendering_parameters().get_left() ) - pos_view.x,
      pos_view.y + m_view.height
      - zoom((wxCoord)m_drag_info->picked_item->
             get_rendering_parameters().get_bottom())
      - zoom((wxCoord)m_drag_info->picked_item->
             get_rendering_parameters().get_height()) );
  const wxSize size
    ( (int)m_drag_info->picked_item->get_rendering_parameters().get_width(),
      (int)m_drag_info->picked_item->get_rendering_parameters().get_height() );

  slope s;
  s.read_from( *m_drag_info->picked_item );
  s.scale( zoom( 1.0 ) );
  
  if ( m_drag_info->left_side )
    {
      s.left_handle.x = gap.x;
      s.left_handle.y = gap.y;
    }
  else
    {
      s.right_handle.x = gap.x;
      s.right_handle.y = gap.y;
    }

  wxPoint p[2];

  if ( s.steepness < 0 )
    {
      p[0] = wxPoint( pos.x,  pos.y );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y - s.steepness );
    }
  else
    {
      p[0] = wxPoint( pos.x,  pos.y + s.steepness );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y );
    }

  wxGraphicsPath path = dc.GetGraphicsContext()->CreatePath();
  
  path.MoveToPoint(p[0]);
  path.AddCurveToPoint
    ( p[0].x + s.left_handle.x, p[0].y - s.left_handle.y,
      p[1].x + s.right_handle.x, p[1].y - s.right_handle.y,
      p[1].x, p[1].y );

  dc.GetGraphicsContext()->StrokePath(path);
} // level_renderer::render_drag_mode_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the grid on the screen.
 * \param dc The device context for the drawings.
 */
void bf::level_renderer::render_grid( wxDC& dc ) const
{
  if ( !get_level().empty() && m_display_grid )
    {
      wxColour color(200, 200, 200);
      dc.SetPen( wxPen( color, 1 ) );
      dc.SetBrush(*wxTRANSPARENT_BRUSH);

      render_grid_vertical_lines(dc);
      render_grid_horizontal_lines(dc);
    }
} // render_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the vertical lines of the grid on the screen.
 * \param dc The device context for the drawings.
 */
void bf::level_renderer::render_grid_vertical_lines( wxDC& dc ) const
{
  const int width = get_active_layer().get_width();
  const int height = get_active_layer().get_height();
  const wxCoord offset = m_grid.get_offset().GetX();
  const wxCoord step = m_grid.get_step().GetX();
  const wxCoord bound =
    unzoom( m_view.x + std::min(zoom(width), m_view.width) );

  for ( int column = unzoom(m_view.x) / step;
        (column * step) + offset < bound; ++column )
    if ( (column * step) + offset > unzoom(m_view.x) )
      {
        wxPoint p[2];
        p[0] = wxPoint
          ( zoom((column * step) + offset) - m_view.x, m_view.height );
        p[1] = wxPoint
          ( p[0].x,
            m_view.height - std::min(zoom(height) + m_view.y, m_view.height) );
        dc.DrawPolygon(2, p);
      }
} // render_grid_vertical_lines()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the horizontal lines of the grid on the screen.
 * \param dc The device context for the drawings.
 */
void bf::level_renderer::render_grid_horizontal_lines( wxDC& dc ) const
{
  const int width = get_active_layer().get_width();
  const int height = get_active_layer().get_height();
  const wxCoord offset = m_grid.get_offset().GetY();
  const wxCoord step = m_grid.get_step().GetY();
  const wxCoord bound =
    unzoom( m_view.y + std::min(zoom(height), m_view.height) );

  for ( int line = unzoom(m_view.y) / step;
        (line * step) + offset < bound; ++line )
    if ( (line * step) + offset > unzoom(m_view.y) )
      {
        wxPoint p[2];
        p[0] = wxPoint
          ( 0, m_view.height - zoom( (line * step) + offset ) + m_view.y );
        p[1] = wxPoint
          ( std::min( m_view.width, zoom(width) - m_view.x), p[0].y );
        dc.DrawPolygon(2, p);
      }
} // level_renderer::render_grid_horizontal_lines()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual (sprite and position) of an item.
 * \param item The item to render.
 * \return The sprite and the position where it has to be rendered.
 */
std::pair<wxBitmap, wxPoint> bf::level_renderer::get_item_visual
( const item_instance& item ) const
{
  CLAW_PRECOND( item.get_rendering_parameters().has_sprite() );

  std::pair<wxBitmap, wxPoint> result;
  const wxString name
    ( std_to_wx_string
      (item.get_rendering_parameters().get_sprite().get_image_name()) );

  result = m_image_cache->get_image
    ( item.get_rendering_parameters().get_sprite(),
      zoom(item.get_rendering_parameters().get_sprite().width()),
      zoom(item.get_rendering_parameters().get_sprite().height()) );

  return result;
} // level_renderer::get_item_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual (sprite and position) of an item.
 * \param item The item to render.
 * \param rendering_attributes The rendering attributes to combine with sprite.
 * \return The sprite and the position where it has to be rendered.
 */
std::pair<wxBitmap, wxPoint> bf::level_renderer::get_item_visual
( const item_instance& item, 
  const bitmap_rendering_attributes& rendering_attributes ) const
{
  CLAW_PRECOND( item.get_rendering_parameters().has_sprite() );

  std::pair<wxBitmap, wxPoint> result;
  const wxString name
    ( std_to_wx_string
      (item.get_rendering_parameters().get_sprite().get_image_name()) );

  sprite sp = item.get_rendering_parameters().get_sprite();
  sp.combine(rendering_attributes);

  result = m_image_cache->get_image
    ( sp,
      zoom(item.get_rendering_parameters().get_sprite().width()),
      zoom(item.get_rendering_parameters().get_sprite().height()) );

  return result;
} // level_renderer::get_item_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return if item has a visual.
 * \param item The item to render.
 * \param z_order The items.
 */
bool bf::level_renderer::has_visual
( const item_instance& item,
  const std::multimap<int, item_instance*>& z_order) const
{
  bool result(false);

  if ( item.get_rendering_parameters().has_sprite() )
    result = true;
  else
    {
      const item_class& my_class(item.get_class());
      const std::string default_name("item_with_decoration.item_to_mimic");
      item_reference_type mimic;

      if ( my_class.has_field
           ( default_name, type_field::item_reference_field_type ) )
        if ( item.has_value( my_class.get_field(default_name) ) )
          {
            item.get_value( default_name, mimic );

            std::multimap<int, item_instance*>::const_iterator itz;

            for (itz=z_order.begin(); itz!=z_order.end(); ++itz)
              if ( itz->second->get_id() == mimic.get_value() )
                result =  itz->second->get_rendering_parameters().has_sprite();
          }
    }

  return result;
} // level_renderer::has_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual (sprite and position) of an item.
 * \param item The item to render.
 * \param z_order The items.
 * \return The sprite and the position where it has to be rendered.
 */
std::pair<wxBitmap, wxPoint> bf::level_renderer::get_item_or_mimic_visual
( const item_instance& item,
  const std::multimap<int, item_instance*>& z_order  ) const
{
  CLAW_PRECOND( has_visual(item, z_order) );

  std::pair<wxBitmap, wxPoint> result;

  if ( item.get_rendering_parameters().has_sprite() )
    result = get_item_visual(item);
  else
    {
      const item_class& my_class(item.get_class());
      const std::string default_name("item_with_decoration.item_to_mimic");
      item_reference_type mimic;

      if ( my_class.has_field
           ( default_name, type_field::item_reference_field_type ) )
        if ( item.has_value( my_class.get_field(default_name) ) )
          {
            item.get_value( default_name, mimic );
            std::multimap<int, item_instance*>::const_iterator itz;

            for (itz=z_order.begin(); itz!=z_order.end(); ++itz)
              if ( itz->second->get_id() == mimic.get_value() )
                result = 
                  get_item_visual
                  ( *itz->second,
                    item.get_rendering_parameters().get_sprite() );
          }
    }

  return result;
} // level_renderer::get_item_or_mimic_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pen used for displaying an item.
 * \param item The item to display.
 * \param index The index of the layer in which the item is rendered.
 */
wxPen bf::level_renderer::get_display_pen
( const item_instance& item, unsigned int index ) const
{
  wxPen result;

  if ( index == get_level().get_active_layer_index() )
    result.SetWidth( 2 );
  else
    result.SetWidth( 1 );

  if ( get_level().item_is_selected( &item ) )
    {
      if ( index == get_level().get_active_layer_index() )
        result.SetColour(*wxRED);
      else
        result.SetColour( wxT("#800000") );
    }
  else
    {
      const std::string active_tag( get_level().get_active_layer().get_tag() );
      const std::string tag( get_level().get_layer( index ).get_tag() );

      if ( tag == active_tag )
        result.SetColour
          ( wxColour( std_to_wx_string(item.get_class().get_color()) ) );
      else
        result.SetColour( wxT("#3b3b3b") );
    }

  return result;
} // level_renderer::get_display_pen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the box bounding box of an item and its sprite, if any.
 * \param item The item to bound.
 */
wxRect bf::level_renderer::get_presence_box( const item_instance& item ) const
{
  wxRect result( rectangle_to_wx( get_level().get_visual_box(item) ) );

  wxPoint pos_gap( result.GetPosition() );

  if ( item.get_rendering_parameters().is_mirrored() )
    pos_gap.x -= item.get_rendering_parameters().get_gap_x();
  else
    pos_gap.x += item.get_rendering_parameters().get_gap_x();

  if ( item.get_rendering_parameters().is_flipped() )
    pos_gap.y -= item.get_rendering_parameters().get_gap_y();
  else
    pos_gap.y += item.get_rendering_parameters().get_gap_y();

  wxPoint pos_min( std::min(result.GetPosition().x, pos_gap.x),
                   std::min(result.GetPosition().y, pos_gap.y) );

  wxPoint pos_max( std::max(result.GetPosition().x, pos_gap.x),
                   std::max(result.GetPosition().y, pos_gap.y) );

  result.SetPosition( pos_min );
  result.width += pos_max.x - pos_min.x;
  result.height += pos_max.y - pos_min.y;

  return result;
} // level_renderer::get_presence_box()
