/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays a global view of the edited level and allows to
          change the position of the view.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_OVERVIEW_FRAME_HPP__
#define __BF_LEVEL_OVERVIEW_FRAME_HPP__

#include <wx/frame.h>

namespace bf
{
  class level_thumbnail_ctrl;
  class ingame_view_frame;
  class workspace_environment;

  /**
   * \brief This control displays a global view of the edited level and allows
   *        to change the position of the view.
   * \author Julien Jorge
   */
  class level_overview_frame
    : public wxFrame
  {
  public:
     /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_UPDATE_WIDTH,
        ID_UPDATE_HEIGHT
      }; // enum control_id

  public:
    level_overview_frame
    ( ingame_view_frame& parent, workspace_environment* env );
    ~level_overview_frame();

  private:
    void on_close(wxCloseEvent& event);
    void on_refresh(wxCommandEvent& event);
    void on_update_width(wxCommandEvent& event);
    void on_update_height(wxCommandEvent& event);

  private:
    level_thumbnail_ctrl* m_thumbnail;

    DECLARE_EVENT_TABLE()

  }; // class level_overview_frame
} // namespace bf

#endif // __BF_LEVEL_OVERVIEW_FRAME_HPP__
