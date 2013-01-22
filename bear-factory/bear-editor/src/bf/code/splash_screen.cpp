/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::splash_screen class.
 * \author Julien Jorge.
 */
#include "bf/splash_screen.hpp"

#include "bf/version.hpp"

#include <wx/app.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/log.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a splash screen.
 * \param img_name The name of the image to display.
 * \param status_style The style of the static text displaying the status of the
 *        application.
 */
bf::splash_screen*
bf::splash_screen::create( const wxString& img_name, long status_style )
{
  wxLogNull no_log; // turn off the error dialog if the file does not exists

  wxImage::AddHandler( new wxPNGHandler() );
  splash_screen* splash(NULL);

  wxBitmap img;

#ifdef BF_IMAGES_PATH
  const wxString splash_path = wxT(BF_IMAGES_PATH "/") + img_name;
  img.LoadFile(splash_path, wxBITMAP_TYPE_PNG);
#endif

  if ( !img.IsOk() ) // try to find the splash screen for a not installed app.
    {
      const wxFileName app( wxTheApp->argv[0] );
      img.LoadFile(app.GetPath() + wxT("/../desktop/images/") + img_name);
    }

  if ( img.IsOk() )
    splash = new splash_screen(img, status_style);

  return splash;
} // splash_screen::create()

#include <iostream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param img The image in the background of the splash screen.
 * \param status_style The style of the static text displaying the status of the
 *        application.
 */
bf::splash_screen::splash_screen( const wxBitmap& img, long status_style )
  : wxSplashScreen( img, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT, 0,
                    NULL, wxID_ANY )
{
  m_status_label =
    new wxStaticText( GetSplashWindow(), wxID_ANY, _("Loading..."),
                      wxDefaultPosition, wxDefaultSize, status_style );
  m_version_label = new wxStaticText
    ( GetSplashWindow(), wxID_ANY, wxT(BF_VERSION_STRING) );

  m_status_label->Move
    ( 0, GetSplashWindow()->GetSize().y - m_status_label->GetSize().y );
  m_status_label->SetSize
    ( GetSplashWindow()->GetSize().x, m_status_label->GetSize().y );

  m_version_label->Move
    ( GetSplashWindow()->GetSize().x - m_version_label->GetSize().x, 0 );
} // splash_screen::splash_screen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the label in which the status is displayed.
 */
wxStaticText& bf::splash_screen::get_status_label()
{
  return *m_status_label;
} // splash_screen::get_status_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the label in which the version is displayed.
 */
wxStaticText& bf::splash_screen::get_version_label()
{
  return *m_version_label;
} // splash_screen::get_version_label()
