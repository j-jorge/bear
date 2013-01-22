/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The main window of our program.
 * \author Julien Jorge
 */
#ifndef __BF_MAIN_FRAME_HPP__
#define __BF_MAIN_FRAME_HPP__

#include "bf/configuration.hpp"
#include <wx/frame.h>

namespace bf
{
  class model_frame;
  class model_properties_ctrl;
  class windows_layout;

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
        ID_SAVE_ALL_MODELS,
        ID_UPDATE_IMAGE_POOL
      }; // enum control_id

  public:
    main_frame();
    ~main_frame();

    void new_model( const wxString& path );
    void load_model( const wxString& path );
    void set_active_model( model_frame* m );

    void update_action();
    void update_snapshot();
    void update_mark();

  private:
    void create_menu();
    void create_controls();

    wxMenu* create_model_menu() const;
    wxMenu* create_help_menu() const;

    void save_config();

    void add_model_view( model_frame* m );

    void on_configuration_menu(wxCommandEvent& event);
    void on_update_image_pool_menu(wxCommandEvent& event);
    void on_new_model(wxCommandEvent& event);
    void on_open_model(wxCommandEvent& event);
    void on_save_all(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_online_doc(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void on_close(wxCloseEvent& event);

  private:
    /** \brief The configuration of the program. */
    void turn_model_entries( bool enable );

    /** \brief The properties of the currently edited model. */
    model_properties_ctrl* m_model_properties;

    /** \brief The windows of the program. */
    windows_layout* m_windows_layout;

    DECLARE_EVENT_TABLE()

  }; // class main_frame
} // namespace bf

#endif // __BF_MAIN_FRAME_HPP__
