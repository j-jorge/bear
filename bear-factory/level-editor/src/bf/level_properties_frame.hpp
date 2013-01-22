/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window showing the properties of a level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_PROPERTIES_FRAME_HPP__
#define __BF_LEVEL_PROPERTIES_FRAME_HPP__

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <string>

namespace bf
{
  class level;

  /**
   * \brief The window showing the properties of a level.
   * \author Julien Jorge
   */
  class level_properties_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_BROWSE
      }; // enum control_id

  public:
    level_properties_frame( wxWindow* parent );

    unsigned int get_width() const;
    unsigned int get_height() const;
    const std::string& get_music() const;
    const std::string& get_name() const;

    void init_from( const level& lvl );

  private:
    void fill_controls();
    void create_controls();
    void create_sizer_controls();
    wxSizer* create_level_sizer();
    wxSizer* create_music_sizer();
    wxSizer* create_name_sizer();

    void on_ok(wxCommandEvent& event);
    void on_browse(wxCommandEvent& event);

  private:
    /** \brief The width of the level. */
    unsigned int m_width;

    /** \brief The height of the level. */
    unsigned int m_height;

    /** \brief The music to play in this level. */
    std::string m_music;

    /** \brief The name of the level. */
    std::string m_name;

    /** \brief The text control in which we configure the width of the level. */
    wxSpinCtrl* m_width_text;

    /** \brief The text control in which we configure the height of the
        level. */
    wxSpinCtrl* m_height_text;

    /** \brief The control in which we display the music. */
    wxTextCtrl* m_music_text;

    /** \brief The button to select the music. */
    wxButton* m_music_browse;

    /** \brief The control in which we display the name of the level. */
    wxTextCtrl* m_name_text;

    /** \brief Minimum width of the level. */
    static const unsigned int s_min_width;

    /** \brief Minimum height of the level. */
    static const unsigned int s_min_height;

    DECLARE_EVENT_TABLE()

  }; // class level_properties_frame
} // namespace bf

#endif // __BF_LEVEL_PROPERTIES_FRAME_HPP__
