/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::ingame_view class.
 * \author Julien Jorge
 */
#include "bf/ingame_view.hpp"

#include "bf/compilation_context.hpp"
#include "bf/drag_info.hpp"
#include "bf/gui_level.hpp"
#include "bf/ingame_view_frame.hpp"
#include "bf/item_class_pool.hpp"
#include "bf/item_choice_frame.hpp"
#include "bf/properties_frame.hpp"
#include "bf/layer_list_frame.hpp"
#include "bf/level_file_xml_writer.hpp"
#include "bf/main_frame.hpp"
#include "bf/slope.hpp"
#include "bf/windows_layout.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/wx_type_cast.hpp"

#include "bf/history/action_add_item.hpp"
#include "bf/history/action_copy_selection.hpp"
#include "bf/history/action_delete_selection.hpp"
#include "bf/history/action_move_selection.hpp"
#include "bf/history/action_paste_items.hpp"
#include "bf/history/action_set_item_position_and_size.hpp"
#include "bf/history/action_set_slope_curve.hpp"
#include "bf/history/action_rotate_selection.hpp"

#include <wx/dcbuffer.h>
#include <wx/defs.h>

/*----------------------------------------------------------------------------*/
bf::level_clipboard bf::ingame_view::s_clipboard;




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
 * \param env The workspace environment used.
 *
 * The level will be deleted in the destructor.
 */
bf::ingame_view::ingame_view
( ingame_view_frame& parent, gui_level* lvl, windows_layout& layout, 
  workspace_environment* env )
  : super( &parent, wxID_ANY ), m_parent(parent), m_layout(layout),
    m_history(lvl), m_view(0, 0), m_drag_info(NULL), m_renderer( *lvl, env ),
    m_selection_manager( *lvl, layout.get_properties_frame() ),
    m_workspace(env)
{
  CLAW_PRECOND(lvl != NULL);
  SetDropTarget( new item_drop_target(*this) );
} // ingame_view::ingame_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level contains a layer.
 */
bool bf::ingame_view::empty() const
{
  return get_level().empty();
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
 * \brief Get the size of the visible part of the level.
 */
wxSize bf::ingame_view::get_view_size() const
{
  //return m_renderer.get_view_size();
  return 
    wxSize( m_renderer.unzoom(GetSize().x), m_renderer.unzoom(GetSize().y) );
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
( wxPoint position, unsigned int index, wxCoord height ) const
{
  return m_renderer.get_position_in_layer( position, index, height );
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
    }

  m_parent.adjust_scrollbars();
} // ingame_view::set_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the zoom.
 */
unsigned int bf::ingame_view::get_zoom() const
{
  return m_renderer.get_zoom();
} // ingame_view::get_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the zoom.
 * \param z The new value of the zoom.
 */
void bf::ingame_view::set_zoom(unsigned int z)
{
  wxPoint c = get_center_in_level();
  m_renderer.set_zoom(z);

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

  m_renderer.set_zoom( z );
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
  return get_level().get_active_layer_index();
} // ingame_view::get_active_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the active layer.
 */
void bf::ingame_view::set_active_index( unsigned int index )
{
  CLAW_PRECOND( index < get_level().layers_count() );

  get_level().set_active_layer(index);

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
  return get_level().get_active_layer();
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
 * \brief Tells to which items the operations are applied.
 */
bf::edit_mode bf::ingame_view::get_edit_mode() const
{
  return m_selection_manager.get_edit_mode();
} // ingame_view::get_edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to which items the operations are applied.
 * \param m The new edit mode.
 */
void bf::ingame_view::set_edit_mode( edit_mode m )
{
  m_selection_manager.set_edit_mode( m );
} // ingame_view::set_edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the selection to which the operations must apply.
 */
bf::item_selection bf::ingame_view::get_edit_selection() const
{
  return m_selection_manager.get_edit_mode().get_selection( get_level() );
} // ingame_view::get_edit_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the indices of the layers to which the operations must apply.
 */
std::vector<std::size_t> bf::ingame_view::get_edit_layers() const
{
  return m_selection_manager.get_edit_mode().get_edit_layers( get_level() );
} // ingame_view::get_edit_layers()

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
  m_renderer.show_grid( v );
} // ingame_view::show_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the grid is visible or not.
 */
bool bf::ingame_view::get_grid_visibility() const
{
  return m_renderer.get_grid_visibility();
} // ingame_view::get_grid_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Use a bright bacground.
 * \param v Use it or not.
 */
void bf::ingame_view::set_bright_background( bool v )
{
  m_renderer.set_bright_background( v );
} // ingame_view::set_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we use a bright bacground.
 */
bool bf::ingame_view::get_bright_background() const
{
  return m_renderer.get_bright_background();
} // ingame_view::get_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggles the visibility of the continuity hints.
 */
void bf::ingame_view::toggle_continuity_hints()
{
  m_renderer.toggle_continuity_hints();
} // ingame_view::toggle_continuity_hints()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the continuity hints are displayed..
 */
bool bf::ingame_view::get_continuity_hints() const
{
  return m_renderer.get_continuity_hints();
} // ingame_view::get_continuity_hints()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show/hide the id.
 * \param v Tell is the id is visible.
 */
void bf::ingame_view::set_id_visibility( bool v )
{
  m_renderer.set_id_visibility( v );
} // ingame_view::set_id_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the id is visible or not.
 */
bool bf::ingame_view::get_id_visibility() const
{
  return m_renderer.get_id_visibility();
} // ingame_view::get_id_visibity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the relationship drawings.
 */
void bf::ingame_view::toggle_relationship_drawing()
{
  m_renderer.toggle_relationship_drawing();
  Refresh();
} // ingame_view::toggle_relationship_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the drawing of the relationships is enabled.
 */
bool bf::ingame_view::get_relationship_drawing() const
{
  return m_renderer.get_relationship_drawing();
} // ingame_view::get_relationship_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the graphic drawing.
 */
void bf::ingame_view::toggle_graphic_drawing()
{
  m_renderer.toggle_graphic_drawing();
  Refresh();
} // ingame_view::toggle_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the graphic drawing is enabled.
 */
bool bf::ingame_view::get_graphic_drawing() const
{
  return m_renderer.get_graphic_drawing();
} // ingame_view::get_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the wireframe drawing.
 */
void bf::ingame_view::toggle_wireframe_drawing()
{
  m_renderer.toggle_wireframe_drawing();
  Refresh();
} // ingame_view::toggle_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the wireframe drawing is enabled.
 */
bool bf::ingame_view::get_wireframe_drawing() const
{
  return m_renderer.get_wireframe_drawing();
} // ingame_view::get_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the grid.
 */
const bf::grid& bf::ingame_view::get_grid() const
{
  return m_renderer.get_grid();
} // ingame_view::get_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the grid.
 * \param g The new grid.
 */
void bf::ingame_view::set_grid( const grid& g )
{
  m_renderer.set_grid( g );
} // ingame_view::set_grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the grid according to the current selection.
 */
void bf::ingame_view::set_grid_on_selection()
{
  // TODO move the logic into the grid class and let ingame_view_frame do the
  // work.

  if ( has_selection() )
    {
      const item_instance* selection
        ( get_edit_selection().get_main_selection() );
      grid g( get_grid() );

      wxSize step( g.get_step() );

      if ( selection->get_rendering_parameters().get_width() != 0 )
        step.x = (int)selection->get_rendering_parameters().get_width();
      if ( selection->get_rendering_parameters().get_height() != 0 )
        step.y = (int)selection->get_rendering_parameters().get_height();

      g.set_step(step);

      wxSize offset( g.get_offset() );

      offset.x =
        (unsigned int)selection->get_rendering_parameters().get_left() % step.x;
      offset.y =
        (unsigned int)
        selection->get_rendering_parameters().get_bottom() % step.y;

      g.set_offset( offset );

      set_grid( g );

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
    if ( m_workspace->get_item_class_pool().has_item_class(class_name) )
      {
        item_instance* item =
          new item_instance
          ( m_workspace->get_item_class_pool().get_item_class_ptr(class_name) );
         
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
  lvl.save( os, get_level(), m_workspace );
  m_history.set_saved();
} // ingame_view::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the current level.
 * \param f The file in which we compile.
 * \param o The level of optimization during the compilation.
 */
void bf::ingame_view::compile( compiled_file& f, unsigned int o )
{
  compilation_context context(o, m_workspace);
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

  m_selection_manager.refresh_properties();

  Refresh();
} // ingame_view::update_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an item selected.
 */
bool bf::ingame_view::has_selection() const
{
  return m_selection_manager.has_selection();
} // ingame_view::has_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selection to nothing.
 */
void bf::ingame_view::clear_selection()
{
  m_selection_manager.clear_selection();
  Refresh();
} // ingame_view::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select all items.
 */
void bf::ingame_view::select_all()
{
  m_selection_manager.select_all();
  Refresh();
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
  m_selection_manager.set_selection( item );

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
  const item_selection selection( get_edit_selection() );

  if ( selection.empty() )
    return;

  const item_instance& main_selection( *selection.get_main_selection() );

  item_copy copy;
  copy.x = main_selection.get_rendering_parameters().get_left();
  copy.y = main_selection.get_rendering_parameters().get_bottom();

  for ( item_selection::const_iterator it( selection.begin() );
        it != selection.end(); ++it )
    copy.items.push_front(**it);

  s_clipboard.copy_items( copy );
} // ingame_view::copy_to_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the current selection in the clipboard then remove the selected
 *        items.
 */
void bf::ingame_view::cut_to_clipboard()
{
  copy_to_clipboard();

  do_action
    ( new action_delete_selection( get_level(), get_edit_layers() ) );
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
} // ingame_view::get_structure_sprites()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render all items of the level.
 */
void bf::ingame_view::render()
{
  wxBufferedPaintDC this_dc( this );

  wxGraphicsContext* gc
    ( wxGraphicsRenderer::GetDefaultRenderer()->CreateContext( this_dc ) );
  
  wxGCDC dc;
  dc.SetGraphicsContext( gc );

  if( IsShown() )
    m_renderer.render
      ( dc, wxRect( m_view, GetSize() ), m_check_result, m_drag_info );
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
 * \brief Pick items found near a given point.
 * \param pos The position of the point.
 * \param result The selected items.
 */
void bf::ingame_view::select_item_at
( const wxPoint& pos, std::set<item_instance*>& result )
{
  result.clear();

  const std::vector<item_instance*> items
    ( get_edit_mode().find_items_at
      ( get_level(), position_type( pos.x, pos.y ) ) );

  if ( !items.empty() )
    {
      const std::set<item_instance*> candidates( items.begin(), items.end() );

      item_choice_frame dlg( this, get_edit_selection(), candidates );

      if ( dlg.ShowModal() == wxID_OK )
        dlg.get_selection(result);
    }
} // ingame_view::select_item_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Changes the selection on an item.
 * \param item The item to process.
 */
void bf::ingame_view::toggle_selection( item_instance* item )
{
  m_selection_manager.toggle_selection( item );
  Refresh();
} // ingame_view::toggle_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an item in the selection.
 * \param item The item to select.
 */
void bf::ingame_view::add_selection( item_instance* item )
{
  m_selection_manager.add_selection( item );
  Refresh();
} // ingame_view::add_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds some items in the selection.
 * \param items The items to select.
 */
void bf::ingame_view::add_selection
( const std::vector<item_instance*>& items )
{
  m_selection_manager.add_selection( items );
  Refresh();
} // ingame_view::add_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the selected items.
 * \param item The items to select.
 */
void bf::ingame_view::set_selection
( const std::vector<item_instance*>& item )
{
  m_selection_manager.set_selection( item );
  Refresh();
} // ingame_view::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the selected item.
 * \param item The item to select.
 */
void bf::ingame_view::set_selection( item_instance* item )
{
  m_selection_manager.set_selection( item );
  Refresh();
} // ingame_view::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the selected items at the position where the mouse button was
 *        released.
 * \param add Indicates if the items are added at the selection.
 */
void bf::ingame_view::copy_selection(bool add)
{
  do_action
    ( action_copy_selection::create_for_layers
      ( get_level(), get_edit_layers(), m_drag_info->delta().x,
        m_drag_info->delta().y, add ) );

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
      ( get_edit_selection(),
        m_drag_info->delta().x, m_drag_info->delta().y ) );
} // ingame_view::move_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Rotate the selected items.
 * \param clockwise Indicates if the rotation the clockwise.
 */
void bf::ingame_view::rotate_selection( bool clockwise )
{
  do_action
    ( new action_rotate_selection( get_edit_selection(), clockwise ) );
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

  if ( gap_positive <= get_grid().get_magnetism_force() )
    {
      if ( gap_positive <= gap_negative )
        result = -gap_positive;
    }
  else if ( gap_negative <= get_grid().get_magnetism_force() )
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
  if ( !get_level().has_selection( get_active_index() ) )
    return;

  const item_instance* main_selection
    ( get_level().get_main_selection( get_active_index() ) );

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
      get_grid().get_offset().x, get_grid().get_step().x );
  pos.y +=
    update_coordinate_magnetism
    ( item_position.y,
      (unsigned int)main_selection->get_rendering_parameters().get_height(),
      get_grid().get_offset().y, get_grid().get_step().y );

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
  grid g( get_grid() );
  wxSize new_offset = g.get_offset();

  switch( keycode )
    {
    case WXK_LEFT:
      if ( new_offset.x == 0 )
        new_offset.x = g.get_step().x - 1;
      else
        --new_offset.x;
      break;
    case WXK_DOWN:
      if ( new_offset.y == 0 )
        new_offset.y = g.get_step().y - 1;
      else
        --new_offset.y;
      break;
    case WXK_RIGHT:
      new_offset.x = ( new_offset.x + 1 ) % g.get_step().x;
      break;
    case WXK_UP:
      new_offset.y = ( new_offset.y + 1 ) % g.get_step().y;
      break;
    }

  g.set_offset(new_offset);
  set_grid( g );

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

      item_instance* const item
        ( get_edit_mode().first_item( get_level(), wx_to_position( point ) ) );
      wxString class_str, id_str, dist_str;

      if ( item != NULL )
        {
          set_tooltip(item);
          class_str = std_to_wx_string(item->get_class().get_class_name());

          if ( !item->get_id().empty() )
            id_str =
              wxString::Format
              ( _("id = '%s'"), std_to_wx_string(item->get_id()).c_str() );

          const item_selection selection( get_edit_selection() );

          if ( !selection.empty() )
            {
              const item_rendering_parameters& r1
                ( selection.get_main_selection()->
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
  if ( !has_selection() )
    return false;

  bool grip = true;

  item_instance* const selection( get_edit_selection().get_main_selection() );
  const wxRect box
    ( rectangle_to_wx( get_level().get_visual_box( *selection ) ) );
  const wxSize s( m_renderer.get_grip_size(), m_renderer.get_grip_size() );
  wxPoint mouse_pos;

  const wxRect top_left( box.GetTopLeft() - s, s );
  const wxRect top_right( box.GetTopRight() - wxSize(0, s.y), s );
  const wxRect bottom_left( box.GetBottomLeft() - wxSize(s.x, 0), s );
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
      const wxCoord grip_size( m_renderer.get_grip_size() );
      const wxCoord h
        ((wxCoord)selection->get_rendering_parameters().get_height() / 2);
      const wxCoord w
        ((wxCoord)selection->get_rendering_parameters().get_width() / 2);
      const wxRect middle_left
        ( box.GetLeft() - grip_size,
          box.GetTop() + h - grip_size / 2, grip_size, grip_size );
      const wxRect middle_right
        ( box.GetRight(), box.GetTop() + h - grip_size / 2,
          grip_size, grip_size );
      const wxRect middle_bottom
        ( box.GetLeft() + w - grip_size / 2, box.GetTop() - grip_size,
          grip_size, grip_size );
      const wxRect middle_top
        ( box.GetLeft() + w - grip_size / 2, box.GetBottom(),
          grip_size, grip_size );

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

  if( has_selection() )
    {
      item_instance* item( get_edit_selection().get_main_selection() );

      if ( item->get_class().get_class_name() == "bear::slope" )
        {
          const wxRect box
            ( rectangle_to_wx( get_level().get_visual_box( *item ) ) );

          slope s;
          s.read_from( *item );

          wxPoint p[2];
          
          if ( s.steepness < 0 )
            {
              p[0] = wxPoint( box.GetLeft(),  box.GetBottom() );
              p[1] = wxPoint( box.GetRight(), box.GetBottom() + s.steepness );
            }
          else
            {
              p[0] = wxPoint( box.GetLeft(),  box.GetBottom() - s.steepness);
              p[1] = wxPoint( box.GetRight(), box.GetBottom() );
            }
          
          const wxCoord grip_size( m_renderer.get_grip_size() );
          const wxRect left_rect
            ( p[0].x - grip_size / 2 + s.left_handle.x,
              p[0].y - grip_size / 2 + s.left_handle.y,
              grip_size, grip_size );

          const wxRect right_rect
             ( p[1].x - grip_size / 2 + s.right_handle.x,
               p[1].y - grip_size / 2 + s.right_handle.y,
              grip_size, grip_size );
          
          wxPoint mouse_pos;
          
          if ( left_rect.Contains(pos) )
            {
              m_drag_info->mouse_origin = wxPoint( p[0].x, p[0].y );
              mouse_pos =
                wxPoint( p[0].x + s.left_handle.x, p[0].y + s.left_handle.y);
              m_drag_info->left_side = true;
            }
          else if ( right_rect.Contains(pos) )
            {
              m_drag_info->mouse_origin = wxPoint( p[1].x, p[1].y );
              mouse_pos =
                wxPoint( p[1].x + s.left_handle.x, p[1].y + s.left_handle.y );
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

  if ( get_grid().get_magnetism_active() && !shift && (has_selection()) )
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

  const std::vector<item_instance*> items
    ( get_edit_mode().pick_items
      ( get_level(),
        rectangle_type
        ( m_drag_info->mouse_origin.x, m_drag_info->mouse_origin.y,
          m_drag_info->mouse_position.x, m_drag_info->mouse_position.y ) ) );

  if ( ctrl || alt )
    add_selection( items );
  else
    set_selection( items );
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

  slope s;
  s.read_from( *m_drag_info->picked_item );
  s.scale( zoom( 1.0 ) );

  if ( m_drag_info->left_side )
    {
      if ( ( gap.x != s.left_handle.x ) || ( gap.y != s.left_handle.y ) )
        do_action
          ( new action_set_slope_curve
            ( m_drag_info->picked_item,
              gap.x, gap.y, s.right_handle.x, s.right_handle.y ) );
    }
  else if ( ( gap.x != s.right_handle.x ) || ( gap.y != s.right_handle.y ) )
    do_action
      ( new action_set_slope_curve
        ( m_drag_info->picked_item,
          s.left_handle.x, s.left_handle.y, gap.x, gap.y ) );
} // ingame_view::apply_drag_mode_slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the zoomed equivalent value of a value.
 * \param v The value to transform.
 */
double bf::ingame_view::zoom( double v ) const
{
  return m_renderer.zoom( v );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the zoomed equivalent value of a value.
 * \param v The value to transform.
 */
unsigned int bf::ingame_view::zoom( unsigned int v ) const
{
  return m_renderer.zoom( v );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a coordinate.
 * \param v The value to transform.
 */
wxCoord bf::ingame_view::zoom( wxCoord v ) const
{
  return m_renderer.zoom( v );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a point.
 * \param v The value to transform.
 */
wxPoint bf::ingame_view::zoom( wxPoint v ) const
{
  return m_renderer.zoom( v );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoomed value of a size.
 * \param v The value to transform.
 */
wxSize bf::ingame_view::zoom( wxSize v ) const
{
  return m_renderer.zoom( v );
} // ingame_view::zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed coordinate.
 * \param v The value to transform.
 */
wxCoord bf::ingame_view::unzoom( wxCoord v ) const
{
  return m_renderer.unzoom( v );
} // ingame_view::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed point.
 * \param v The value to transform.
 */
wxPoint bf::ingame_view::unzoom( wxPoint v ) const
{
  return m_renderer.unzoom( v );
} // ingame_view::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the original value of a zoomed size.
 * \param v The value to transform.
 */
wxSize bf::ingame_view::unzoom( wxSize v ) const
{
  return m_renderer.unzoom( v );
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

  const item_selection old_sel( get_edit_selection() );

  if ( m_history.do_action(action) )
    {
      m_parent.set_changed( true );
      m_parent.set_compile_changed( true );
      m_parent.set_layer_info();

      m_layout.get_layer_list_frame().refresh();

      if ( !get_level().empty()
           && !old_sel.same_group_than( get_edit_selection() ) )
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
 * \brief Compute the position of the view in the current layer.
 */
wxPoint bf::ingame_view::compute_local_view_position() const
{ 
  return m_renderer.compute_local_view_position();
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
  return m_renderer.compute_global_view_position( view_position );
} // ingame_view::compute_global_view_position()

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
  
      if ( ! grip || event.ShiftDown() )
        grip = set_drag_mode_slope( point );
    }

  if ( !grip )
    {
      std::set<item_instance*> items;

      if ( event.AltDown() )
        select_item_at( point, items );
      else
        {
          item_instance* const first_selected_item =
            get_edit_mode().first_selected_item
            ( get_level(), wx_to_position( point ) );

          if ( (first_selected_item == NULL) || event.ControlDown() )
            {
              item_instance* const item
                ( get_edit_mode().first_item
                  ( get_level(), wx_to_position( point ) ) );

              if (item != NULL)
                items.insert(item);
            }
          else
            items.insert( first_selected_item );
        }
      
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

        if ( !get_grid().get_magnetism_active() || event.ShiftDown()
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

  item_instance* const item =
    get_edit_mode().first_selected_item
    ( get_level(), position_type( point.x, point.y ) );

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
      add_selection( std::vector<item_instance*>(items.begin(), items.end()) );
    }
  else if ( get_level().has_selection( get_active_index() ) )
    {
      const item_instance* selection
        ( get_level().get_main_selection( get_active_index() ) );

      const double dx
        ( point.x - selection->get_rendering_parameters().get_left() );
      const double dy
        ( point.y - selection->get_rendering_parameters().get_bottom() );

      do_action
        ( action_copy_selection::create_for_layers
          ( get_level(), get_edit_layers(), dx, dy, false ) );
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
  if ( empty() )
    return;

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
            m_parent.adjust_scrollbars();
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
        m_parent.adjust_scrollbars();
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
