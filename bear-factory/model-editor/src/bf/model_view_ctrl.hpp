/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays the active action of a model.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_VIEW_CTRL_HPP__
#define __BF_MODEL_VIEW_CTRL_HPP__

#include <wx/panel.h>
#include <wx/scrolbar.h>

namespace bf
{
  class gui_model;
  class model_view;
  class mark_event;
  class placement_event;
  class bounding_box_event;

  /**
   * \brief This control displays an action.
   * \author Julien Jorge
   */
  class model_view_ctrl:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_MODEL_VIEW
      }; // enum control_id

  public:
    model_view_ctrl( wxWindow& parent, wxWindowID id, gui_model& m );

    void reload_action();

    void set_date( double d );
    double next();
    bool is_finished() const;
    double get_duration_until_next() const;

    void set_zoom( unsigned int z );
    unsigned int get_zoom() const;
    void auto_zoom();

    void toggle_wireframe_mode();
    bool get_wireframe_drawing() const;

    void toggle_graphic_mode();
    bool get_graphic_drawing() const;

    void toggle_text_mode();
    bool get_text_drawing() const;

  private:
    void create_controls( gui_model& m );
    void create_sizers();

    void adjust_scrollbars();

    void on_scroll(wxScrollEvent& event);
    void on_size(wxSizeEvent& event);
    void on_mark_change(mark_event& event);
    void on_placement_move(placement_event& event);
    void on_placement_size(placement_event& event);
    void on_bounding_box_change(bounding_box_event& event);
    void on_bounding_box_selection(bounding_box_event& event);

  private:
    /** \brief The control that displays the model. */
    model_view* m_model_view;

    /** \brief Horizontal scrollbar to scroll the model. */
    wxScrollBar* m_h_scrollbar;

    /** \brief Vertical scrollbar to scroll the model. */
    wxScrollBar* m_v_scrollbar;

    DECLARE_EVENT_TABLE()

  }; // class model_view_ctrl
} // namespace bf

#endif // __BF_MODEL_VIEW_CTRL_HPP__
