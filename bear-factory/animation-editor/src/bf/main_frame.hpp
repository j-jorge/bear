/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The main window of our program.
 * \author Julien Jorge
 */
#ifndef __BF_MAIN_FRAME_HPP__
#define __BF_MAIN_FRAME_HPP__

#include <wx/wx.h>

#include "bf/image_pool.hpp"
#include "bf/animation_edit.hpp"
#include "bf/workspace_environment.hpp"

namespace bf
{
  /**
   * \brief The main window of our program.
   * \author Julien Jorge
   */
  class main_frame:
    public wxFrame
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_UPDATE_IMAGE_POOL,
        ID_COMPILE,
        ID_SCALE_DURATION,
        ID_CHANGE_DURATION
      }; // enum control_id

  public:
    main_frame(const std::string & w);

    void load_animation( const wxString& path );

private:
    void search_workspace( const std::string& path );
    void make_title();
    bool is_changed() const;

    bool save();
    bool save_as();
    bool effective_save();

    void turn_animation_menu_entries( bool b );

    void create_menu();
    void create_toolbar();
    void create_controls();

    wxMenu* create_animation_menu() const;
    wxMenu* create_edit_menu() const;
    wxMenu* create_help_menu() const;

    void save_config();

    void compile_animation();
    void compile_animation_no_check();

    void on_configuration_menu(wxCommandEvent& event);
    void on_update_image_pool_menu(wxCommandEvent& event);
    void on_new_animation(wxCommandEvent& event);
    void on_open_animation(wxCommandEvent& event);
    void on_save(wxCommandEvent& event);
    void on_save_as(wxCommandEvent& event);
    void on_compile(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);

    void on_change_duration(wxCommandEvent& event);

    void on_online_doc(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void on_close(wxCloseEvent& event);

  private:
    /** \brief The animation editor. */
    animation_edit* m_animation_edit;

    /** \brief The last saved animation. */
    animation m_last_saved_animation;

    /** \brief The path to the animation file. */
    wxString m_animation_file;

    /** \brief The workspace environment. */
    workspace_environment m_workspace;

    DECLARE_EVENT_TABLE()

  }; // class main_frame
} // namespace bf

#endif // __BF_MAIN_FRAME_HPP__
