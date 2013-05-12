/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::ingame_view class.
 * \author Julien Jorge
 */
#include "bf/ingame_view.hpp"

#include "bf/compilation_context.hpp"
#include "bf/gui_level.hpp"
#include "bf/image_pool.hpp"
#include "bf/ingame_view_frame.hpp"
#include "bf/item_class_pool.hpp"
#include "bf/item_choice_frame.hpp"
#include "bf/properties_frame.hpp"
#include "bf/layer.hpp"
#include "bf/layer_list_frame.hpp"
#include "bf/level_file_xml_writer.hpp"
#include "bf/main_frame.hpp"
#include "bf/windows_layout.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/history/action_add_item.hpp"
#include "bf/history/action_copy_selection.hpp"
#include "bf/history/action_delete_selection.hpp"
#include "bf/history/action_group.hpp"
#include "bf/history/action_move_selection.hpp"
#include "bf/history/action_paste_items.hpp"
#include "bf/history/action_set_item_position_and_size.hpp"
#include "bf/history/action_set_slope_curve.hpp"
#include "bf/history/action_move_down.hpp"
#include "bf/history/action_move_up.hpp"
#include "bf/history/action_rotate_selection.hpp"

#include <claw/assert.hpp>
#include <wx/dcbuffer.h>
#include <wx/clipbrd.h>

#include <limits>

/*----------------------------------------------------------------------------*/
const wxCoord bf::ingame_view::s_point_size = 10;
const wxCoord bf::ingame_view::s_grip_size = 10;
bf::level_clipboard bf::ingame_view::s_clipboard;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::ingame_view::drag_info::drag_info()
  : drag_mode(drag_mode_none), picked_item(NULL), x_active(true), y_active(true)
{

} // ingame_view::drag_info::drag_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the difference between the initial mouse position and the current
 *        mouse position.
 */
wxSize bf::ingame_view::drag_info::delta() const
{
  return wxSize
    ( mouse_position.x - mouse_origin.x, mouse_position.y - mouse_origin.y );
} // ingame_view::drag_info::delta()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param view The view associated with this target.
 */
bf::ingame_view::item_drop_target::item_drop_target( ingame_view& view )
  : m_view(view)
{

} // ingame_view::item_drop_target::item_drop_target()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the level. The class name of the item is dropped on
 *        the view.
 * \param x The x coordinate of the mouse.
 * \param y The y coordinate of the mouse.
 * \param data The name of the class to instanciate.
 */
bool bf::ingame_view::item_drop_target::OnDropText
( wxCoord x, wxCoord y, const wxString& data )
{
  m_view.m_layout.get_main_frame().use_class( wx_to_std_string(data) );

  const wxPoint point
    ( m_view.unzoom(x),  m_view.unzoom(m_view.GetSize().y - y));

  return m_view.add_item( wx_to_std_string(data), point.x, point.y );
} // ingame_view::item_drop_target::OnDropText()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param lvl The level.
 * \param layout The window layout of the program.
 *
 * The level will be deleted in the destructor.
 */
bf::ingame_view::ingame_view
( ingame_view_frame& parent, gui_level* lvl, windows_layout& layout )
  : super( &parent, wxID_ANY ), m_parent(parent), m_layout(layout),
    m_history(lvl), m_view(0, 0), m_drag_info(NULL), m_wireframe_drawing(true),
    m_graphic_drawing(true), m_display_grid(false), m_display_id(true),
    m_display_relationship(true), m_bright_background(false),
    m_image_cache(new sprite_image_cache), m_zoom(100)
{
  CLAW_PRECOND(lvl != NULL);
  SetDropTarget( new item_drop_target(*this) );
} // ingame_view::ingame_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::ingame_view::~ingame_view()
{
  delete m_image_cache;
} // ingame_view::~ingame_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level contains a layer.
 */
bool bf::ingame_view::empty() const
{
  return m_history.get_level().empty();
} // ingame_view::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the center of the view in the level.
 */
wxPoint bf::ingame_view::get_center_in_level() const
{
  if ( empty() )
    return wxPoint(0, 0);
  else
    {
      wxPoint pos_view = compute_local_view_position();

       return wxPoint
         ( unzoom(pos_view.x + GetSize().x / 2),
           unzoom(pos_view.y + GetSize().y / 2) );
    }
} // ingame_view::get_center_in_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the view in the level.
 */
const wxPoint& bf::ingame_view::get_view_position() const
{
  return m_view;
} // ingame_view::get_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the visible part of the layer.
 */
wxSize bf::ingame_view::get_view_size() const
{
  return zoom( GetSize() );
} // ingame_view::get_view_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the layer with the current zoom.
 */
wxSize bf::ingame_view::get_layer_view_size() const
{
  CLAW_PRECOND( !empty() );
  return wxSize
    ( zoom( get_active_layer().get_width() ),
      zoom( get_active_layer().get_height() ) );
} // ingame_view::get_layer_view_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute a position according to the a given layer.
 * \param position The considered position.
 * \param index The layer in which we compute view position.
 * \param height The height of the item.
 */
wxPoint bf::ingame_view::get_position_in_layer
( wxPoint position, unsigned int index, wxCoord height) const
{
  CLAW_PRECOND( index < m_history.get_level().layers_count() );
  
  wxPoint pos_view
    ( (double)m_view.x * 
      (double)( get_level().get_layer(index).get_width() - get_view_size().x ) /
      (double)( get_level().get_width() - get_view_size().x ),
      (double)m_view.y * 
      (double)( get_level().get_layer(index).get_height() -get_view_size().y ) /
      (double)( get_level().get_height() - get_view_size().y ) );

  return 
    wxPoint( zoom((wxCoord)position.x) - pos_view.x,
             pos_view.y + GetSize().y - zoom((wxCoord)position.y) 
             - zoom((wxCoord)height) );
} // ingame_view::get_position_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the view in the level.
 * \param x X-position of the view.
 * \param y Y-position of the view.
 */
void bf::ingame_view::set_view_position( wxCoord x, wxCoord y )
{
  if ( empty() )
    m_view.x = m_view.y = 0;
  else
    {
      m_view.x = x;
      m_view.y = y;
#if 0
      if ( (unsigned int)unzoom(m_view.x + GetSize().x)
           > get_active_layer().get_width() )
        m_view.x = zoom(get_active_layer().get_width()) - GetSize().x;

      if (m_view.x < 0)
        m_view.x = 0;

      if ((unsigned int)unzoom(m_view.y + GetSize().y)
          > get_active_layer().get_height())
        m_view.y = zoom(get_active_layer().get_height()) - GetSize().y;

      if (m_view.y < 0)
        m_view.y = 0;
#else
      if ( (unsigned int)unzoom(m_view.x + GetSize().x)
           > get_level().get_width() )
        m_view.x = zoom(get_level().get_width()) - GetSize().x;

      if (m_view.x < 0)
        m_view.x = 0;

      if ((unsigned int)unzoom(m_view.y + GetSize().y)
          > get_level().get_height())
        m_view.y = zoom(get_level().get_height()) - GetSize().y;

      if (m_view.y < 0)
        m_view.y = 0;
#endif
    }

  m_parent.adjust_scrollbars();
} // ingame_view::set_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the zoom.
 */
unsigned int bf::ingame_view::get_zoom() const
{
  return m_zoom;
} // ingame_view::get_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the zoom.
 * \param z The new value of the zoom.
 */
void bf::ingame_view::set_zoom(unsigned int z)
{
  wxPoint c = get_center_in_level();
  m_zoom = z;

  wxPoint view_position = compute_global_view_position
    ( wxPoint( zoom(c.x) - GetSize().x/2, zoom(c.y) - GetSize().y/2 ) );
  set_view_position( view_position.x, view_position.y );
} // ingame_view::set_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the zoom and center on the mouse.
 * \param z The new value of the zoom.
 * \param mouse_position The position of the mouse.
 */
void bf::ingame_view::set_zoom(unsigned int z, wxPoint mouse_position)
{
  // Go in the center of mouse position and current position
  wxPoint c = get_center_in_level();
  c.x = ( c.x + mouse_position.x ) / 2;
  c.y = ( c.y + mouse_position.y ) / 2;

  m_zoom = z;
  wxPoint view_position = compute_global_view_position
    ( wxPoint( zoom(c.x) - GetSize().x/2, zoom(c.y) - GetSize().y/2 ) );
  set_view_position( view_position.x, view_position.y );
} // ingame_view::set_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index of the active layer.
 */
unsigned int bf::ingame_view::get_active_index() const
{
  return m_history.get_level().get_active_layer_index();
} // ingame_view::get_active_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the active layer.
 */
void bf::ingame_view::set_active_index( unsigned int index )
{
  CLAW_PRECOND( index < m_history.get_level().layers_count() );

  m_history.get_level().set_active_layer(index);

  m_parent.adjust_scrollbars();
  m_parent.set_layer_info();
  m_parent.update_toolbar();
  update_layout();
} // ingame_view::set_active_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer on which we are working.
 */
bf::layer& bf::ingame_view::get_active_layer() const
{
  return m_history.get_level().get_active_layer();
} // ingame_view::get_active_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level on which we are working.
 */
const bf::gui_level& bf::ingame_view::get_level() const
{
  return m_history.get_level();
} // ingame_view::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level on which we are working.
 */
bf::gui_level& bf::ingame_view::get_level()
{
  return m_history.get_level();
} // ingame_view::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show/hide the grid.
 * \param v Tell is the grid is visible.
 */
void bf::ingame_view::show_grid( bool v )
{
  m_display_grid = v;
} // ingame_view::show_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the grid is visible or not.
 */
bool bf::ingame_view::get_grid_visibility() const
{
  return m_display_grid;
} // ingame_view::get_grid_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Use a bright bacground.
 * \param v Use it or not.
 */
void bf::ingame_view::set_bright_background( bool v )
{
  m_bright_background = v;
} // ingame_view::set_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we use a bright bacground.
 */
bool bf::ingame_view::get_bright_background() const
{
  return m_bright_background;
} // ingame_view::get_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show/hide the id.
 * \param v Tell is the id is visible.
 */
void bf::ingame_view::set_id_visibility( bool v )
{
  m_display_id = v;
} // ingame_view::set_id_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the id is visible or not.
 */
bool bf::ingame_view::get_id_visibility() const
{
  return m_display_id;
} // ingame_view::get_id_visibity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the relationship drawings.
 */
void bf::ingame_view::toggle_relationship_drawing()
{
  m_display_relationship = !m_display_relationship;
  Refresh();
} // ingame_view::toggle_relationship_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the drawing of the relationships is enabled.
 */
bool bf::ingame_view::get_relationship_drawing() const
{
  return m_display_relationship;
} // ingame_view::get_relationship_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the graphic drawing.
 */
void bf::ingame_view::toggle_graphic_drawing()
{
  m_graphic_drawing = !m_graphic_drawing;
  Refresh();
} // ingame_view::toggle_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the graphic drawing is enabled.
 */
bool bf::ingame_view::get_graphic_drawing() const
{
  return m_graphic_drawing;
} // ingame_view::get_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the wireframe drawing.
 */
void bf::ingame_view::toggle_wireframe_drawing()
{
  m_wireframe_drawing = !m_wireframe_drawing;
  Refresh();
} // ingame_view::toggle_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the wireframe drawing is enabled.
 */
bool bf::ingame_view::get_wireframe_drawing() const
{
  return m_wireframe_drawing;
} // ingame_view::get_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the grid.
 */
const bf::grid& bf::ingame_view::get_grid() const
{
  return m_grid;
} // ingame_view::get_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the grid.
 */
bf::grid& bf::ingame_view::get_grid()
{
  return m_grid;
} // ingame_view::get_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the grid.
 * \param g The new grid.
 */
void bf::ingame_view::set_grid( const grid& g )
{
  m_grid = g;
} // ingame_view::set_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the grid according to the current selection.
 */
void bf::ingame_view::set_grid_on_selection()
{
  if ( has_selection() )
    {
      const item_instance* selection( get_level().get_main_selection() );

      wxSize offset = m_grid.get_offset();
      wxSize step = m_grid.get_step();

      if ( selection->get_rendering_parameters().get_width() != 0 )
        step.x = (int)selection->get_rendering_parameters().get_width();
      if ( selection->get_rendering_parameters().get_height() != 0 )
        step.y = (int)selection->get_rendering_parameters().get_height();

      m_grid.set_step(step);

      offset.x =
        (unsigned int)selection->get_rendering_parameters().get_left() % step.x;
      offset.y =
        (unsigned int)
        selection->get_rendering_parameters().get_bottom() % step.y;

      m_grid.set_offset(offset);

      render();
    }
} // ingame_view::set_grid_on_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an action.
 * \param action The action to do.
 */
void bf::ingame_view::do_action( level_action* action )
{
  CLAW_PRECOND( action != NULL );
  apply_action(action);

  if ( !empty() && !get_active_layer().check_item_position() )
    show_item_position_error(this);
} // ingame_view::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the action of moving the current selection.
 * \param action The action to do.
 */
void bf::ingame_view::do_action( action_move_selection* action )
{
  CLAW_PRECOND( action != NULL );
  apply_action(action);

  if ( ! get_active_layer().check_item_position() )
    show_item_position_error(this);
} // ingame_view::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the action of rotating the current selection.
 * \param action The action to do.
 */
void bf::ingame_view::do_action( action_rotate_selection* action )
{
  CLAW_PRECOND( action != NULL );
  apply_action(action);

  if ( ! get_active_layer().check_item_position() )
    show_item_position_error(this);
} // ingame_view::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item to the current layer.
 * \param class_name The name of the class to instanciate.
 * \param x The x coordinate of the item in the view.
 * \param y The y coordinate of the item in the view.
 * \return true if the item was added.
 */
bool bf::ingame_view::add_item
( const std::string& class_name, wxCoord x, wxCoord y )
{
  bool result = false;

  if ( !empty() )
    if ( m_layout.get_item_class_pool().has_item_class(class_name) )
      {
        item_instance* item =
          new item_instance
          ( m_layout.get_item_class_pool().get_item_class_ptr(class_name) );
         
       wxPoint pos_view = compute_local_view_position();
       item->get_rendering_parameters().set_position
         (x + unzoom(pos_view.x), y + unzoom(pos_view.y));

        do_action( new action_add_item( item, get_active_index() ) );
        set_selection( item );

        result = true;
      }

  return result;
} // ingame_view::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item to the current layer.
 * \param class_name The name of the class to instanciate.
 * \return true if the item was added.
 */
bool bf::ingame_view::add_item( const std::string& class_name )
{
  return
    add_item( class_name, unzoom(GetSize().x / 2), unzoom(GetSize().y / 2) );
} // ingame_view::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the validity of level.
 * \return true if the level is ok.
 */
bool bf::ingame_view::check_level()
{
  m_check_result.clear();
  get_level().check( m_check_result );
  Refresh();
  return m_check_result.is_ok();
} // ingame_view::check_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the errors reported during the last check.
 */
const bf::level_check_result& bf::ingame_view::get_check_result() const
{
  return m_check_result;
} // ingame_view::get_check_result()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the current level.
 * \param os The stream to save the level in.
 */
void bf::ingame_view::save( std::ostream& os )
{
  level_file_xml_writer lvl;
  lvl.save( os, get_level() );
  m_history.set_saved();
} // ingame_view::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the current level.
 * \param f The file in which we compile.
 * \param o The level of optimization during the compilation.
 */
void bf::ingame_view::compile(compiled_file& f, unsigned int o)
{
  compilation_context context(o);
  get_level().compile(f, context);
  m_history.set_compiled();
} // ingame_view::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Undo the last change.
 */
void bf::ingame_view::undo()
{
  if ( m_history.can_undo() )
    {
      m_history.undo();
      m_parent.set_changed( !m_history.level_is_saved() );
      m_parent.set_compile_changed( !m_history.level_is_compiled() );
      update_layout();
    }
} // ingame_view::undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cancel an undo.
 */
void bf::ingame_view::redo()
{
  if ( m_history.can_redo() )
    {
      m_history.redo();
      m_parent.set_changed( !m_history.level_is_saved() );
      m_parent.set_compile_changed( !m_history.level_is_compiled() );
      update_layout();
    }
} // ingame_view::redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the history of the level.
 */
const bf::level_history& bf::ingame_view::get_history() const
{
  return m_history;
} // ingame_view::get_history()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of the windows of the program.
 */
void bf::ingame_view::update_layout()
{
  m_layout.get_layer_list_frame().set_level_view(this);
  m_layout.get_properties_frame().clear();

  if ( has_selection() )
    m_layout.get_properties_frame().add_items
      ( std::list<item_instance*>
        ( get_level().get_selection().begin(),
          get_level().get_selection().end() ) );

  Refresh();
} // ingame_view::update_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an item selected.
 */
bool bf::ingame_view::has_selection() const
{
  return get_level().has_selection();
} // ingame_view::has_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selection to nothing.
 */
void bf::ingame_view::clear_selection()
{
  get_level().clear_selection();
  m_layout.get_properties_frame().clear();
  Refresh();
} // ingame_view::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select all items.
 */
void bf::ingame_view::select_all()
{
  std::list<item_instance*> item;
  pick_item( item,
             wxRect(0, 0, get_level().get_width(), get_level().get_height()) );
  set_selection( item );
} // ingame_view::select_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select an item, its layer and center view on the item.
 * \param item The item to select.
 */
void bf::ingame_view::select_item_and_layer( item_instance* item )
{
  const std::size_t index = get_level().get_layer_by_item(*item);

  if ( index == get_level().layers_count() )
    return;

  // select the layer
  set_active_index( index );
  get_level().set_layer_visibility(index, true);

  // select the item
  set_selection(item);

  // center the view on the item
  wxCoord x = zoom((wxCoord)item->get_rendering_parameters().get_left() +
                   (wxCoord)item->get_rendering_parameters().get_width()/2)
    - GetSize().x/2;

  wxCoord y = zoom((wxCoord)item->get_rendering_parameters().get_bottom()+
                   (wxCoord)item->get_rendering_parameters().get_height()/2)
    - GetSize().y/2;

  set_view_position(x, y);
} // ingame_view::set_item_and_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the clipboard.
 */
bf::level_clipboard& bf::ingame_view::get_clipboard() const
{
  return s_clipboard;
} // ingame_view::get_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the current selection in the clipboard.
 */
void bf::ingame_view::copy_to_clipboard() const
{
  if ( has_selection() )
    {
      const item_instance& main_selection( *get_level().get_main_selection() );

      item_copy copy;
      copy.x = main_selection.get_rendering_parameters().get_left();
      copy.y = main_selection.get_rendering_parameters().get_bottom();

      item_selection::const_iterator it;
      const item_selection& selection( get_level().get_selection() );

      for (it=selection.begin(); it!=selection.end(); ++it)
        copy.items.push_front(**it);

      s_clipboard.copy_items( copy );
    }
} // ingame_view::copy_to_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the current selection in the clipboard then remove the selected
 *        items.
 */
void bf::ingame_view::cut_to_clipboard()
{
  copy_to_clipboard();
  do_action( new action_delete_selection(get_level()) );
} // ingame_view::cut_to_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Paste the content of the clipboard.
 */
void bf::ingame_view::paste_from_clipboard()
{
  if ( s_clipboard.has_items() )
    do_action
      ( new action_paste_items
        ( get_level(), s_clipboard.get_items(),
          unzoom(m_view.x + GetSize().x / 2),
          unzoom(m_view.y + GetSize().y / 2) ) );
} // ingame_view::paste_from_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites of the items located in layer having the same size
 *        than the level. Sprites are ordered by increasing z order.
 * \param s (out) The sprites.
 */
void bf::ingame_view::get_structure_sprites
( std::list<sprite_with_position>& s ) const
{
  layer::item_iterator it;

  for (unsigned int i=0; i!=get_level().layers_count(); ++i)
    if ( get_level().layer_is_visible(i) )
      {
        const layer& the_layer = get_level().get_layer(i);

        if ( (the_layer.get_width() == m_history.get_level().get_width())
             && (the_layer.get_height() == m_history.get_level().get_height()) )
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
                    ( m_history.get_level().get_height()
                      - zit->second->get_rendering_parameters().get_bottom()
                      - zit->second->get_rendering_parameters().get_height()
                      - zit->second->get_rendering_parameters().get_gap_y() );
                else
                  spr.pos.y =
                    (wxCoord)
                    ( m_history.get_level().get_height()
                      - zit->second->get_rendering_parameters().get_bottom()
                      - zit->second->get_rendering_parameters().get_height()
                      + zit->second->get_rendering_parameters().get_gap_y() );

                s.push_back( spr );
              }
          }
      }
} // ingame_view::get_structure_sprites()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render all items of the level.
 */
void bf::ingame_view::render()
{
  wxBufferedPaintDC dc( this );
  // Create graphics context from it
  wxGraphicsContext* gc = wxGraphicsContext::Create( dc );

  if( IsShown() )
    {
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

      render_layers(dc,*gc);
      render_grid(dc);
    }

  if ( gc )
    delete gc;
} // ingame_view::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if items are in the level.
 * \param The windows that checks positions.
 */
void bf::ingame_view::show_item_position_error(wxWindow* win)
{
  wxMessageDialog dlg
    ( win,
      _("Some items are outside the level. The operation will be canceled."),
      _("Items are outside."), wxOK );
  
  dlg.ShowModal();

  m_history.revert();
  m_parent.set_changed( !m_history.level_is_saved() );
  m_parent.set_compile_changed( !m_history.level_is_compiled() );
  update_layout();
} // ingame_view::show_item_position_error()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the mouse.
 * \param point The point where the mouse event has occured .
 */
wxPoint bf::ingame_view::compute_mouse_position(const wxPoint& point) const
{ 
  if ( empty() )
    return wxPoint(0, 0);
  else
    {
      wxPoint pos_view = compute_local_view_position();

      return wxPoint
        ( unzoom(pos_view.x + point.x),
          unzoom(pos_view.y + GetSize().y - point.y) );
    }
} // ingame_view::compute_mouse_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick the first item found near a given point.
 * \param pos The position of the point.
 */
bf::item_instance* bf::ingame_view::pick_first_item( const wxPoint& pos )
{
  item_instance* result=NULL;
  std::list<item_instance*> instances;
  layer::item_iterator it;
  wxCoord min_max_dist = std::numeric_limits<wxCoord>::max();

  if ( !empty() )
    {
      layer& the_layer( m_history.get_level().get_active_layer() );

      for (it=the_layer.item_begin();
           it!=the_layer.item_end(); ++it)
        {
          wxRect box = get_bounding_box( *it );

          if ( box.Contains(pos) )
            {
              const wxCoord dist =
                std::max
                ( std::max( pos.x - box.GetLeft(), box.GetRight() - pos.x),
                  std::max( pos.y - box.GetTop(), box.GetBottom() - pos.y) );

              if ( dist < min_max_dist )
                {
                  result = &(*it);
                  min_max_dist = dist;
                }
            }
        }
    }

  return result;
} // ingame_view::pick_first_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render all layers.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 */
void bf::ingame_view::render_layers( wxDC& dc, wxGraphicsContext& gc ) const
{
  for (unsigned int i=0; i!=m_history.get_level().layers_count(); ++i)
    if ( get_level().layer_is_visible(i) )
      {
        const layer& the_layer = m_history.get_level().get_layer(i);
        layer::item_iterator it;
        std::multimap<int, item_instance*> z_order;

        const wxRect vis_box( unzoom(m_view), unzoom(GetSize()) );

        for (it=the_layer.item_begin(); it!=the_layer.item_end(); ++it)
          {
            wxRect box = get_presence_box(*it);
            box.SetPosition
              ( get_position_in_layer
                (box.GetPosition(), i, box.GetSize().y) + unzoom(m_view) );

            if ( box.Intersects(vis_box)  )
              z_order.insert
                ( std::pair<int, item_instance*>
                  (it->get_rendering_parameters().get_pos_z(), &(*it)) );
          }

        render_items( dc, gc, z_order, i );

        if ( i == get_active_index() )
          render_drag(dc, gc, z_order, i);
      }
} // ingame_view::render_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a set of items.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param z_order The items.
 * \param i The index of the layer in which the items are.
 */
void bf::ingame_view::render_items
( wxDC& dc, wxGraphicsContext& gc, 
  const std::multimap<int, item_instance*>& z_order,
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
          render_item_filled(gc, *it->second, i);
          wireframe_forced.push_back(it->second);
        }

  if( m_wireframe_drawing || !m_graphic_drawing )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_item_wireframe(dc, gc, *it->second, i );
  else
    for ( ; !wireframe_forced.empty(); wireframe_forced.pop_front() )
      render_item_wireframe(dc, gc, *wireframe_forced.front(), i );

  for (it=z_order.begin(); it!=z_order.end(); ++it)
    if ( m_check_result.contains(&get_level().get_layer(i), it->second) )
      render_non_valid_item(dc, *it->second,i);

  if ( m_display_id )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_item_id(dc, *it->second, i);

  if ( m_display_relationship )
    for (it=z_order.begin(); it!=z_order.end(); ++it)
      render_relationship(dc, *it->second, z_order, i );
} // ingame_view::render_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the relationship among items.
 * \param dc The device context for the drawings.
 * \param item The center item, for which we draw the relationship.
 * \param z_order The items.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_relationship
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
} // ingame_view::render_relationship()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the relationship among items.
 * \param dc The device context for the drawings.
 * \param ref The center item, for which we draw the relationship.
 * \param rel An item in relation with \a ref.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_relationship
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

  dc.SetPen( wxPen(clr, 1, wxDOT_DASH) );
  dc.SetBrush( wxBrush(clr) );

  dc.DrawCircle(center_ref, r);
  dc.DrawCircle(center_rel, r/2);
  dc.DrawLine(center_ref, center_rel);
} // ingame_view::render_relationship()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 * \param index The index of the layer in which the item is rendered.
 * \param z_order The items.
 */
void bf::ingame_view::render_item
( wxDC& dc, wxGraphicsContext& gc, 
  const item_instance& item, const wxPoint& pos,
  unsigned int index, const std::multimap<int, item_instance*>& z_order ) const
{
  if ( has_visual(item, z_order) )
    render_item_as_sprite(dc, item, pos, z_order);

  render_item_as_wireframe(dc, gc, item, pos, index);

  if ( m_display_id )
    {
      const wxSize size
        ( (int)item.get_rendering_parameters().get_width(),
          (int)item.get_rendering_parameters().get_height() );
      render_item_id(dc, item, pos+size/2);
    }
} // ingame_view::render_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the sprite of an item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param z_order The items.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_item_sprite
( wxDC& dc, const item_instance& item,
  const std::multimap<int, item_instance*>& z_order, unsigned int index ) const
{
  wxPoint pos = 
    get_position_in_layer
    ( wxPoint( (wxCoord)item.get_rendering_parameters().get_left(),
               (wxCoord)item.get_rendering_parameters().get_bottom()), index,
      (wxCoord)item.get_rendering_parameters().get_height());

  render_item_as_sprite(dc, item, pos, z_order);
} // ingame_view::render_item_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a filled box on an item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_item_filled
( wxGraphicsContext& gc, const item_instance& item, unsigned int index ) const
{
  wxPoint pos = 
    get_position_in_layer
    ( wxPoint( (wxCoord)item.get_rendering_parameters().get_left(),
               (wxCoord)item.get_rendering_parameters().get_bottom()), index,
    (wxCoord)item.get_rendering_parameters().get_height() );

  wxSize size
    ( zoom((wxCoord)item.get_rendering_parameters().get_width()),
      zoom((wxCoord)item.get_rendering_parameters().get_height()) );

  wxPoint2DDouble p[4];

  p[0] = wxPoint( pos.x,  pos.y );
  p[1] = wxPoint( pos.x + size.x - 1, pos.y );
  p[2] = wxPoint( pos.x + size.x - 1, pos.y + size.y - 1 );
  p[3] = wxPoint( pos.x, pos.y + size.y - 1 );

  gc.SetPen( get_display_pen(item, index) );

  const wxColour base_colour( std_to_wx_string(item.get_class().get_color()) );
  unsigned char alpha( wxALPHA_OPAQUE / 2 );

  if ( index != get_active_index() )
    alpha = alpha / 2 / std::abs( (int)index - (int)get_active_index() );

  const wxColour c
    ( base_colour.Red(), base_colour.Green(), base_colour.Blue(), alpha );
  wxBrush brush( c );

  gc.SetBrush(brush);

  gc.DrawLines(4, p);
} // ingame_view::render_item_filled()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the box around an item on the screen.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_item_wireframe
( wxDC& dc, wxGraphicsContext& gc, const item_instance& item, 
  unsigned int index ) const
{
  wxPoint pos = 
    get_position_in_layer
    ( wxPoint((wxCoord)item.get_rendering_parameters().get_left(),
              (wxCoord)item.get_rendering_parameters().get_bottom()), index,
    item.get_rendering_parameters().get_height() );

  render_item_as_wireframe(dc, gc, item, pos, index);
} // ingame_view::render_item_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the identifier of an item.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer.
 */
void
bf::ingame_view::render_item_id
( wxDC& dc, const item_instance& item, unsigned int index ) const
{
  const wxRect box( get_bounding_box(item) );

  const wxSize size( zoom(box.width), zoom(box.height) );

  const wxPoint pos =
    get_position_in_layer
    ( wxPoint( box.x + unzoom(size.x / 2),box.y), index, 0 );
    //  ( zoom(box.x) + size.x / 2 - m_view.x,
    //  m_view.y + GetSize().y - zoom(box.y) );

  render_item_id(dc, item, pos);
} // ingame_view::render_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the sprite of an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 * \param z_order The items.
 */
void bf::ingame_view::render_item_as_sprite
( wxDC& dc, const item_instance& item, const wxPoint& pos,
  const std::multimap<int, item_instance*>& z_order ) const
{
  wxPoint p(pos);

  // the y coordinate of the ingame_view and the level are opposed
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
} // ingame_view::render_item_as_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the box of an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_item_as_wireframe
( wxDC& dc, wxGraphicsContext& gc, 
  const item_instance& item, const wxPoint& pos, unsigned int index ) const
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
      render_slope_steepness(dc, gc, item, pos, size, index);
    }
} // ingame_view::render_item_as_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an item on the screen at a given position.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the item on the screen.
 */
void bf::ingame_view::render_item_id
( wxDC& dc, const item_instance& item, const wxPoint& pos ) const
{
  wxString s(std_to_wx_string(item.get_id()));

  dc.SetBrush(*wxBLACK_BRUSH);
  dc.DrawText( s, pos.x - dc.GetTextExtent(s).x / 2, pos.y);
} // ingame_view::render_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an item as a point on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the point on the screen.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_item_as_point
( wxDC& dc, const item_instance& item, const wxPoint& pos,
  unsigned int index ) const
{
  dc.SetPen( get_display_pen(item, index) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  dc.DrawCircle(pos.x, pos.y, s_point_size);

  wxPoint p[2];

  p[0] = wxPoint( pos.x, pos.y - 2 * s_point_size );
  p[1] = wxPoint( pos.x, pos.y + 2 * s_point_size );

  dc.DrawPolygon(2, p);

  p[0] = wxPoint( pos.x - 2 * s_point_size, pos.y );
  p[1] = wxPoint( pos.x + 2 * s_point_size, pos.y );

  dc.DrawPolygon(2, p);
} // ingame_view::render_item_as_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param z_order The items.
 * \return true if the sprite was rendered.
 */
bool bf::ingame_view::render_sprite
( wxDC& dc, const item_instance& item, const wxPoint& pos,
  const std::multimap<int, item_instance*>& z_order) const
{
  std::pair<wxBitmap, wxPoint> spr( get_item_or_mimic_visual(item, z_order) );

  if ( spr.first.IsOk() )
    dc.DrawBitmap
      ( spr.first, pos.x + spr.second.x, pos.y + spr.second.y, true );

  return spr.first.IsOk();
} // ingame_view::render_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the bounding box of an item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_item_bounding_box
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
} // ingame_view::render_item_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the grip on curve of the slope.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_slope_curve_grip
( wxDC& dc, const item_instance& item, const wxPoint& pos, const wxSize& size,
  unsigned int index ) const
{
  if ( index == get_active_index() )
    {
      dc.SetPen( get_display_pen(item, index) );
      dc.SetBrush(*wxTRANSPARENT_BRUSH);

      double steepness;
      double left_x;
      double left_y;
      double right_x;
      double right_y;
      
      compute_slope_parameters
        (item, steepness, left_x, left_y, right_x, right_y);
      
      wxPoint p[2];

      if ( steepness < 0 )
        {
          p[0] = wxPoint( pos.x,  pos.y );
          p[1] = wxPoint( pos.x + size.x - 1 , pos.y - steepness );
        }
      else
        {
          p[0] = wxPoint( pos.x,  pos.y + steepness);
          p[1] = wxPoint( pos.x + size.x - 1 , pos.y );
        }

      dc.DrawRectangle
        ( p[0].x - s_grip_size / 2 + left_x, 
          p[0].y - s_grip_size / 2 - left_y,
          s_grip_size, s_grip_size );
      dc.DrawLine(p[0].x + left_x, p[0].y - left_y, p[0].x, p[0].y);      
      
      dc.DrawRectangle
        ( p[1].x - s_grip_size / 2 + right_x, 
          p[1].y - s_grip_size / 2 - right_y,
          s_grip_size, s_grip_size );
      dc.DrawLine(p[1].x + right_x, p[1].y - right_y, p[1].x, p[1].y);   
    }
} // ingame_view::render_slope_curve_grip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the steepness of the slope.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 * \param index The index of the layer in which the item is rendered.
 */
void bf::ingame_view::render_slope_steepness
( wxDC& dc, wxGraphicsContext& gc,
  const item_instance& item, const wxPoint& pos, const wxSize& size,
  unsigned int index ) const
{
  wxPen pen( wxColour( std_to_wx_string(item.get_class().get_color()) ), 
             1, wxSOLID );
  gc.SetPen( pen );
  gc.SetBrush(*wxTRANSPARENT_BRUSH);
  
  double steepness;
  double left_x;
  double left_y;
  double right_x;
  double right_y;
  
  compute_slope_parameters(item, steepness, left_x, left_y, right_x, right_y);
        
  wxPoint p[2];
  
  if ( steepness < 0 )
    {
      p[0] = wxPoint( pos.x,  pos.y );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y - steepness );
    }
  else
    {
      p[0] = wxPoint( pos.x,  pos.y + steepness);
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y );
    }

  wxGraphicsPath path = gc.CreatePath();
  
  path.MoveToPoint(p[0]);
  path.AddCurveToPoint
    ( p[0].x + left_x, p[0].y - left_y, p[1].x + right_x, 
      p[1].y - right_y, p[1].x, p[1].y );

  gc.StrokePath(path);
} // ingame_view::render_slope_steepness()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute slope parameters.
 * \param item The item to render.
 * \param steepness The steepness of the slope.
 * \param left_x The value of control_point.left.x field
 * \param left_y The value of control_point.left.y field
 * \param right_x The value of control_point.right.x field
 * \param right_y The value of control_point.right.y field
 */
void bf::ingame_view::compute_slope_parameters
( const item_instance& item, double & steepness, double & left_x,
  double & left_y, double & right_x, double & right_y ) const
{
  steepness = 0;
  left_x = 0;
  left_y = 0;
  right_x = 0;
  right_y = 0;

  if ( item.has_value( item.get_class().get_field("slope.steepness") ) )
    {
      real_type steepness_field;
      item.get_value( "slope.steepness", steepness_field );
      steepness = zoom( wxCoord(steepness_field.get_value() ) );
    }

  if ( item.has_value
       ( item.get_class().get_field("slope.control_point.left.x") ) )
    {
      real_type left_x_field;
      item.get_value( "slope.control_point.left.x", left_x_field );
      left_x = zoom( wxCoord(left_x_field.get_value() ) );
    }

  if ( item.has_value
       ( item.get_class().get_field("slope.control_point.left.y") ) )
    {
      real_type left_y_field;
      item.get_value( "slope.control_point.left.y", left_y_field );
      left_y = zoom( wxCoord(left_y_field.get_value() ) ); 
    }

  if ( item.has_value
       ( item.get_class().get_field("slope.control_point.right.x") ) )
    {
      real_type right_x_field;
      item.get_value( "slope.control_point.right.x", right_x_field );
      right_x = zoom( wxCoord(right_x_field.get_value() ) );
    }

  if ( item.has_value
       ( item.get_class().get_field("slope.control_point.right.y") ) )
    {
      real_type right_y_field;
      item.get_value( "slope.control_point.right.y", right_y_field );
      right_y = zoom( wxCoord(right_y_field.get_value() ) );
    }
} // ingame_view::compute_slope_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a non valid item on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_non_valid_item
( wxDC& dc, const item_instance& item, unsigned int index ) const
{
  wxPen pen( *wxRED );

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
} // ingame_view::render_non_valid_item()


/*----------------------------------------------------------------------------*/
/**
 * \brief Render a non valid item as point on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 */
void bf::ingame_view::render_non_valid_item_as_point
( wxDC& dc, const item_instance& item, const wxPoint& pos ) const
{
  wxPoint p[2];

  p[0] = wxPoint( pos.x - 2 * s_point_size, pos.y - 2 * s_point_size );
  p[1] = wxPoint( pos.x + 2 * s_point_size, pos.y + 2 * s_point_size );

  dc.DrawPolygon(2, p);

  p[0] = wxPoint( pos.x - 2 * s_point_size, pos.y + 2 * s_point_size );
  p[1] = wxPoint( pos.x + 2 * s_point_size, pos.y - 2 * s_point_size);

  dc.DrawPolygon(2, p);
} // ingame_view::render_non_valid_item_as_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a non valid item as point on the screen.
 * \param dc The device context for the drawings.
 * \param item The item to render.
 * \param pos The position of the box.
 * \param size The size of the box.
 */
void bf::ingame_view::render_non_valid_item_box
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
} // ingame_view::render_non_valid_item_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the grip of the selected item.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_grip( wxDC& dc, unsigned int index ) const
{
  CLAW_PRECOND( has_selection() );

  const item_instance* main_selection( get_level().get_main_selection() );
  wxRect b_box( get_bounding_box(*main_selection) );

  b_box.SetPosition
    ( get_position_in_layer
      ( wxPoint(b_box.GetLeft(), b_box.GetBottom()), index, 0));

  wxRect box
    ( b_box.GetLeft(), b_box.GetTop(),
     zoom(b_box.GetWidth()),
     zoom(b_box.GetHeight()) );

  dc.SetPen(*wxRED_PEN);
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  dc.DrawRectangle
    ( box.GetLeft() - s_grip_size, box.GetTop() - s_grip_size,
      s_grip_size, s_grip_size );
  dc.DrawRectangle
    ( box.GetRight(), box.GetTop() - s_grip_size, s_grip_size, s_grip_size );
  dc.DrawRectangle
    ( box.GetRight(), box.GetBottom(), s_grip_size, s_grip_size );
  dc.DrawRectangle
    ( box.GetLeft() - s_grip_size, box.GetBottom(), s_grip_size, s_grip_size );

  if ( (main_selection->get_rendering_parameters().get_height() != 0)
       && (main_selection->get_rendering_parameters().get_width() != 0) )
    {
      dc.DrawRectangle
        ( box.GetLeft() - s_grip_size,
          box.GetTop() + box.GetHeight() / 2 - s_grip_size / 2,
          s_grip_size, s_grip_size );
      dc.DrawRectangle
        ( box.GetRight(), box.GetTop() + box.GetHeight() / 2 - s_grip_size / 2,
          s_grip_size, s_grip_size );
      dc.DrawRectangle
        ( box.GetLeft() + box.GetWidth() / 2 - s_grip_size / 2,
          box.GetTop() - s_grip_size, s_grip_size, s_grip_size );
      dc.DrawRectangle
        ( box.GetLeft() + box.GetWidth() / 2 - s_grip_size / 2,
          box.GetBottom(), s_grip_size, s_grip_size );
    }
} // ingame_view::render_grip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param z_order The items.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_drag
( wxDC& dc, wxGraphicsContext& gc, 
  const std::multimap<int, item_instance*>& z_order, unsigned int index ) const
{
  if ( m_drag_info != NULL )
    switch(m_drag_info->drag_mode)
      {
      case drag_info::drag_mode_selection:
        render_drag_mode_selection(dc,index);
        break;
      case drag_info::drag_mode_move:
        render_drag_mode_move(dc, gc, z_order,index);
        break;
      case drag_info::drag_mode_size:
        render_drag_mode_size(dc,index);
        break;      
      case drag_info::drag_mode_slope:
        render_drag_mode_slope(dc, gc, index);
        break;
      default:
        {
          // nothing to do
        }
      }
} // ingame_view::render_drag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of selection.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_drag_mode_selection
( wxDC& dc, unsigned int index ) const
{
  dc.SetPen( wxPen( *wxRED, 1, wxLONG_DASH ) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  wxPoint p[4];
  wxPoint pos_view = compute_local_view_position();
  p[0].x = zoom(m_drag_info->mouse_origin.x) - pos_view.x;
  p[0].y =  GetSize().y - zoom(m_drag_info->mouse_origin.y - 1) + pos_view.y;
  
  p[1] = wxPoint( p[0].x + zoom(m_drag_info->delta().x), p[0].y );
  p[2] = wxPoint( p[1].x, p[0].y - zoom(m_drag_info->delta()).y );
  p[3] = wxPoint( p[0].x, p[2].y );

  dc.DrawPolygon(4, p);
} // ingame_view::render_drag_mode_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of selection.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param z_order The item.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_drag_mode_move
( wxDC& dc, wxGraphicsContext& gc, 
  const std::multimap<int, item_instance*>& z_order, unsigned int index ) const
{
  item_selection::const_iterator it;
  const item_selection& selection( get_level().get_selection() );
  wxPoint pos_view = compute_local_view_position();

  for (it=selection.begin(); it!=selection.end(); ++it)
    {
      wxPoint pos
        ( zoom( (wxCoord)(*it)->get_rendering_parameters().get_left()
                + m_drag_info->delta().x ) - pos_view.x,
          pos_view.y + GetSize().y - zoom(m_drag_info->delta().y)
          - zoom((wxCoord)(*it)->get_rendering_parameters().get_bottom())
          - zoom((wxCoord)(*it)->get_rendering_parameters().get_height()) );

      render_item(dc, gc, **it, pos, get_active_index(), z_order);
    }
} // ingame_view::render_drag_mode_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of sizing.
 * \param dc The device context for the drawings.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_drag_mode_size
( wxDC& dc, unsigned int index ) const
{
  dc.SetPen( wxPen( *wxRED, 1, wxSHORT_DASH ) );
  dc.SetBrush(*wxTRANSPARENT_BRUSH);

  wxPoint p[4];
  wxPoint pos_view = compute_local_view_position();

  // m_drag_info->mouse_origin is the immobile corner
  // m_drag_info->mouse_position is the moving corner
  p[0].x = zoom(m_drag_info->mouse_origin.x) - pos_view.x;
  p[0].y = GetSize().y - zoom(m_drag_info->mouse_origin.y - 1) + pos_view.y;

  p[2].x = zoom(m_drag_info->mouse_position.x) - pos_view.x;
  p[2].y = GetSize().y - zoom(m_drag_info->mouse_position.y - 1) + pos_view.y;

  p[1] = wxPoint( p[2].x, p[0].y );
  p[3] = wxPoint( p[0].x, p[2].y );

  dc.DrawPolygon(4, p);
} // ingame_view::render_drag_mode_size()


/*----------------------------------------------------------------------------*/
/**
 * \brief Render the result of the drag in a situation of changing slope curve.
 * \param dc The device context for the drawings.
 * \param gc The graphics context for the drawings.
 * \param index The index of the layer.
 */
void bf::ingame_view::render_drag_mode_slope
( wxDC& dc, wxGraphicsContext& gc, unsigned int index ) const
{
  wxPen pen
    ( wxColour
      ( std_to_wx_string(m_drag_info->picked_item->get_class().get_color()) ), 
      1, wxSOLID );
  gc.SetPen( pen );
  gc.SetBrush(*wxTRANSPARENT_BRUSH);
  
  double steepness;
  double left_x;
  double left_y;
  double right_x;
  double right_y;
  
  compute_slope_parameters
    (*m_drag_info->picked_item, steepness, left_x, left_y, right_x, right_y);
  
  wxPoint gap
    ( m_drag_info->mouse_position.x - m_drag_info->mouse_origin.x,
      m_drag_info->mouse_position.y - m_drag_info->mouse_origin.y );
  wxPoint pos_view = compute_local_view_position();
  wxPoint pos
    ( zoom( (wxCoord)m_drag_info->picked_item->
            get_rendering_parameters().get_left() ) - pos_view.x,
      pos_view.y + GetSize().y 
      - zoom((wxCoord)m_drag_info->picked_item->
             get_rendering_parameters().get_bottom())
      - zoom((wxCoord)m_drag_info->picked_item->
             get_rendering_parameters().get_height()) );
  const wxSize size
    ( (int)m_drag_info->picked_item->get_rendering_parameters().get_width(),
      (int)m_drag_info->picked_item->get_rendering_parameters().get_height() );

  if ( m_drag_info->left_side )
    {
      left_x = gap.x;
      left_y = gap.y;
    }
  else
    {
      right_x = gap.x;
      right_y = gap.y;
    }

  wxPoint p[2];

  if ( steepness < 0 )
    {
      p[0] = wxPoint( pos.x,  pos.y );
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y - steepness );
    }
  else
    {
      p[0] = wxPoint( pos.x,  pos.y + steepness);
      p[1] = wxPoint( pos.x + size.x - 1 , pos.y );
    }

  wxGraphicsPath path = gc.CreatePath();
  
  path.MoveToPoint(p[0]);
  path.AddCurveToPoint
    ( p[0].x + left_x, p[0].y - left_y, p[1].x + right_x, 
      p[1].y - right_y, p[1].x, p[1].y );

  gc.StrokePath(path);
} // ingame_view::render_drag_mode_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the grid on the screen.
 * \param dc The device context for the drawings.
 */
void bf::ingame_view::render_grid( wxDC& dc ) const
{
  if ( !empty() && m_display_grid )
    {
      wxColour color(200, 200, 200);
      dc.SetPen( wxPen( color, 1, wxSHORT_DASH ) );
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
void bf::ingame_view::render_grid_vertical_lines( wxDC& dc ) const
{
  const int width = get_active_layer().get_width();
  const int height = get_active_layer().get_height();
  const wxCoord offset = m_grid.get_offset().GetX();
  const wxCoord step = m_grid.get_step().GetX();
  const wxCoord bound =
    unzoom( m_view.x + std::min(zoom(width), GetSize().x) );

  for ( int column = unzoom(m_view.x) / step;
        (column * step) + offset < bound; ++column )
    if ( (column * step) + offset > unzoom(m_view.x) )
      {
        wxPoint p[2];
        p[0] = wxPoint
          ( zoom((column * step) + offset) - m_view.x, GetSize().y );
        p[1] = wxPoint
          ( p[0].x,
            GetSize().y - std::min(zoom(height) + m_view.y, GetSize().y) );
        dc.DrawPolygon(2, p);
      }
} // render_grid_vertical_lines()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the horizontal lines of the grid on the screen.
 * \param dc The device context for the drawings.
 */
void bf::ingame_view::render_grid_horizontal_lines( wxDC& dc ) const
{
  const int width = get_active_layer().get_width();
  const int height = get_active_layer().get_height();
  const wxCoord offset = m_grid.get_offset().GetY();
  const wxCoord step = m_grid.get_step().GetY();
  const wxCoord bound =
    unzoom( m_view.y + std::min(zoom(height), GetSize().y) );

  for ( int line = unzoom(m_view.y) / step;
        (line * step) + offset < bound; ++line )
    if ( (line * step) + offset > unzoom(m_view.y) )
      {
        wxPoint p[2];
        p[0] = wxPoint
          ( 0, GetSize().y - zoom( (line * step) + offset ) + m_view.y );
        p[1] = wxPoint
          ( std::min( GetSize().x, zoom(width) - m_view.x), p[0].y );
        dc.DrawPolygon(2, p);
      }
} // ingame_view::render_grid_horizontal_lines()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual (sprite and position) of an item.
 * \param item The item to render.
 * \return The sprite and the position where it has to be rendered.
 */
std::pair<wxBitmap, wxPoint> bf::ingame_view::get_item_visual
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
} // ingame_view::get_item_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual (sprite and position) of an item.
 * \param item The item to render.
 * \param rendering_attributes The rendering attributes to combine with sprite.
 * \return The sprite and the position where it has to be rendered.
 */
std::pair<wxBitmap, wxPoint> bf::ingame_view::get_item_visual
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
} // ingame_view::get_item_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return if item has a visual.
 * \param item The item to render.
 * \param z_order The items.
 */
bool bf::ingame_view::has_visual
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
} // ingame_view::has_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual (sprite and position) of an item.
 * \param item The item to render.
 * \param z_order The items.
 * \return The sprite and the position where it has to be rendered.
 */
std::pair<wxBitmap, wxPoint> bf::ingame_view::get_item_or_mimic_visual
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
} // ingame_view::get_item_or_mimic_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pen used for displaying an item.
 * \param item The item to display.
 * \param index The index of the layer in which the item is rendered.
 */
wxPen bf::ingame_view::get_display_pen
( const item_instance& item, unsigned int index ) const
{
  wxPen result( wxColour( std_to_wx_string(item.get_class().get_color()) ) );

  if ( index != get_active_index() )
    result.SetStyle( wxLONG_DASH );

  if ( get_level().item_is_selected(index, &item) )
    {
      if ( index == get_active_index() )
        {
          if ( !get_level().item_is_main_selection(&item) )
            result.SetStyle( wxSHORT_DASH );

          result.SetColour(*wxRED);
        }
      else
        result.SetColour( wxT("#800000") );
    }

  return result;
} // ingame_view::get_display_pen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer on which we are working.
 */
const bf::layer& bf::ingame_view::current_layer()
{
  return m_history.get_level().get_active_layer();
} // ingame_view::current_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if there exists a selected item in a given position.
 * \param pos The position of the point.
 */
bool bf::ingame_view::exist_selected_item( const wxPoint& pos ) const
{
  return first_selected_item( pos ) != NULL;
} // ingame_view::exist_selected_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if there exists a selected item in a given position.
 * \param pos The position of the point.
 */
bf::item_instance*
bf::ingame_view::first_selected_item( const wxPoint& pos ) const
{
  item_instance* result = NULL;

  item_selection::const_iterator it;
  const item_selection& selection( get_level().get_selection() );

  for ( it=selection.begin(); (it!=selection.end()) && (result==NULL); ++it )
    {
      wxSize size( (int)(*it)->get_rendering_parameters().get_width(),
                   (int)(*it)->get_rendering_parameters().get_height() );
      wxPoint point( (int)(*it)->get_rendering_parameters().get_left(),
                     (int)(*it)->get_rendering_parameters().get_bottom());

      if ( (size.x == 0) || (size.y == 0) )
        {
          size.x = 2*s_point_size;
          size.y = 2*s_point_size;
          point.x -= s_point_size;
          point.y -= s_point_size;
        }

      if ( ( point.x <= pos.x ) &&
           ( point.x + size.x >= pos.x ) &&
           ( point.y <= pos.y ) &&
           ( point.y + size.y >= pos.y ) )
        result = *it;
    }

  return result;
} // ingame_view::first_selected_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick items found near a given point.
 * \param pos The position of the point.
 * \param items The selected items.
 */
void bf::ingame_view::select_item_at
( const wxPoint& pos, std::set<item_instance*>& items )
{
  items.clear();

  if ( !empty() )
    {
      layer& the_layer( m_history.get_level().get_active_layer() );

      std::set<item_instance*> choice;

      for ( layer::item_iterator it=the_layer.item_begin();
            it!=the_layer.item_end(); ++it )
        {
          const wxRect box( get_bounding_box( *it ) );

          if ( box.Contains(pos) )
            choice.insert(&(*it));
        }

      if ( !choice.empty() )
        {
          item_choice_frame dlg(this, get_level().get_selection(), choice);
          if ( dlg.ShowModal() == wxID_OK )
            dlg.get_selection(items);
        }
    }
} // ingame_view::select_item_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick all items intersecting a box
 * \param item (out) The items selected.
 * \param box The box where to pick the items.
 */
void bf::ingame_view::pick_item
( std::list<item_instance*>& item, const wxRect& box ) const
{
  if ( !empty() )
    {
      layer& the_layer( m_history.get_level().get_active_layer() );

      for (layer::item_iterator it=the_layer.item_begin();
           it!=the_layer.item_end(); ++it)
        {
          wxRect item_box = get_bounding_box( *it );

          if ( box.Intersects(item_box) )
            item.push_front( &(*it) );
        }
    }
} // ingame_view::pick_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the selection on an item.
 * \param item The item to process.
 * \remark The function does nothing if item == NULL.
 */
void bf::ingame_view::toggle_selection( item_instance* item )
{
  if ( item != NULL )
    {
      if ( get_level().item_is_selected(item) )
        {
          get_level().remove_from_selection(item);
          m_layout.get_properties_frame().remove_item(item);
        }
      else
        {
          get_level().add_to_selection(item, true);
          m_layout.get_properties_frame().add_item(item);
        }

      Refresh();
    }
} // ingame_view::toggle_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the selection.
 * \param item The item to select.
 */
void bf::ingame_view::add_selection( item_instance* item )
{
  if ( item != NULL )
    {
      get_level().add_to_selection(item, true);
      m_layout.get_properties_frame().add_item(item);
      Refresh();
    }
} // ingame_view::add_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add some items in the selection.
 * \param item The items to select.
 */
void bf::ingame_view::add_selection( const std::list<item_instance*>& item )
{
  if ( !item.empty() )
    {
      std::list<item_instance*>::const_iterator it;

      for (it=item.begin(); it!=item.end(); ++it)
        get_level().add_to_selection(*it);

      m_layout.get_properties_frame().add_items(item);
      get_level().add_to_selection(item.front(), true);
      Refresh();
    }
} // ingame_view::add_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selected items.
 * \param item The items to select.
 */
void bf::ingame_view::set_selection( const std::list<item_instance*>& item)
{
  if ( item.empty() )
    clear_selection();
  else
    set_selection( item, item.front() );
} // ingame_view::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selected items.
 * \param item The items to select.
 * \param selected Set this item as the selected item.
 * \param add Indicates if the items are added at the selection.
 */
void bf::ingame_view::set_selection
( const std::list<item_instance*>& item, item_instance* selected, bool add )
{
  CLAW_PRECOND( std::find( item.begin(), item.end(), selected ) != item.end() );

  if ( !add )
    {
      get_level().clear_selection();
      m_layout.get_properties_frame().clear();
    }

  add_selection( item );
  get_level().add_to_selection(selected, true);

  Refresh();
} // ingame_view::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selected item.
 * \param item The item to select.
 */
void bf::ingame_view::set_selection( item_instance* item )
{
  if ( item == NULL )
    clear_selection();
  else
    {
      std::list<item_instance*> item_list;
      item_list.push_front(item);
      set_selection( item_list );
    }
} // ingame_view::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the focus box of an item.
 * \param item The item to bound.
 */
wxRect bf::ingame_view::get_bounding_box( const item_instance& item ) const
{
  wxRect result;

  result.x = (int)item.get_rendering_parameters().get_left();
  result.y = (int)item.get_rendering_parameters().get_bottom();
  result.width = (int)item.get_rendering_parameters().get_width();
  result.height = (int)item.get_rendering_parameters().get_height();

  if ( result.width == 0 )
    {
      result.x -= s_point_size;
      result.width = 2 * s_point_size;
    }

  if ( result.height == 0 )
    {
      result.y -= s_point_size;
      result.height = 2 * s_point_size;
    }

  return result;
} // ingame_view::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the box bounding box of an item and its sprite, if any.
 * \param item The item to bound.
 */
wxRect bf::ingame_view::get_presence_box( const item_instance& item ) const
{
  wxRect result = get_bounding_box(item);

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
} // ingame_view::get_presence_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the selected items at the position where the mouse button was
 *        released.
 * \param add Indicates if the items are added at the selection.
 */
void bf::ingame_view::copy_selection(bool add)
{
  do_action
    ( new action_copy_selection
      ( get_level(), m_drag_info->delta().x, m_drag_info->delta().y, add ) );
  m_parent.update_toolbar();
} // ingame_view::copy_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the selected items at the position where the mouse button was
 *        released.
 */
void bf::ingame_view::move_selection()
{
  do_action
    ( new action_move_selection
      ( get_level(), m_drag_info->delta().x, m_drag_info->delta().y ) );
} // ingame_view::move_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Rotate the selected items.
 * \param clockwise Indicates if the rotation the clockwise.
 */
void bf::ingame_view::rotate_selection( bool clockwise )
{
  do_action
    ( new action_rotate_selection( get_level(), clockwise ) );
} // ingame_view::rotate_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the gap between the grid and the selected item.
 * \param item_position The coordinate of the selected item.
 * \param size_item The size of the selected item.
 * \param offset_grid The oofset of the grid.
 * \param step_grid The step of the grid.
 */
int bf::ingame_view::update_coordinate_magnetism
( unsigned int item_position, unsigned int size_item, unsigned int offset_grid,
  unsigned int step_grid )
{
  int result;
  int gap[4];
  gap[0] = (item_position - offset_grid) % step_grid;
  gap[1] = step_grid - gap[0];
  gap[2] = (item_position - offset_grid + size_item) % step_grid;
  gap[3] = step_grid - gap[2];

  unsigned int gap_positive;
  if ( gap[0] <= gap[2] )
    gap_positive = gap[0];
  else
    gap_positive = gap[2];

  unsigned int gap_negative;
  if ( gap[1] <= gap[3] )
    gap_negative = gap[1];
  else
    gap_negative = gap[3];

  result = 0;

  if ( gap_positive <= m_grid.get_magnetism_force() )
    {
      if ( gap_positive <= gap_negative )
        result = -gap_positive;
    }
  else if ( gap_negative <= m_grid.get_magnetism_force() )
    result = gap_negative;

  return result;
} // ingame_view::update_coordinate_magnetism()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the position of the mouse according to the constraints.
 * \param position The position of the mouse in the level.
 */
void bf::ingame_view::update_mouse_position( const wxPoint& position )
{
  const item_instance* main_selection( get_level().get_main_selection() );

  // magnetism
  wxPoint item_position;
  item_position.x = position.x
    + (wxCoord)main_selection->get_rendering_parameters().get_left()
    - m_drag_info->mouse_origin.x;
  item_position.y = position.y
    + (wxCoord)main_selection->get_rendering_parameters().get_bottom()
    - m_drag_info->mouse_origin.y;

  wxPoint pos = position;

  pos.x +=
    update_coordinate_magnetism
    ( item_position.x,
      (unsigned int)main_selection->get_rendering_parameters().get_width(),
      m_grid.get_offset().x, m_grid.get_step().x );
  pos.y +=
    update_coordinate_magnetism
    ( item_position.y,
      (unsigned int)main_selection->get_rendering_parameters().get_height(),
      m_grid.get_offset().y, m_grid.get_step().y );

  if ( m_drag_info->x_active )
    m_drag_info->mouse_position.x = pos.x;

  if ( m_drag_info->y_active )
    m_drag_info->mouse_position.y = pos.y;
} // ingame_view::update_mouse_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event Move the grid of one pixel.
 * \param keycode The keycode pressed.
 */
void bf::ingame_view::move_grid(int keycode)
{
  wxSize new_offset = m_grid.get_offset();

  switch( keycode )
    {
    case WXK_LEFT:
      if ( new_offset.x == 0 )
        new_offset.x = m_grid.get_step().x - 1;
      else
        --new_offset.x;
      break;
    case WXK_DOWN:
      if ( new_offset.y == 0 )
        new_offset.y = m_grid.get_step().y - 1;
      else
        --new_offset.y;
      break;
    case WXK_RIGHT:
      new_offset.x = ( new_offset.x + 1 ) % m_grid.get_step().x;
      break;
    case WXK_UP:
      new_offset.y = ( new_offset.y + 1 ) % m_grid.get_step().y;
      break;
    }

  m_grid.set_offset(new_offset);

  Refresh();
} // ingame_view::move_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param keycode The keycode pressed.
 */
void bf::ingame_view::move_view(int keycode)
{
  switch( keycode )
    {
#if 0
    case WXK_LEFT:
      set_view_position
        ( m_view.x - m_parent.get_h_scrollbar().GetPageSize(), m_view.y );
      break;
    case WXK_DOWN:
      set_view_position
        ( m_view.x, m_view.y - m_parent.get_v_scrollbar().GetPageSize() );
      break;
    case WXK_UP:
      set_view_position
        ( m_view.x, m_view.y + m_parent.get_v_scrollbar().GetPageSize() );
      break;
    case WXK_RIGHT:
      set_view_position
        ( m_view.x + m_parent.get_h_scrollbar().GetPageSize(), m_view.y );
      break;
#else
    case WXK_LEFT:
      set_view_position
        ( m_view.x - m_parent.get_h_scrollbar().GetPageSize() * 
          get_level().get_width() / get_layer_view_size().x, m_view.y );
      break;
    case WXK_DOWN:
      set_view_position
        ( m_view.x, m_view.y - m_parent.get_v_scrollbar().GetPageSize() * 
          get_level().get_height() / get_layer_view_size().y );
      break;
    case WXK_UP:
      set_view_position
        ( m_view.x, m_view.y + m_parent.get_v_scrollbar().GetPageSize() * 
          get_level().get_height() / get_layer_view_size().y );
      break;
    case WXK_RIGHT:
      set_view_position
        ( m_view.x + m_parent.get_h_scrollbar().GetPageSize() * 
          get_level().get_width() / get_layer_view_size().x, m_view.y );
      break;
#endif
    }

  Refresh();
} // ingame_view::move_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the mouse position.
 * \param point The position.
 */
void bf::ingame_view::write_mouse_position(const wxPoint& point)
{
  bool size_mode = false;

  if ( m_drag_info != NULL )
    if ( m_drag_info->drag_mode == drag_info::drag_mode_size )
      size_mode = true;

  if ( size_mode )
    {
      wxRect box = get_drag_mode_size_box();

      m_parent.GetStatusBar()->SetStatusText
        ( wxString::Format( _("x=%d"), box.x), 1 );
      m_parent.GetStatusBar()->SetStatusText
        ( wxString::Format( _("y=%d"), box.y), 2 );
      m_parent.GetStatusBar()->SetStatusText
        ( wxString::Format( _("width=%d"), box.width), 3 );
      m_parent.GetStatusBar()->SetStatusText
        ( wxString::Format( _("height=%d"), box.height), 4 );
    }
  else
    {
      SetToolTip(NULL);

      m_parent.GetStatusBar()->SetStatusText
        ( wxString::Format( _("x=%d"), point.x), 1 );
      m_parent.GetStatusBar()->SetStatusText
        ( wxString::Format( _("y=%d"), point.y), 2 );

      item_instance* item = pick_first_item(point);
      wxString class_str, id_str, dist_str;

      if ( item != NULL )
        {
          set_tooltip(item);
          class_str = std_to_wx_string(item->get_class().get_class_name());

          if ( !item->get_id().empty() )
            id_str =
              wxString::Format
              ( _("id = '%s'"), std_to_wx_string(item->get_id()).c_str() );

          if ( m_history.get_level().has_selection() )
            {
              const item_rendering_parameters& r1
                ( m_history.get_level().get_main_selection()->
                  get_rendering_parameters() );
              const item_rendering_parameters& r2
                ( item->get_rendering_parameters() );

              dist_str =
                wxString::Format
                ( _("(%.2f, %.2f)"),
                  r1.get_horizontal_middle() - r2.get_horizontal_middle(),
                  r1.get_vertical_middle() - r2.get_vertical_middle() );
            }
        }

      m_parent.GetStatusBar()->SetStatusText( class_str, 0 );
      m_parent.GetStatusBar()->SetStatusText( id_str, 3 );
      m_parent.GetStatusBar()->SetStatusText( dist_str, 4 );
    }
} // ingame_view::write_mouse_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the tooltip according to the check result of a given item.
 * \param item The item.
 */
void bf::ingame_view::set_tooltip( item_instance* item )
{
  wxString tooltip;

  if ( m_check_result.contains( &get_active_layer(), item ) )
    {
      tooltip =
          wxString::Format
          ( _("%s:"),
              std_to_wx_string(item->get_class().get_class_name()).c_str() );

      item_check_result::const_iterator it;
      const item_check_result result =
        m_check_result.get_layer_result
        ( &get_active_layer() ).get_item_result(item);

      for ( it=result.begin(); it!=result.end(); ++it )
        if ( it->get_cause().empty() )
          tooltip +=
            wxString::Format
            ( _("\n%s"),
              wxGetTranslation( std_to_wx_string(it->get_message()) ) );
        else
          tooltip +=
            wxString::Format
            ( _("\n%s: %s"),
              wxGetTranslation( std_to_wx_string(it->get_cause()) ),
              wxGetTranslation( std_to_wx_string(it->get_message()) ) );
    }

  SetToolTip(tooltip);
} // ingame_view::set_tooltip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the user tries to resize an item and set the drag mode if it
 *        is the case.
 * \param pos The position of the mouse in the layer.
 * \pre m_drag_info->picked_item is the item on which the user clicked.
 * \pre pos is in the bounding box of m_drag_info->picked_item.
 */
bool bf::ingame_view::set_drag_mode_size( const wxPoint& pos )
{
  bool grip = true;

  if(  has_selection() )
    {
      item_instance* selection( get_level().get_main_selection() );
      const wxRect box = get_bounding_box( *selection );
      const wxSize s( s_grip_size, s_grip_size );
      wxPoint mouse_pos;

      const wxRect top_left( box.GetTopLeft() - s, s );
      const wxRect top_right( box.GetTopRight() - wxSize(0, s_grip_size), s );
      const wxRect bottom_left
        ( box.GetBottomLeft() - wxSize(s_grip_size, 0), s );
      const wxRect bottom_right( box.GetBottomRight(), s );

      if ( top_left.Contains(pos) )
        {
          m_drag_info->mouse_origin = box.GetBottomRight();
          mouse_pos = box.GetTopLeft();
        }
      else if ( top_right.Contains(pos) )
        {
          m_drag_info->mouse_origin = box.GetBottomLeft();
          mouse_pos = box.GetTopRight();
        }
      else if ( bottom_left.Contains(pos) )
        {
          m_drag_info->mouse_origin = box.GetTopRight();
          mouse_pos = box.GetBottomLeft();
        }
      else if ( bottom_right.Contains(pos) )
        {
          m_drag_info->mouse_origin = box.GetTopLeft();
          mouse_pos = box.GetBottomRight();
        }
      else if ( (selection->get_rendering_parameters().get_width() != 0)
                && (selection->get_rendering_parameters().get_height() != 0) )
        {
          const wxCoord h
            ((wxCoord)selection->get_rendering_parameters().get_height() / 2);
          const wxCoord w
            ((wxCoord)selection->get_rendering_parameters().get_width() / 2);
          const wxRect middle_left
            ( box.GetLeft() - s_grip_size,
              box.GetTop() + h - s_grip_size / 2, s_grip_size, s_grip_size );
          const wxRect middle_right
            ( box.GetRight(), box.GetTop() + h - s_grip_size / 2,
              s_grip_size, s_grip_size );
          const wxRect middle_bottom
            ( box.GetLeft() + w - s_grip_size / 2,
              box.GetTop() - s_grip_size, s_grip_size, s_grip_size );
          const wxRect middle_top
            ( box.GetLeft() + w - s_grip_size / 2,
              box.GetBottom(), s_grip_size, s_grip_size );

          if ( middle_left.Contains(pos) )
            {
              m_drag_info->mouse_origin = box.GetTopRight();
              mouse_pos = box.GetTopLeft();
              m_drag_info->y_active = false;
            }
          else if ( middle_right.Contains(pos) )
            {
              m_drag_info->mouse_origin = box.GetTopLeft();
              mouse_pos = box.GetTopRight();
              m_drag_info->y_active = false;
            }
          else if ( middle_bottom.Contains(pos) )
            {
              m_drag_info->mouse_origin = box.GetBottomLeft();
              mouse_pos = box.GetTopLeft();
              m_drag_info->x_active = false;
            }
          else if ( middle_top.Contains(pos) )
            {
              m_drag_info->mouse_origin = box.GetTopLeft();
              mouse_pos = box.GetBottomRight();
              m_drag_info->x_active = false;
            }
          else
            grip = false;
        }
      else
        grip = false;

      if ( grip )
        {
          m_drag_info->picked_item = selection;
          m_drag_info->drag_mode = drag_info::drag_mode_size;
          m_drag_info->mouse_position = mouse_pos;
        }
    }
  else
    grip = false;

  return grip;
} // ingame_view::set_drag_mode_size()


/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the user tries to change slope curve and set the drag mode 
 * if it is the case.
 * \param pos The position of the mouse in the layer.
 * \pre m_drag_info->picked_item is the item on which the user clicked.
 * \pre pos is in the bounding box of m_drag_info->picked_item.
 */
bool bf::ingame_view::set_drag_mode_slope( const wxPoint& pos )
{
  bool grip = true;

  if(  has_selection() )
    {
      item_instance* item( get_level().get_main_selection() );

      if ( item->get_class().get_class_name() == "bear::slope" )
        {
          const wxRect box = get_bounding_box( *item );

          double steepness;
          double left_x;
          double left_y;
          double right_x;
          double right_y;
          
          compute_slope_parameters
            (*item, steepness, left_x, left_y, right_x, right_y);

          steepness = 
            steepness < 0 ? - unzoom( - steepness ) : unzoom( steepness );
          
          left_x = left_x < 0 ? - unzoom( - left_x ) : unzoom( left_x );
          left_y = left_y < 0 ? - unzoom( - left_y ) : unzoom( left_y );
          right_x = right_x < 0 ? - unzoom( - right_x ) : unzoom( right_x );
          right_y = right_y < 0 ? - unzoom( - right_y ) : unzoom( right_y );

          wxPoint p[2];
          
          if ( steepness < 0 )
            {
              p[0] = wxPoint( box.GetLeft(),  box.GetBottom() );
              p[1] = wxPoint( box.GetRight(), box.GetBottom() + steepness );
            }
          else
            {
              p[0] = wxPoint( box.GetLeft(),  box.GetBottom() - steepness);
              p[1] = wxPoint( box.GetRight(), box.GetBottom() );
            }
          
          const wxRect left_rect
            ( p[0].x - s_grip_size / 2 + left_x, 
              p[0].y - s_grip_size / 2 - left_y,
              s_grip_size, s_grip_size );

          const wxRect right_rect
             ( p[1].x - s_grip_size / 2 + right_x, 
               p[1].y - s_grip_size / 2 - right_y,
              s_grip_size, s_grip_size );
          
          wxPoint mouse_pos;
          std::cout << p[1].x - s_grip_size / 2 + right_x << " - "
                    << p[1].y - s_grip_size / 2 - right_y << " ; "
                    << pos.x << " - "
                    << pos.y << " :" 
                    << right_x << " - " << std::endl;

          if ( left_rect.Contains(pos) )
            {
              m_drag_info->mouse_origin = wxPoint( p[0].x, p[0].y);
              mouse_pos = wxPoint( p[0].x + left_x, p[0].y - left_y);
              m_drag_info->left_side = true;
            }
          else if ( right_rect.Contains(pos) )
            {
              m_drag_info->mouse_origin = wxPoint( p[1].x, p[1].y);
              mouse_pos = wxPoint( p[1].x + left_x, p[1].y - right_y);
              m_drag_info->left_side = false;
            }
          else
            grip = false;
          
          if ( grip )
            {
              m_drag_info->picked_item = item;
              m_drag_info->drag_mode = drag_info::drag_mode_slope;
              m_drag_info->mouse_position = mouse_pos;
            }
        }
      else
        grip = false;
    }
  else
    grip = false;

  std::cout << "grip=" << grip << std::endl;

  return grip;
} // ingame_view::set_drag_mode_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the box when resizing an item.
 */
wxRect bf::ingame_view::get_drag_mode_size_box() const
{
  CLAW_PRECOND( m_drag_info != NULL );
  CLAW_PRECOND( m_drag_info->drag_mode == drag_info::drag_mode_size );
  CLAW_PRECOND( m_drag_info->picked_item != NULL );

  wxRect result;

  if ( m_drag_info->x_active )
    {
      result.x =
        std::min( m_drag_info->mouse_position.x, m_drag_info->mouse_origin.x );
      result.width =
        std::abs
        ( m_drag_info->mouse_position.x - m_drag_info->mouse_origin.x ) + 1;
    }
  else
    {
      result.x = m_drag_info->mouse_origin.x;
      result.width =
        (int)m_drag_info->picked_item->get_rendering_parameters().get_width();
    }

  if ( m_drag_info->y_active )
    {
      result.y =
        std::min( m_drag_info->mouse_position.y, m_drag_info->mouse_origin.y );
      result.height =
        std::abs
        ( m_drag_info->mouse_position.y - m_drag_info->mouse_origin.y ) + 1;
    }
  else
    {
      result.y = m_drag_info->mouse_origin.y;
      result.height =
        (int)m_drag_info->picked_item->get_rendering_parameters().get_height();
    }

  return result;
} // ingame_view::get_drag_mode_size_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in move mode.
 * \param ctrl Control key is pressed.
 * \param shift Shift key is pressed.
 * \param alt Alt key is pressed.
 */
void bf::ingame_view::apply_drag_mode_move( bool ctrl, bool shift, bool alt )
{
  CLAW_PRECOND( m_drag_info->drag_mode == drag_info::drag_mode_move );

  if ( m_grid.get_magnetism_active() && !shift && (has_selection()) )
    update_mouse_position(m_drag_info->mouse_position);

  if ( ctrl )
    copy_selection( alt );
  else
    move_selection();
} // ingame_view::apply_drag_mode_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in pick mode.
 * \param ctrl Control key is pressed.
 * \param alt Alt key is pressed.
 */
void bf::ingame_view::apply_drag_mode_pick( bool ctrl, bool alt )
{
  CLAW_PRECOND( m_drag_info->drag_mode == drag_info::drag_mode_pick );

  if ( alt )
    add_selection(m_drag_info->picked_item);
  else if ( ctrl )
    toggle_selection(m_drag_info->picked_item);
} // ingame_view::apply_drag_mode_pick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in selection mode.
 * \param ctrl Control key is pressed.
 * \param alt Alt key is pressed.
 */
void bf::ingame_view::apply_drag_mode_selection( bool ctrl, bool alt )
{
  CLAW_PRECOND( m_drag_info->drag_mode == drag_info::drag_mode_selection );

  std::list<item_instance*> item;
  pick_item
    ( item, wxRect(m_drag_info->mouse_origin, m_drag_info->mouse_position) );

  if ( ctrl || alt )
    add_selection( item );
  else
    set_selection(item);
} // ingame_view::apply_drag_mode_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in size mode.
 */
void bf::ingame_view::apply_drag_mode_size()
{
  CLAW_PRECOND( m_drag_info != NULL );
  CLAW_PRECOND( m_drag_info->drag_mode == drag_info::drag_mode_size );
  CLAW_PRECOND( m_drag_info->picked_item != NULL );

  wxRect box = get_drag_mode_size_box();

  if ( (box.x !=
        m_drag_info->picked_item->get_rendering_parameters().get_left())
       || (box.y !=
           m_drag_info->picked_item->get_rendering_parameters().get_bottom())
       || (box.width !=
           m_drag_info->picked_item->get_rendering_parameters().get_width())
       || (box.height !=
           m_drag_info->picked_item->get_rendering_parameters().get_height()) )
    do_action
      ( new action_set_item_position_and_size
        (m_drag_info->picked_item, box.x, box.y, box.width, box.height) );
} // ingame_view::apply_drag_mode_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in slope mode.
 */
void bf::ingame_view::apply_drag_mode_slope()
{
  CLAW_PRECOND( m_drag_info != NULL );
  CLAW_PRECOND( m_drag_info->drag_mode == drag_info::drag_mode_slope );
  CLAW_PRECOND( m_drag_info->picked_item != NULL );

  wxPoint gap
    ( m_drag_info->mouse_position.x - m_drag_info->mouse_origin.x,
      m_drag_info->mouse_position.y - m_drag_info->mouse_origin.y );

   double steepness;
   double left_x;
   double left_y;
   double right_x;
   double right_y;
   
   compute_slope_parameters
     (*m_drag_info->picked_item, steepness, left_x, left_y, right_x, right_y);

   if ( m_drag_info->left_side && ( gap.x != left_x || gap.y != left_y ) ) 
     do_action
       ( new action_set_slope_curve
         (m_drag_info->picked_item, gap.x, gap.y, right_x, right_y) );
   else if ( ! m_drag_info->left_side && 
             ( gap.x != right_x || gap.y != right_y ) )
     do_action
       ( new action_set_slope_curve
         (m_drag_info->picked_item, left_x, left_y, gap.x, gap.y) );
} // ingame_view::apply_drag_mode_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a coordinate.
 * \param v The value to transform.
 */
wxCoord bf::ingame_view::zoom( wxCoord v ) const
{
  return v * (wxCoord)m_zoom / 100;
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a point.
 * \param v The value to transform.
 */
wxPoint bf::ingame_view::zoom( wxPoint v ) const
{
  return wxPoint( zoom(v.x), zoom(v.y) );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a size.
 * \param v The value to transform.
 */
wxSize bf::ingame_view::zoom( wxSize v ) const
{
  return wxSize( zoom(v.x), zoom(v.y) );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed coordinate.
 * \param v The value to transform.
 */
wxCoord bf::ingame_view::unzoom( wxCoord v ) const
{
  return v * 100 / m_zoom;
} // ingame_view::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed point.
 * \param v The value to transform.
 */
wxPoint bf::ingame_view::unzoom( wxPoint v ) const
{
  return wxPoint( unzoom(v.x), unzoom(v.y) );
} // ingame_view::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed size.
 * \param v The value to transform.
 */
wxSize bf::ingame_view::unzoom( wxSize v ) const
{
  return wxSize( unzoom(v.x), unzoom(v.y) );
} // ingame_view::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an action to the level.
 * \param action The action to apply.
 */
template<typename Action>
void bf::ingame_view::apply_action( Action* action )
{
  CLAW_PRECOND( action != NULL );
  item_selection old_sel;

  if ( m_history.get_level().has_selection() )
    old_sel = m_history.get_level().get_selection();

  if ( m_history.do_action(action) )
    {
      m_parent.set_changed( true );
      m_parent.set_compile_changed( true );
      m_parent.set_layer_info();

      m_layout.get_layer_list_frame().refresh();

      if ( !m_history.get_level().empty()
           && !old_sel.same_group_than(m_history.get_level().get_selection()) )
        update_layout();
      else
        {
          m_layout.get_properties_frame().refresh();
          Refresh();
        }
    }
} // ingame_view::apply_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::ingame_view::on_size( wxSizeEvent& WXUNUSED(event) )
{
  Refresh();
} // ingame_view::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when the frame obtains the focus.
 * \param event This event occured.
 */
void bf::ingame_view::on_focused(wxFocusEvent& event)
{
  m_parent.SetFocus();
} // ingame_view::on_focused()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::ingame_view::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  render();
} // ingame_view::on_paint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the view in the current layer.
 */
wxPoint bf::ingame_view::compute_local_view_position() const
{ 
  if ( empty() )
    return wxPoint(0, 0);
  else
    return wxPoint
      ( (double)m_view.x * 
        (double)( get_active_layer().get_width() - get_view_size().x ) /
        (double)( get_level().get_width() - get_view_size().x ),
        (double)m_view.y * 
        (double)( get_active_layer().get_height() - get_view_size().y ) /
        (double)( get_level().get_height() - get_view_size().y ) );
} // ingame_view::compute_local_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the view according to a position in the
 * current layer.
 * \param view_position The position of a virtual view. 
 */
wxPoint bf::ingame_view::compute_global_view_position
(wxPoint view_position) const
{ 
  if ( empty() )
    return wxPoint(0, 0);
  else
    return wxPoint
      ( (double)view_position.x / 
        (double)( get_active_layer().get_width() - get_view_size().x ) *
        (double)( get_level().get_width() - get_view_size().x ),
        (double)view_position.y / 
        (double)( get_active_layer().get_height() - get_view_size().y ) *
        (double)( get_level().get_height() - get_view_size().y ) );
} // ingame_view::compute_global_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user start a click in the frame.
 * \param event The mouse event that occured.
 */
void bf::ingame_view::on_mouse_left_down(wxMouseEvent& event)
{
  wxPoint point = compute_mouse_position( event.GetPosition() );

  m_drag_info = new drag_info();
  m_drag_info->mouse_origin = point;
  m_drag_info->mouse_position = m_drag_info->mouse_origin;

  bool grip = false;

  if ( !event.AltDown() && !event.ControlDown() )
    {
      grip = set_drag_mode_size( point );
  
      if ( ! grip )
        grip = set_drag_mode_slope( point );
    }

  if ( !grip )
    {
      std::set<item_instance*> items;

      if ( event.AltDown() )
        select_item_at( point, items );
      else if ( !exist_selected_item(point) || event.ControlDown() )
        {
          item_instance* item = pick_first_item( point );

          if (item != NULL)
            items.insert(item);
        }
      else
	items.insert(first_selected_item(point));
      
      if ( items.empty() )
        m_drag_info->drag_mode = drag_info::drag_mode_selection;
      else
        {
          std::set<item_instance*>::iterator item;
          m_drag_info->drag_mode = drag_info::drag_mode_pick;

          if ( event.AltDown() )
            for ( item = items.begin(); item != items.end(); ++item )
              toggle_selection( *item );
          else if ( !empty() )
            for ( item = items.begin(); item != items.end(); ++item )
              if ( !get_level().item_is_selected(*item) )
                if ( event.ControlDown() )
                  toggle_selection( *item );
                else
                  set_selection(*item);
              else
                {
                  m_drag_info->picked_item = *item;
                  set_drag_mode_size( point );
                }
        }
    }
} // ingame_view::on_mouse_left_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked in the frame.
 * \param event The mouse event that occured.
 */
void bf::ingame_view::on_mouse_move(wxMouseEvent& event)
{
  wxPoint point = compute_mouse_position( event.GetPosition() );

  if ( event.LeftIsDown() )
    if ( m_drag_info != NULL )
      {
        int x(0), y(0);

        if ( event.GetX() > GetSize().x )
          x = event.GetX() - GetSize().x;
        else if ( event.GetX() < 0 )
          x = event.GetX();

        if ( event.GetY() > GetSize().y )
          y = event.GetY() - GetSize().y;
        else if ( event.GetY() < 0 )
          y = event.GetY();

        if ( (x!=0) || (y!=0) )
          set_view_position( m_view.x + x, m_view.y - y );

        if ( (( std::abs(point.x - m_drag_info->mouse_origin.x) >= 10 )
              || ( std::abs(point.y - m_drag_info->mouse_origin.y) >= 10 ))
             && (m_drag_info->drag_mode == drag_info::drag_mode_pick) )
          m_drag_info->drag_mode = drag_info::drag_mode_move;

        if ( !m_grid.get_magnetism_active() || event.ShiftDown()
             || !has_selection() )
          m_drag_info->mouse_position = point;
        else
          update_mouse_position( point );

        Refresh();
      }

  write_mouse_position( point );
} // ingame_view::on_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user click in the frame.
 * \param event The mouse event that occured.
 */
void bf::ingame_view::on_mouse_left_up(wxMouseEvent& event)
{
  wxPoint point = compute_mouse_position( event.GetPosition() );

  if ( m_drag_info == NULL )
    event.Skip();
  else
    {
      m_drag_info->mouse_position = point;

      if ( m_drag_info->drag_mode == drag_info::drag_mode_move )
        apply_drag_mode_move
          ( event.ControlDown(), event.ShiftDown(), event.AltDown() );
      else if ( m_drag_info->drag_mode == drag_info::drag_mode_pick )
        apply_drag_mode_pick( event.ControlDown(), event.AltDown() );
      else if ( m_drag_info->drag_mode == drag_info::drag_mode_selection )
        apply_drag_mode_selection( event.ControlDown(), event.AltDown() );
      else if ( m_drag_info->drag_mode == drag_info::drag_mode_size )
        apply_drag_mode_size();
      else if ( m_drag_info->drag_mode == drag_info::drag_mode_slope )
        apply_drag_mode_slope();

      delete m_drag_info;
      m_drag_info = NULL;
    }
  m_parent.update_toolbar();
} // ingame_view::on_mouse_left_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user double click in the frame.
 * \param event The mouse event that occured.
 */
void bf::ingame_view::on_mouse_left_double_click(wxMouseEvent& event)
{
  wxPoint point = compute_mouse_position( event.GetPosition() );

  item_instance* item = first_selected_item(point);

  if ( item != NULL )
    {
      if ( m_layout.get_properties_frame().has_last_edited_field_name() )
        {
          std::string field
            (m_layout.get_properties_frame().get_last_edited_field_name());

          if ( item->get_class().has_field(field) )
            m_layout.get_properties_frame().edit_field(field);
        }
    }
} // ingame_view::on_mouse_left_double_click()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user middle-click in the frame.
 * \param event The mouse event that occured.
 */
void bf::ingame_view::on_mouse_middle_up(wxMouseEvent& event)
{ 
  wxPoint point = compute_mouse_position( event.GetPosition() );

  if ( event.ControlDown() )
    {
      std::set<item_instance*> items;
      select_item_at( point, items );
      add_selection( std::list<item_instance*>(items.begin(), items.end()) );
    }
  else if ( has_selection() )
    {
      const item_instance* selection( get_level().get_main_selection() );

      const double dx
        ( point.x - selection->get_rendering_parameters().get_left() );
      const double dy
        ( point.y - selection->get_rendering_parameters().get_bottom() );

      do_action( new action_copy_selection( get_level(), dx, dy, false ) );
    }
  m_parent.update_toolbar();
} // ingame_view::on_mouse_middle_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief The middle button of mouse turn.
 * \param event The mouse event that occured.
 */
void bf::ingame_view::on_mouse_wheel_rotation(wxMouseEvent& event)
{
  int rotation = event.GetWheelRotation();
  wxPoint point = compute_mouse_position( event.GetPosition() );

  if ( rotation < 0 )
    if ( event.ShiftDown() )
      {
        set_view_position
          ( m_view.x + m_parent.get_h_scrollbar().GetPageSize() / 4 * 
            get_level().get_width() / get_layer_view_size().x, m_view.y );
      }
    else if ( event.ControlDown() )
      {
        if (get_zoom() > 5)
          {
            set_zoom( get_zoom() - 5, point );
            m_parent.update_zoom();
          }
      }
    else if ( event.AltDown() )
      rotate_selection(true);
    else
      set_view_position
        ( m_view.x, 
          m_view.y - m_parent.get_v_scrollbar().GetPageSize() / 4 * 
          get_level().get_height() / get_layer_view_size().y );
  else
    if ( event.ShiftDown() )
      set_view_position
        ( m_view.x - m_parent.get_h_scrollbar().GetPageSize() / 4 * 
          get_level().get_width() / get_layer_view_size().x, m_view.y );
    else if ( event.ControlDown() )
      {
        set_zoom( get_zoom() + 5, point );
        m_parent.update_zoom();
      }
    else if ( event.AltDown() )
      rotate_selection(false);
    else
      set_view_position
        ( m_view.x, 
          m_view.y + m_parent.get_v_scrollbar().GetPageSize() / 4 * 
          get_level().get_height() / get_layer_view_size().y );

  Refresh();
} // ingame_view::on_mouse_wheel_rotation()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user pressed a keybord key.
 * \param event The keyboard event that occured.
 */
void bf::ingame_view::on_key_down(wxKeyEvent& event)
{
  switch( event.GetKeyCode() )
    {
    case WXK_LEFT:
    case WXK_UP:
    case WXK_RIGHT:
    case WXK_DOWN:
      if ( event.ControlDown() )
        event.Skip();
      else if ( event.ShiftDown() )
        move_grid(event.GetKeyCode());
      else
        {
          move_view(event.GetKeyCode());
          wxPoint point(m_view.x + event.GetX(),
                        m_view.y + GetSize().y - event.GetY());
          write_mouse_position(point);
        }
      break;
    case  WXK_PAGEDOWN:
      if ( event.ControlDown() && event.ShiftDown()
           && get_level().has_selection()
           && (get_active_index() < get_level().layers_count() - 1) )
	{
	  do_action( new action_move_down( get_level() ));
	  set_active_index(get_active_index()+1);
	}
      else
        event.Skip();
      break;
    case  WXK_PAGEUP:
      if ( event.ControlDown() && event.ShiftDown()
           && get_level().has_selection() && (get_active_index() > 0) )
	{
	  do_action( new action_move_up( get_level() ));
	  set_active_index(get_active_index()-1);
	}
      else
        event.Skip();
      break;
    default:
      event.Skip();
    }
} // ingame_view::on_key_down()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::ingame_view, wxWindow)
  EVT_SIZE( bf::ingame_view::on_size )
  EVT_SET_FOCUS( bf::ingame_view::on_focused )
  EVT_PAINT( bf::ingame_view::on_paint )
  EVT_LEFT_UP( bf::ingame_view::on_mouse_left_up )
  EVT_LEFT_DOWN( bf::ingame_view::on_mouse_left_down )
  EVT_LEFT_DCLICK( bf::ingame_view::on_mouse_left_double_click )
  EVT_MIDDLE_UP( bf::ingame_view::on_mouse_middle_up )
  EVT_MOUSEWHEEL( bf::ingame_view::on_mouse_wheel_rotation )
  EVT_MOTION( bf::ingame_view::on_mouse_move )
  EVT_KEY_DOWN( bf::ingame_view::on_key_down )
END_EVENT_TABLE()
