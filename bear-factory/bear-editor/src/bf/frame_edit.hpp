/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of a frame.
 * \author Sebastien Angibaud
 */
#ifndef __BF_FRAME_EDIT_HPP__
#define __BF_FRAME_EDIT_HPP__

#include "bf/free_edit.hpp"
#include "bf/animation_frame.hpp"
#include "bf/custom_type.hpp"
#include "bf/image_pool.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/dialog.h>

namespace bf
{
  class sprite_edit;

  /**
   * \brief The window showing the properties of a frame.
   * \author Sebastien Angibaud
   */
  class BEAR_EDITOR_EXPORT frame_edit:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_DURATION,
  IDC_SPRITE
      }; // enum control_id

  public:
    frame_edit
      ( wxWindow& parent, const image_pool& pool,
        const animation_frame& frame = default_value<animation_frame>::get());

    const animation_frame& get_frame() const;

  private:
    void validate();
    void create_controls();
    void create_sizer_controls();

    void fill();
    void edit_sprite();
    void edit_duration();

    void on_ok(wxCommandEvent& event);

  private:
   /** \brief The edited frame. */
    animation_frame m_frame;

    /** \brief The control for duration. */
    free_edit<real_type>* m_duration;

    /** \brief The control in which we display the sprite. */
    sprite_edit* m_sprite;

    /** \brief The image pool to use. */
    const image_pool& m_image_pool;

    // DECLARE_EVENT_TABLE()

  }; // class frame_edit
} // namespace bf

#endif // __BF_FRAME_EDIT_HPP__
