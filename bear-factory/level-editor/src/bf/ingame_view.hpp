/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This control displays the items of the level.
 * \author Julien Jorge
 */
#ifndef __BF_INGAME_VIEW_HPP__
#define __BF_INGAME_VIEW_HPP__

#include "bf/grid.hpp"
#include "bf/level_clipboard.hpp"
#include "bf/level_check_result.hpp"
#include "bf/sprite_image_cache.hpp"
#include "bf/sprite_with_position.hpp"
#include "bf/types.hpp"
#include "bf/history/level_history.hpp"

#include <wx/wx.h>
#include <wx/dnd.h>
#include <wx/event.h>

namespace bf
{
  class action_group;
  class layer;
  class ingame_view_frame;
  class windows_layout;

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
    /** \brief Informations on a drag local to the view. */
    class drag_info
    {
    public:
      enum drag_mode_type
        {
          drag_mode_none,
          drag_mode_selection,
          drag_mode_move,
          drag_mode_size,
          drag_mode_pick,
          drag_mode_slope          
        }; // enum drag_mode

    public:
      drag_info();

      wxSize delta() const;

    public:
      /** \brief The current mode of the drag. */
      drag_mode_type drag_mode;

      /** \brief The item picked at the begining of the drag. */
      item_instance* picked_item;

      /** \brief Origin of the drag. */
      wxPoint mouse_origin;

      /** \brief Position of the mouse during the drag. */
      wxPoint mouse_position;

      /** \brief Tell if the drag conterns the X-axis. */
      bool x_active;

      /** \brief Tell if the drag conterns the Y-axis. */
      bool y_active;

      /** \brief Tell if the drag conterns the left side. */
      bool left_side;
    }; // struct drag_info

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
    ( ingame_view_frame& parent, gui_level* lvl, windows_layout& layout );
    ~ingame_view();

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

    void show_grid( bool v );
    bool get_grid_visibility() const;

    void set_bright_background( bool b );
    bool get_bright_background() const;

    void set_id_visibility( bool v );
    bool get_id_visibility() const;

    void toggle_relationship_drawing();
    bool get_relationship_drawing() const;

    void toggle_graphic_drawing();
    bool get_graphic_drawing() const;

    void toggle_wireframe_drawing();
    bool get_wireframe_drawing() const;

    const grid& get_grid() const;
    grid& get_grid();
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
    void render_layers( wxGCDC& dc ) const;

    void render_items
    ( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order,
      unsigned int i ) const;

    void render_relationship
    ( wxDC& dc, const item_instance& item,
      const std::multimap<int, item_instance*>& z_order,
      unsigned int index) const;
    void render_relationship
    ( wxDC& dc, const item_instance& ref, const item_instance& rel,
      unsigned int index) const;

    void render_item
    ( wxGCDC& dc, const item_instance& item, const wxPoint& pos,
      unsigned int index,
      const std::multimap<int, item_instance*>& z_order ) const;

    void
    render_item_sprite
    ( wxDC& dc, const item_instance& item,
      const std::multimap<int, item_instance*>& z_order,
      unsigned int index) const;
    void render_item_filled
    ( wxGCDC& gc, const item_instance& item,
      unsigned int index ) const;
    void render_item_wireframe
    ( wxGCDC& dc, const item_instance& item, unsigned int index ) const;
    void render_item_id
    ( wxDC& dc, const item_instance& item, unsigned int layer ) const;

    void render_item_as_sprite
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      const std::multimap<int, item_instance*>& z_order) const;
    void render_item_as_wireframe
    ( wxGCDC& dc, const item_instance& item, const wxPoint& pos,
      unsigned int index ) const;
    void render_item_id
    ( wxDC& dc, const item_instance& item, const wxPoint& pos ) const;

    void render_item_as_point
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      unsigned int index ) const;
    bool render_sprite
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      const std::multimap<int, item_instance*>& z_order ) const;
    void render_item_bounding_box
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      const wxSize& size, unsigned int index ) const;
    void render_slope_steepness
    ( wxGCDC& dc, const item_instance& item, 
      const wxPoint& pos, const wxSize& size, unsigned int index ) const;
    void render_slope_curve_grip
    ( wxDC& dc, const item_instance& item, 
      const wxPoint& pos, const wxSize& size, unsigned int index ) const;
    void compute_slope_parameters
    ( const item_instance& item, double & steepness, double & left_x,
      double & left_y, double & right_x, double & right_y ) const;
    void render_non_valid_item
    ( wxDC& dc, const item_instance& item, unsigned int index ) const;
    void render_non_valid_item_as_point
    ( wxDC& dc, const item_instance& item, const wxPoint& pos ) const;
    void render_non_valid_item_box
    ( wxDC& dc, const item_instance& item,
      const wxPoint& pos, const wxSize& size ) const;

    void render_grip( wxDC& dc, unsigned int index ) const;
    void render_grip_box( wxDC& dc, wxCoord left, wxCoord top ) const;

    void render_drag
    ( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order, 
      unsigned int index ) const;
    void render_drag_mode_selection( wxDC& dc, unsigned int index ) const;
    void render_drag_mode_move
    ( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order, 
      unsigned int index ) const;
    void render_drag_mode_size( wxDC& dc, unsigned int index ) const;
    void render_drag_mode_slope( wxGCDC& dc, unsigned int index ) const;
    void render_grid( wxDC& dc ) const;
    void render_grid_vertical_lines( wxDC& dc ) const;
    void render_grid_horizontal_lines( wxDC& dc ) const;

    std::pair<wxBitmap, wxPoint>
    get_item_visual( const item_instance& item ) const;
    std::pair<wxBitmap, wxPoint> 
    get_item_visual
    ( const item_instance& item, 
      const bitmap_rendering_attributes& rendering_attributes ) const;
    bool has_visual
    ( const item_instance& item,
      const std::multimap<int, item_instance*>& z_order ) const;
    std::pair<wxBitmap, wxPoint>
    get_item_or_mimic_visual
    ( const item_instance& item,
      const std::multimap<int, item_instance*>& z_order ) const;
    wxPen
    get_display_pen( const item_instance& item, unsigned int index ) const;

    const layer& current_layer();

    void select_item_at
    ( const wxPoint& pos, std::set<bf::item_instance*>& items );

    void toggle_selection( item_instance* item );
    void add_selection( item_instance* item );
    void add_selection( const std::vector<item_instance*>& item);
    void set_selection( const std::vector<item_instance*>& item);
    void set_selection
    ( const std::vector<item_instance*>& item, item_instance* selected,
      bool add = false);
    void set_selection( item_instance* item );

    wxRect get_presence_box( const item_instance& item ) const;

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

    wxCoord zoom( wxCoord v ) const;
    wxPoint zoom( wxPoint v ) const;
    wxSize zoom( wxSize v ) const;
    wxCoord unzoom( wxCoord v ) const;
    wxPoint unzoom( wxPoint v ) const;
    wxSize unzoom( wxSize v ) const;

    template<typename Action>
    void apply_action( Action* action );

    void on_size(wxSizeEvent& event);
    void on_focused(wxFocusEvent& event);
    void on_paint(wxPaintEvent& event);
    wxPoint compute_local_view_position() const;
    wxPoint compute_global_view_position(wxPoint view_position) const;
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

    /** \brief The grid. */
    grid m_grid;

    /** \brief Tell if we draw the bounding box of the items. */
    bool m_wireframe_drawing;

    /** \brief Tell if we display the pictures of the items. */
    bool m_graphic_drawing;

    /** \brief Tell if we display the grid. */
    bool m_display_grid;

    /** \brief Tell if we display the id. */
    bool m_display_id;

    /** \brief Tell if we display the relationship among the items. */
    bool m_display_relationship;

    /** \brief Tell if we use a bright background. */
    bool m_bright_background;

    /** \brief A cache of the sprites of the level. */
    sprite_image_cache* m_image_cache;

    /** \brief The current level of zoom. */
    unsigned int m_zoom;

    /** \brief The result of the last check on the level. */
    level_check_result m_check_result;

    /** \brief The size of grip in the corner of the selected item. */
    static const wxCoord s_grip_size;

    /** \brief The items copied in the clipboard. */
    static level_clipboard s_clipboard;

    DECLARE_EVENT_TABLE()

  }; // class ingame_view
} // namespace bf

#endif // __BF_INGAME_VIEW_HPP__
