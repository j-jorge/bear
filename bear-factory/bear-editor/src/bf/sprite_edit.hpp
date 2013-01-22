/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window showing the properties of a sprite.
 * \author Sebastien Angibaud
 */
#ifndef __BF_SPRITE_EDIT_HPP__
#define __BF_SPRITE_EDIT_HPP__

#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/sprite.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <vector>

namespace bf
{
  class bitmap_rendering_attributes_edit;
  class sprite_view_ctrl;

  /**
   * \brief The window showing the properties of a sprite.
   * \author Sebastien Angibaud
   */
  class BEAR_EDITOR_EXPORT sprite_edit:
    public wxPanel,
    public base_edit<sprite>
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_IMAGE_SELECT,
        IDC_RECTANGLE_CLIP_CHANGE,
        IDC_SPRITEPOS_CHOICE
      }; // enum control_id

  public:
    sprite_edit
    ( wxWindow& parent, const sprite& spr = default_value<sprite>::get() );

    bool validate();

  private:
    sprite make_sprite() const;

    void value_updated();
    void fill_controls();

    void create_controls();
    void create_sizer_controls();
    wxSizer* create_position_sizer();
    wxSizer* create_image_name_sizer();

    void fill_spritepos();
    void read_spritepos_file( std::istream& f );

    void control_sprite_size();
    void check_sprite_pos();

    void on_image_select(wxCommandEvent& event);
    void on_refresh(wxCommandEvent& event);
    void on_rectangle_clip_change( wxSpinEvent& event );
    void on_select_sprite_pos( wxCommandEvent& event );

  private:
    /** \brief A combo box with the rectangles from a .spritepos file. */
    wxChoice* m_spritepos_combo;

    /** \brief The text control in which we configure the left of the
        sprite. */
    wxSpinCtrl* m_left_text;

    /** \brief The text control in which we configure the top of the
        sprite. */
    wxSpinCtrl* m_top_text;

    /** \brief The text control in which we configure the width of the
        sprite in the image source. */
    wxSpinCtrl* m_clip_width_text;

    /** \brief The text control in which we configure the height of the
        sprite in the image source. */
    wxSpinCtrl* m_clip_height_text;

    /** \brief The control in which we configure the rendering attributes. */
    bitmap_rendering_attributes_edit* m_rendering_attributes;

    /** \brief The control in which we display the image name. */
    wxTextCtrl* m_image_name_text;

    /** \brief The control in which we display the sprite. */
    sprite_view_ctrl* m_sprite_view;

    DECLARE_EVENT_TABLE()

  }; // class sprite_edit
} // namespace bf

#endif // __BF_SPRITE_EDIT_HPP__
