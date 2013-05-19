/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This window display the active part of the level currently edited and
 *        use images to display the items.
 * \author Julien Jorge
 */
#ifndef __BF_INGAME_VIEW_FRAME_HPP__
#define __BF_INGAME_VIEW_FRAME_HPP__

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <list>

#include "bf/accelerator_table.hpp"
#include "bf/item_event.hpp"

namespace bf
{
  class error_check_level_dialog;
  class gui_level;
  class ingame_view;
  class level_overview_frame;
  class windows_layout;

  /**
   * \brief This window display the active part of the level currently edited
   *        and use images to display the items.
   * \author Julien Jorge
   */
  class ingame_view_frame:
    public wxFrame
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_SHOW_GRID_PROPERTIES,
        ID_SET_GRID_ON_SELECTION,
        ID_GRID_VISIBLE,
        ID_GRID_MAGNETISM,
        ID_ID_VISIBLE,
        ID_WIREFRAME,
        ID_RELATIONSHIP,
        ID_GRAPHISM,
        ID_CHECK_LEVEL,
        ID_COMPILE_LEVEL,
        ID_QUICK_COMPILE_LEVEL,
        ID_RUN_LEVEL,
        ID_SELECT_NONE,
        ID_LEVEL_OVERVIEW,
        ID_STATISTICS,
        ID_ALIGN_LEFT,
        ID_ALIGN_RIGHT,
        ID_ALIGN_TOP,
        ID_ALIGN_BOTTOM,
        ID_ALIGN_CENTER_X,
        ID_ALIGN_CENTER_Y,
        ID_ALIGN,
        ID_ARRANGE_X,
        ID_ARRANGE_Y,
        ID_ARRANGE,
        ID_CLONE,
        ID_OFFSET,
        ID_SELECT_LAYER,
        ID_MOVE_IN_LAYER,
        ID_MOVE_IN_LAYER_UP,
        ID_MOVE_IN_LAYER_DOWN,
	ID_MOVE_IN_OTHER_LAYER,
        ID_MOVE_SELECTION_UP,
        ID_MOVE_SELECTION_DOWN,
        ID_MOVE_SELECTION_LEFT,
        ID_MOVE_SELECTION_RIGHT,
        ID_BRIGHT_BACKGROUND,
        ID_ZOOM,
        ID_PREVIOUS_LAYER,
        ID_NEXT_LAYER,
        ID_NEW_LAYER_FROM_IMAGE,
        ID_ERROR_FRAME,
        ID_EDIT_MODE,
        ID_EDIT_ACTIVE_LAYER,
        ID_EDIT_SAME_TAG,
        ID_EDIT_ALL
      }; // enum control_id

  public:
    ingame_view_frame( windows_layout& layout, gui_level* lvl,
                       const wxString& level_file = wxEmptyString );

    const wxString& get_level_file() const;

    void adjust_scrollbars();
    void set_changed( bool b = true );
    void set_compile_changed( bool b = true );
    bool is_changed() const;
    bool is_compile_changed() const;

    void set_layer_info();

    bool save();
    bool save_as();

    bool add_item( const std::string& class_name );

    ingame_view* get_ingame_view();
    const ingame_view* get_ingame_view() const;

    wxPoint get_view_position() const;

    const wxScrollBar& get_h_scrollbar();
    const wxScrollBar& get_v_scrollbar();

    void update_zoom() const;
    void update_toolbar() const;

  private:
    void make_title();

    bool effective_save();

    void create_controls( windows_layout& layout, gui_level* lvl );
    void create_member_controls( windows_layout& layout, gui_level* lvl );
    void create_sizer_controls();
    void create_menu();
    wxMenu* create_level_menu() const;
    wxMenu* create_edit_menu(wxMenu* moving_layer_menu);
    wxMenu* create_align_menu() const;
    wxMenu* create_edit_mode_menu() const;
    wxMenu* create_view_menu() const;
    wxMenu* create_layer_menu(wxMenu* layer_menu);
    wxMenu* create_reference_item_field_menu();

    void create_toolbar();

    void create_accelerators();

    void toggle_grid_visibility();
    void toggle_grid_magnetism();
    void toggle_id_visibility();
    void toggle_relationship_visibility();
    void toggle_bright_background();
    void toggle_wireframe_mode();
    void toggle_graphic_mode();

    void show_grid_configuration();

    bool check_level();
    void check_level_verbose();

    bool compile_level
    ( unsigned int o = std::numeric_limits<unsigned int>::max() );
    bool compile_level_no_check( unsigned int o );

    std::string get_compiled_level_file_path() const;

    template<typename MenuType>
    void update_menu( MenuType& m ) const;
    void update_moving_layer_menu
    ( wxMenu* moving_layer_menu, 
      std::map<int, unsigned int>& moving_layer_index);
    
    void add_entry_reference_item_field_menu
    ( const std::string& field, const std::string& id_item, 
      bool item_to_selection );
    void add_entry_reference_item_list_field_menu
    ( const std::string& field, const std::string& id_item, 
      bool item_to_selection, bool add );
    void clear_reference_item_field_menu();
    void add_entry_item_to_selection();
    void add_entry_selection_to_item();
    void update_reference_item_field_menu();
    
    void update_layer_menu
    (wxMenu* layer_menu, std::map<int, unsigned int>& layer_index);
    std::map<double, double> arrange( const std::set<double>& s ) const;

    void on_size(wxSizeEvent& event);
    void on_paint(wxPaintEvent& event);
    void on_activate(wxActivateEvent& event);
    void on_scroll(wxScrollEvent& event);
    void on_close(wxCloseEvent& event);
    void on_context_menu(wxContextMenuEvent& event);
    void on_level_properties(wxCommandEvent& event);
    void on_level_statistics(wxCommandEvent& event);
    void on_undo(wxCommandEvent& event);
    void on_redo(wxCommandEvent& event);
    void on_cut(wxCommandEvent& event);
    void on_copy(wxCommandEvent& event);
    void on_paste(wxCommandEvent& event);
    void on_delete(wxCommandEvent& event);
    void on_select_all(wxCommandEvent& event);
    void on_select_none(wxCommandEvent& event);
    void on_check_level(wxCommandEvent& event);
    void on_save(wxCommandEvent& event);
    void on_compile_level(wxCommandEvent& event);
    void on_quick_compile_level(wxCommandEvent& event);
    void on_run_level(wxCommandEvent& event);
    void on_menu_close(wxCommandEvent& event);
    void on_show_grid_properties(wxCommandEvent& event);
    void on_set_grid_on_selection(wxCommandEvent& event);
    void on_grid_magnetism(wxCommandEvent& event);
    void on_grid_visible(wxCommandEvent& event);
    void on_id_visible(wxCommandEvent& event);
    void on_relationship(wxCommandEvent& event);
    void on_wireframe(wxCommandEvent& event);
    void on_graphic(wxCommandEvent& event);
    void on_level_overview(wxCommandEvent& event);
    void on_menu_highlight(wxMenuEvent& event);
    void on_open_menu(wxMenuEvent& event);
    void on_arrange_horizontally(wxCommandEvent& event);
    void on_arrange_vertically(wxCommandEvent& event);
    void on_align_left(wxCommandEvent& event);
    void on_align_right(wxCommandEvent& event);
    void on_align_top(wxCommandEvent& event);
    void on_align_bottom(wxCommandEvent& event);
    void on_align_center_x(wxCommandEvent& event);
    void on_align_center_y(wxCommandEvent& event);
    void on_clone(wxCommandEvent& event);
    void on_offset(wxCommandEvent& event);

    void on_move_in_layer_up(wxCommandEvent& event);
    void on_move_in_layer_down(wxCommandEvent& event);
    void on_move_in_other_layer(wxCommandEvent& event);
    void on_select_layer(wxCommandEvent& event);
    void on_set_selection_to_item( wxCommandEvent& event );
    void on_set_list_selection_to_item( wxCommandEvent& event );
    void on_add_list_selection_to_item( wxCommandEvent& event );
    void on_set_item_to_selection( wxCommandEvent& event );
    void on_set_list_item_to_selection( wxCommandEvent& event );
    void on_add_list_item_to_selection( wxCommandEvent& event );

    void on_move_selection_up(wxCommandEvent& event);
    void on_move_selection_down(wxCommandEvent& event);
    void on_move_selection_left(wxCommandEvent& event);
    void on_move_selection_right(wxCommandEvent& event);

    void on_align(wxCommandEvent& event);
    void on_arrange(wxCommandEvent& event);

    void on_bright_background(wxCommandEvent& event);
    void on_zoom_100(wxCommandEvent& event);
    void on_zoom_in(wxCommandEvent& event);
    void on_zoom_out(wxCommandEvent& event);
    void on_zoom_change( wxSpinEvent& event );

    void on_previous_layer(wxCommandEvent& event);
    void on_next_layer(wxCommandEvent& event);

    void on_new_layer_from_image(wxCommandEvent& event);

    void on_edit_mode_active_layer( wxCommandEvent& event );
    void on_edit_mode_same_tag( wxCommandEvent& event );
    void on_edit_mode_all( wxCommandEvent& event );

    void on_error_selected( item_event& event );

  private:
    /** \brief The accelerators of the frame. */
    accelerator_table m_accelerator;

    /** \brief The windows of the program. */
    windows_layout& m_layout;

    /** \brief The control that displays the items. */
    ingame_view* m_ingame_view;

    /** \brief Horizontal scrollbar to scroll the level. */
    wxScrollBar* m_h_scrollbar;

    /** \brief Vertical scrollbar to scroll the level. */
    wxScrollBar* m_v_scrollbar;

    /** \brief The path to the level file. */
    wxString m_level_file;

    /** \brief Informations about the active layer. */
    wxString m_layer_info;

    /** \brief Tell if the level has changes that are not saved. */
    bool m_changed;

    /** \brief Tell if the level has changes that are not compiled. */
    bool m_compile_changed;

    /** \brief The popup menu displayed when right-clicking the level. */
    wxMenu m_popup_menu;

    /** \brief The alignment menu, displayed both in the menu and when clicking
        on the toolbar tool. */
    wxMenu* m_align_menu;

    /** \brief The arrange menu, displayed both in the menu and when clicking
        on the toolbar tool. */
    wxMenu* m_arrange_menu;

    /** \brief Overview of the level. */
    level_overview_frame* m_overview;

    /** \brief The list of the errors in the level. */
    error_check_level_dialog* m_errors;

    /** \brief The control displaying the zoom. */
    wxSpinCtrl* m_zoom_spin;

    /** \brief A map associated wxID with moving layer index in menu bar. */
    std::map<int, unsigned int> m_moving_layer_index;

    /** \brief A map associated wxID with moving layer index in popup menu. */
    std::map<int, unsigned int> m_moving_layer_popup_index;

    /** \brief A map associated wxID with field name in popup menu. */
    std::map<int, std::string> m_reference_item_field_popup_fields;

    /** \brief The last selected item instance for reference item field. */
    item_instance* m_reference_item_field_instance;

    /** \brief The layer menu for selection movement. */
    wxMenu* m_moving_layer_menu;

    /** \brief The layer popup menu for selection movement. */
    wxMenu* m_moving_layer_popup_menu;

    /** \brief The popup menu for reference item field. */
    wxMenu* m_reference_item_field_popup_menu;

    /** \brief A map associated wxID with layer index in menu bar. */
    std::map<int, unsigned int> m_layer_index;

    /** \brief A map associated wxID with layer index in popup menu. */
    std::map<int, unsigned int> m_layer_popup_index;

    /** \brief The layer menu. */
    wxMenu* m_layer_menu;

    /** \brief The layer popup menu. */
    wxMenu* m_layer_popup_menu;

    DECLARE_EVENT_TABLE()

  }; // class ingame_view_frame
} // namespace bf

#endif // __BF_INGAME_VIEW_FRAME_HPP__
