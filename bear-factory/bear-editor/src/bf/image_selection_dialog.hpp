/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog to select one of the images in bf::image_pool.
 * \author Julien Jorge
 */
#ifndef __BF_IMAGE_SELECTION_DIALOG_HPP__
#define __BF_IMAGE_SELECTION_DIALOG_HPP__

#include "bf/libeditor_export.hpp"

#include <wx/dialog.h>
#include <wx/textctrl.h>

namespace bf
{
  class image_list_ctrl;
  class workspace_environment;

  /**
   * \brief A dialog to select one of the images in bf::image_pool.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT image_selection_dialog:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_PATTERN_TEXT
      }; // enum control_id

  public:
    image_selection_dialog
      ( wxWindow& parent, workspace_environment& env, 
        const wxString& val = wxEmptyString );

    wxString get_image_name() const;

  private:
    void create_controls();

    void fill_image_list();

    void on_pattern_change( wxCommandEvent& event );

  private:
    /** \brief The list of images. */
    image_list_ctrl* m_image_list;

    /** \brief A text in which the user can enter a part of an image name. */
    wxTextCtrl* m_pattern;

    /** \brief The workspace environment to use. */
    workspace_environment& m_workspace;

    /** \brief The last pattern used in this kind of dialog. */
    static wxString s_previous_pattern;

    DECLARE_EVENT_TABLE()

  }; // class image_selection_dialog
} // namespace bf

#endif // __BF_IMAGE_SELECTION_DIALOG_HPP__
