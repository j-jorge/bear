/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Control for editing a field whose value is any animation.
 * \author Julien Jorge
 */
#ifndef __BF_ANY_ANIMATION_EDIT_HPP__
#define __BF_ANY_ANIMATION_EDIT_HPP__

#include "bf/any_animation.hpp"
#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/wx.h>

namespace bf
{
  class animation_edit;
  class animation_file_edit;
  class image_pool;

  /**
   * \brief Control for editing a field whose value is any animation.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT any_animation_edit:
    public wxPanel,
    public base_edit<any_animation>
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_CONTENT_TYPE
      }; // enum control_id

  private:
    typedef base_edit<any_animation> super;

  public:
    any_animation_edit
      ( wxWindow& parent, const image_pool& pool,
        const any_animation& v =
        default_value<any_animation>::get() );

    bool validate();

  private:
    void value_updated();
    void fill_controls();

    any_animation::content_type get_visible_content_type() const;

    void create_controls();
    void create_sizer_controls();

    void on_switch_content_type( wxCommandEvent& event );

  private:
    /** \brief The control in which we display the type of the content currently
        edited. */
    wxChoice* m_content_type;

    /** \brief The control used to edit the animation. */
    animation_edit* m_animation_edit;

    /** \brief The control used to edit the animation file. */
    animation_file_edit* m_animation_file_edit;

    /** \brief The image pool to use. */
    const image_pool& m_image_pool;

    DECLARE_EVENT_TABLE()

  }; // class any_animation_edit
} // namespace bf

#endif // __BF_ANY_ANIMATION_EDIT_HPP__
