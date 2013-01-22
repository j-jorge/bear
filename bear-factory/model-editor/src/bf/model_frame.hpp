/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This window display the active action of the model currently edited.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_FRAME_HPP__
#define __BF_MODEL_FRAME_HPP__

#include <wx/frame.h>
#include <wx/combobox.h>
#include <wx/menu.h>
#include <wx/spinctrl.h>

#include "bf/history/model_history.hpp"
#include "bf/slider_with_ticks.hpp"

namespace bf
{
  class gui_model;
  class model_action;
  class snapshot;
  class model_view_ctrl;
  class windows_layout;
  class mark_event;
  class placement_event;
  class bounding_box_event;

  /**
   * \brief This window display the active action of the model currently edited.
   * \author Julien Jorge
   */
  class model_frame:
    public wxFrame
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_CHECK_MODEL,
        ID_COMPILE_MODEL,
        ID_SLIDER,
        ID_NEW_SNAPSHOT,
        ID_DELETE_SNAPSHOT,
        ID_COPY_SNAPSHOT,
        ID_PASTE_SNAPSHOT,
        ID_PLAY_STOP,
        ID_START,
        ID_TIMER,
        ID_ZOOM,
        ID_MODEL_VIEW,
        ID_WIREFRAME,
        ID_GRAPHISM,
        ID_TEXT
      }; // enum control_id

  public:
    model_frame( windows_layout& layout, gui_model* mdl = NULL,
                 const wxString& model_file = wxEmptyString );

    const wxString& get_model_file() const;

    void set_changed( bool b = true );
    bool is_changed() const;
    bool empty() const;

    void do_action( model_action* action );

    void set_active_action( const std::string& action_name );
    void reset_active_action();

    void set_mark_selection( const std::string& label);
    void set_bounding_box_selection();
    void clear_selection();

    void set_date(double date);

    bool save();
    bool save_as();

    const gui_model& get_model() const;
    gui_model& get_model();

  private:
    void undo();
    void redo();
    void make_title();

    bool effective_save();

    void create_controls();
    void create_member_controls();
    void create_sizer_controls();
    void create_menu();
    wxMenu* create_model_menu() const;
    wxMenu* create_edit_menu() const;
    wxMenu* create_view_menu() const;

    void create_toolbar();

    void create_accelerators();

    void toggle_wireframe_mode();
    void toggle_graphic_mode();
    void toggle_text_mode();

    bool check_model();
    void check_model_verbose();

    void compile_model();
    void compile_model_no_check();

    void update_layout();
    void update_slider() const;
    void update_button() const;
    void update_toolbar() const;
    void fill();
    bool can_delete_snapshot() const;
    bool can_create_snapshot() const;
    bool can_copy_snapshot() const;
    bool can_paste_snapshot() const;

    model_action* create_delete_snapshot_action();

    void delete_snapshot();
    void create_snapshot();
    void copy_snapshot() const;
    void paste_snapshot();

    snapshot* get_snapshot_from_clipboard() const;

    void play();
    void play_next();
    void stop();

    template<typename MenuType>
    void update_menu( MenuType& m ) const;

    void on_activate(wxActivateEvent& event);
    void on_close(wxCloseEvent& event);
    void on_context_menu(wxContextMenuEvent& event);
    void on_save_as(wxCommandEvent& event);
    void on_save(wxCommandEvent& event);
    void on_undo(wxCommandEvent& event);
    void on_redo(wxCommandEvent& event);
    void on_cut(wxCommandEvent& event);
    void on_copy(wxCommandEvent& event);
    void on_paste(wxCommandEvent& event);
    void on_delete(wxCommandEvent& event);
    void on_check_model(wxCommandEvent& event);
    void on_compile_model(wxCommandEvent& event);
    void on_menu_close(wxCommandEvent& event);
    void on_menu_highlight(wxMenuEvent& event);
    void on_open_menu(wxMenuEvent& event);
    void on_wireframe(wxCommandEvent& event);
    void on_graphic(wxCommandEvent& event);
    void on_text(wxCommandEvent& event);
    void on_zoom_100(wxCommandEvent& event);
    void on_zoom_in(wxCommandEvent& event);
    void on_zoom_out(wxCommandEvent& event);
    void on_zoom_fit(wxCommandEvent& event);
    void on_zoom_change(wxSpinEvent& event);
    void on_mouse_wheel_rotation(wxMouseEvent& event);
    void on_new_snapshot(wxCommandEvent& event);
    void on_delete_snapshot(wxCommandEvent& event);
    void on_copy_snapshot(wxCommandEvent& event);
    void on_paste_snapshot(wxCommandEvent& event);
    void on_show_properties(wxCommandEvent& event);
    void on_slider_change(slider_event& event);
    void on_tick_move(tick_event& event);
    void on_play_stop(wxCommandEvent& event);
    void on_start(wxCommandEvent& event);
    void on_timer(wxTimerEvent& event);
    void on_mark_change(mark_event& event);
    void on_mark_move(placement_event& event);
    void on_mark_size(placement_event& event);
    void on_bounding_box_change(bounding_box_event& event);
    void on_bounding_box_selected(bounding_box_event& event);

  private:
    /** \brief The layout of the windows of the program. */
    windows_layout& m_layout;

    /** \brief The model and the undo/redo informations. */
    model_history m_history;

    /** \brief The path to the model file. */
    wxString m_model_file;

    /** \brief Tell if the model has changes that are not saved. */
    bool m_changed;

    /** \brief The popup menu displayed when right-clicking the model. */
    wxMenu m_popup_menu;

    /** \brief The selected mark. */
    std::string m_mark_selection;

    /** \brief The slider control with display the snaphots. */
    slider_with_ticks* m_slider;

    /** \brief The button to add a snaphot. */
    wxBitmapButton* m_new_button;

    /** \brief The button to delete a snaphot. */
    wxBitmapButton* m_delete_button;

    /** \brief The button to copy the current snapshot. */
    wxBitmapButton* m_snapshot_copy_button;

    /** \brief The button to paste a snapshot at the current date. */
    wxBitmapButton* m_snapshot_paste_button;

    /** \brief The button to play the action. */
    wxBitmapButton* m_play_button;

    /** \brief The control displaying the model (current snapshot). */
    model_view_ctrl* m_model_view;

    /** \brief The control displaying the zoom. */
    wxSpinCtrl* m_zoom_spin;

    /** \brief The timer, for the player. */
    wxTimer m_timer;

    /** \brief The height of buttons. */
    static const unsigned int s_button_height;

    /** \brief The width of buttons. */
    static const unsigned int s_button_width;

    DECLARE_EVENT_TABLE()

  }; // class model_frame
} // namespace bf

#endif // __BF_MODEL_FRAME_HPP__
