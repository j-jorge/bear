/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays a sprite and some buttons to adjust its size.
 * \author Julien Jorge
 */
#ifndef __BF_SPRITE_VIEW_CTRL_HPP__
#define __BF_SPRITE_VIEW_CTRL_HPP__

#include "bf/workspace_environment.hpp"
#include "bf/sprite.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include <wx/stattext.h>

namespace bf
{
  class sprite_view;

  /**
   * \brief This control displays a sprite and some buttons to adjust its size.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT sprite_view_ctrl:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_ZOOM_100,
        ID_ZOOM_FIT,
        ID_ZOOM_IN,
        ID_ZOOM_OUT,
        ID_COMBO_ZOOM,
        ID_SPRITE_VIEW
      }; // enum control_id

  public:
    sprite_view_ctrl
      ( wxWindow& parent, workspace_environment* env, 
        const sprite& spr = sprite() );

    void set_sprite( const sprite& spr );

  private:
    void create_controls();
    void create_sizers();

    void set_zoom_from_combo();

    void adjust_scrollbars();

    void on_zoom_100( wxCommandEvent& event );
    void on_zoom_fit( wxCommandEvent& event );
    void on_zoom_in( wxCommandEvent& event );
    void on_zoom_out( wxCommandEvent& event );
    void on_zoom_selection( wxCommandEvent& event );
    void on_scroll(wxScrollEvent& event);
    void on_size(wxSizeEvent& event);
    void on_mouse_move(wxMouseEvent& event);

  private:
    /** \brief The control that displays the sprite. */
    sprite_view* m_sprite_view;

    /** \brief The combo box with the zoom ratio. */
    wxComboBox* m_combo_zoom;

    /** \brief Horizontal scrollbar to scroll the sprite. */
    wxScrollBar* m_h_scrollbar;

    /** \brief Vertical scrollbar to scroll the sprite. */
    wxScrollBar* m_v_scrollbar;

    /** \brief A static text for mouse position. */
    wxStaticText* m_mouse_position;

    /** \brief The workspace environment to use. */
    workspace_environment* m_workspace;

    DECLARE_EVENT_TABLE()

  }; // class sprite_view_ctrl
} // namespace bf

#endif // __BF_SPRITE_VIEW_CTRL_HPP__
