/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog that displays some informations on the program.
 * \author Julien Jorge
 */
#ifndef __BF_ABOUT_DIALOG_HPP__
#define __BF_ABOUT_DIALOG_HPP__

#include <wx/dialog.h>

namespace bf
{
  /**
   * \brief A dialog that displays some informations on the program.
   * \author Julien Jorge
   */
  class about_dialog:
    public wxDialog
  {
  public:
    about_dialog( wxWindow& parent );

    wxString get_image_name() const;

  private:
    void create_controls();

  }; // class about_dialog
} // namespace bf

#endif // __BF_ABOUT_DIALOG_HPP__
