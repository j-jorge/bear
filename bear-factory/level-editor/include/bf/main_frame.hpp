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

#include "bf/run_configuration.hpp"

#include <wx/wx.h>

#include <list>
#include <string>

namespace bf
{
  class class_tree_ctrl;
  class class_selected_event;
  class ingame_view_frame;
  class layer_list_frame;
  class properties_frame;
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
        ID_ITEM_PROPERTIES,
        ID_LAYER_LIST,
        ID_ITEM_CLASS_POOL,
        ID_SAVE_ALL_LEVELS,
        ID_UPDATE_IMAGE_POOL,
        ID_RUN_CONFIGURATION,
        ID_CLASS_LIST,
        ID_NEW_ITEM,
        ID_TREE_FRAME,
        ID_SELECT_CLASS_DIALOG
      }; // enum control_id

  public:
    main_frame();
    ~main_frame();

    void load_level( const wxString& path );
    void new_level( const wxString& path );

    void set_active_level( ingame_view_frame* view );

    const run_configuration& get_run_configuration() const;
    void use_class( const std::string& c );

  private:
    void save_run_configuration() const;
    void load_run_configuration();

    /** \brief Enable/disable the controls relative to the levels. */
    void turn_level_entries( bool enable );

    void create_menu();
    void create_controls();

    wxMenu* create_level_menu() const;
    wxMenu* create_help_menu() const;

    void save_config();

    void add_level_view( ingame_view_frame* view );

    void on_configuration_menu(wxCommandEvent& event);
    void on_run_configuration_menu(wxCommandEvent& event);
    void on_update_image_pool_menu(wxCommandEvent& event);
    void on_item_properties_menu(wxCommandEvent& event);
    void on_layer_list_menu(wxCommandEvent& event);
    void on_new_level(wxCommandEvent& event);
    void on_open_level(wxCommandEvent& event);
    void on_save(wxCommandEvent& event);
    void on_save_as(wxCommandEvent& event);
    void on_save_all(wxCommandEvent& event);
    void on_new_item(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_menu_open(wxMenuEvent& event);
    void on_menu_highlight(wxMenuEvent& event);
    void on_online_doc(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void on_close(wxCloseEvent& event);
    void on_class_selected( class_selected_event& event );
    void on_close_class_tree(wxCloseEvent& event);

  private:
    /** \brief The window displaying the properties of an item. */
    properties_frame* m_properties_frame;

    /** \brief The list of the layers in the current level. */
    layer_list_frame* m_layer_list;

    /** \brief The windows of the program. */
    windows_layout* m_windows_layout;

    /** \brief The configuration of the program to run to test a level. */
    run_configuration m_run_configuration;

    /** \brief The frame for class tree. */
    wxFrame* m_tree_frame;

    /** \brief The class tree. */
    class_tree_ctrl* m_tree_ctrl;
    
    DECLARE_EVENT_TABLE()

  }; // class main_frame
} // namespace bf

#endif // __BF_MAIN_FRAME_HPP__
