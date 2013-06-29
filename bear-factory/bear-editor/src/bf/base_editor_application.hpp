/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The class representing the application.
 * \author Julien Jorge
 */
#ifndef __BF_BASE_EDITOR_APPLICATION_HPP__
#define __BF_BASE_EDITOR_APPLICATION_HPP__

#include <wx/wx.h>

namespace bf
{
  /**
   * \brief The class representing the application.
   * \author Julien Jorge
   */
  class base_editor_application:
    public wxApp
  {
  public:
    base_editor_application();
    ~base_editor_application();

  protected:
    virtual void compile( const wxString& path ) const;
    virtual void update( const wxString& path ) const;
    virtual bool do_init_app();
    virtual bool do_command_line_init();

  private:
    bool OnInit();
    bool init_app();
    bool command_line_init();

    bool compile_arguments() const;
    bool update_arguments() const;
    bool show_help();
    bool show_version();
    bool find_and_erase_option
    ( const wxString& long_name, const wxString& short_name );

  private:
    /** \brief The locale, for internationalization. */
    wxLocale m_locale;

  }; // class base_editor_application
} // namespace bf

#endif // __BF_BASE_EDITOR_APPLICATION_HPP__
