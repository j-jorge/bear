/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Control for editing a field whose value is a path to an animation.
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_FILE_EDIT_HPP__
#define __BF_ANIMATION_FILE_EDIT_HPP__

#include "bf/animation_file_type.hpp"
#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/wx.h>

namespace bf
{
  class bitmap_rendering_attributes_edit;
  class animation_view_ctrl;

  /**
   * \brief Control for editing a field whose value is a path to an animation.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation_file_edit:
    public wxPanel,
    public base_edit<animation_file_type>
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_BROWSE_ANIMATION
      }; // enum control_id

  private:
    typedef base_edit<animation_file_type> super;

  public:
    animation_file_edit
    ( wxWindow& parent,
      const animation_file_type& v =
      default_value<animation_file_type>::get() );

    bool validate();

  private:
    animation_file_type make_animation_file() const;

    void value_updated();
    void fill_controls();

    void create_controls();
    void create_sizer_controls();
    wxSizer* create_path_sizer();

    void animation_view_load();

    void on_browse_animation(wxCommandEvent& event);
    void on_refresh( wxCommandEvent& event );

  private:
    /** \brief The control in which we configure the rendering attributes. */
    bitmap_rendering_attributes_edit* m_rendering_attributes;

    /** \brief The control in which we display the path to the animation. */
    wxTextCtrl* m_path_text;

    /** \brief The control in which we display the animation. */
    animation_view_ctrl* m_animation_view;

    DECLARE_EVENT_TABLE()

  }; // class animation_file_edit
} // namespace bf

#endif // __BF_ANIMATION_FILE_EDIT_HPP__
