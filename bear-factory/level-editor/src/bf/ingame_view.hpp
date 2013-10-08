/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays the items of the level.
 * \author Julien Jorge
 */
#ifndef __BF_INGAME_VIEW_HPP__
#define __BF_INGAME_VIEW_HPP__

#include "bf/edit_mode.hpp"
#include "bf/grid.hpp"
#include "bf/level_clipboard.hpp"
#include "bf/level_check_result.hpp"
#include "bf/level_renderer.hpp"
#include "bf/selection_manager.hpp"
#include "bf/history/level_history.hpp"

#include <wx/wx.h>
#include <wx/dnd.h>
#include <wx/event.h>

namespace bf
{
  class action_group;
  class drag_info;
  class layer;
  class ingame_view_frame;
  class windows_layout;
  class workspace_environment;

  /**
   * \brief This control display the items of the level.
   * \author Julien Jorge
   */
  class ingame_view:
    public wxWindow
  {
  public:
    typedef wxWindow super;

  private:
    /** \brief A drop target for creating an item. The class of the item is
        passed as a simple text. */
    class item_drop_target:
      public wxTextDropTarget
    {
    public:
      item_drop_target( ingame_view& view );

      bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

    private:
      /** \brief The view associated with this target. */
      ingame_view& m_view;

    }; // class item_drop_target

  public:
    ingame_view
    ( ingame_view_frame& parent, gui_level* lvl, windows_layout& layout,
      workspace_environment* env );

    bool empty() const;

    const wxPoint& get_view_position() const;
    wxPoint get_center_in_level() const;
    wxSize get_layer_view_size() const;
    wxPoint get_position_in_layer
    (wxPoint position, unsigned int index, wxCoord height) const;
    wxSize get_view_size() const;
    void set_view_position( wxCoord x, wxCoord y );

    unsigned int get_zoom() const;
    void set_zoom(unsigned int z);
    void set_zoom(unsigned int z, wxPoint mouse_position);

    unsigned int get_active_index() const;
    void set_active_index( unsigned int index );

    layer& get_active_layer() const;
    const gui_level& get_level() const;
    gui_level& get_level();

    edit_mode get_edit_mode() const;
    void set_edit_mode( edit_mode m );

    item_selection get_edit_selection() const;
    std::vector<std::size_t> get_edit_layers() const;

    void show_grid( bool v );
    bool get_grid_visibility() const;

    void set_bright_background( bool b );
    bool get_bright_background() const;

    void toggle_continuity_hints();
    bool get_continuity_hints() const;

    void set_id_visibility( bool v );
    bool get_id_visibility() const;

    void toggle_relationship_drawing();
    bool get_relationship_drawing() const;

    void toggle_graphic_drawing();
    bool get_graphic_drawing() const;

    void toggle_wireframe_drawing();
    bool get_wireframe_drawing() const;

    const grid& get_grid() const;
    void set_grid( const grid& g );
    void set_grid_on_selection();

    void do_action( level_action* action );
    void do_action( action_move_selection* action );
    void do_action( action_rotate_selection* action );

    bool add_item( const std::string& class_name, wxCoord x, wxCoord y );
    bool add_item( const std::string& class_name );

    bool check_level();
    const level_check_result& get_check_result() const;
    void save( std::ostream& os );
    void compile(compiled_file& f, unsigned int o);

    void undo();
    void redo();

    const level_history& get_history() const;

    void update_layout();

    bool has_selection() const;
    void clear_selection();
    void select_all();
    void select_item_and_layer( item_instance* item );

    level_clipboard& get_clipboard() const;

    void copy_to_clipboard() const;
    void cut_to_clipboard();
    void paste_from_clipboard();

    void get_structure_sprites( std::list<sprite_with_position>& s ) const;
    void render();

    void show_item_position_error(wxWindow* win);

    wxPoint compute_mouse_position(const wxPoint& point) const;

  private:
    void select_item_at
    ( const wxPoint& pos, std::set<bf::item_instance*>& items );

    void toggle_selection( item_instance* item );
    void add_selection( item_instance* item );
    void add_selection( const std::vector<item_instance*>& item);
    void set_selection( const std::vector<item_instance*>& item);
    void set_selection( item_instance* item );

    void copy_selection(bool add = false);
    void move_selection();
    void rotate_selection( bool clockwise );

    int update_coordinate_magnetism
    ( unsigned int item_position, unsigned int size_item,
      unsigned int offset_grid, unsigned int step_grid);
    void update_mouse_position( const wxPoint& mouse_position );

    void move_grid(int keycode);
    void move_view(int keycode);

    void write_mouse_position(const wxPoint& point);

    void set_tooltip( item_instance* item );

    bool set_drag_mode_size( const wxPoint& pos );
    bool set_drag_mode_slope( const wxPoint& pos );
    wxRect get_drag_mode_size_box() const;

    void apply_drag_mode_move( bool ctrl, bool shift, bool alt );
    void apply_drag_mode_pick( bool ctrl, bool alt );
    void apply_drag_mode_selection( bool ctrl, bool alt );
    void apply_drag_mode_size();
    void apply_drag_mode_slope();

    template<typename Action>
    void apply_action( Action* action );

    double zoom( double v ) const;
    unsigned int zoom( unsigned int v ) const;
    wxCoord zoom( wxCoord v ) const;
    wxPoint zoom( wxPoint v ) const;
    wxSize zoom( wxSize v ) const;
    wxCoord unzoom( wxCoord v ) const;
    wxPoint unzoom( wxPoint v ) const;
    wxSize unzoom( wxSize v ) const;

    wxPoint compute_local_view_position() const;
    wxPoint compute_global_view_position( wxPoint view_position ) const;

    void on_size(wxSizeEvent& event);
    void on_focused(wxFocusEvent& event);
    void on_paint(wxPaintEvent& event);
    void on_mouse_left_down(wxMouseEvent& event);
    void on_mouse_move(wxMouseEvent& event);
    void on_mouse_left_up(wxMouseEvent& event);
    void on_mouse_left_double_click(wxMouseEvent& event);
    void on_mouse_middle_up(wxMouseEvent& event);
    void on_mouse_wheel_rotation(wxMouseEvent& event);
    void on_key_down(wxKeyEvent& event);

  private:
    /** \brief The frame containing the view. */
    ingame_view_frame& m_parent;

    /** \brief The windows of the interface. */
    windows_layout& m_layout;

    /** \brief The level and the undo/redo informations. */
    level_history m_history;

    /** \brief Position of the view in the (zoomed) level. */
    wxPoint m_view;

    /** \brief Info about the current dragging. */
    drag_info* m_drag_info;

    /** \brief The result of the last check on the level. */
    level_check_result m_check_result;

    /** \brief This object will draw the level in the view. */
    level_renderer m_renderer;

    /** \brief This object manages how the items are selected in the level. */
    selection_manager m_selection_manager;

    /** \brief The workspace environment. */
    workspace_environment* m_workspace;

    /** \brief The items copied in the clipboard. */
    static level_clipboard s_clipboard;

    DECLARE_EVENT_TABLE()

  }; // class ingame_view
} // namespace bf

#endif // __BF_INGAME_VIEW_HPP__
