/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A splash screen for the applications.
 * \author Julien Jorge
 */
#ifndef __BF_SPLASH_SCREEN_HPP__
#define __BF_SPLASH_SCREEN_HPP__

#include <wx/splash.h>
#include <wx/stattext.h>

namespace bf
{
  /**
   * \brief A splash screen for the applications.
   * \author Julien Jorge
   */
  class splash_screen:
    public wxSplashScreen
  {
  public:
    static splash_screen* create
    ( const wxString& img_name,
      long status_style = wxALIGN_CENTRE | wxST_NO_AUTORESIZE );

    splash_screen( const wxBitmap& img, long status_style );

    wxStaticText& get_status_label();
    wxStaticText& get_version_label();

  private:
    /** \brief The static text where the status of the application is
        displayed. */
    wxStaticText* m_status_label;

    /** \brief The static text where the version of the application is
        displayed. */
    wxStaticText* m_version_label;

  }; // class splash_screen
} // namespace bf

#endif // __BF_SPLASH_SCREEN_HPP__
