/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control display a small global view of the edited level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_THUMBNAIL_CTRL_HPP__
#define __BF_LEVEL_THUMBNAIL_CTRL_HPP__

#include "bf/sprite_image_cache.hpp"

#include <wx/window.h>

namespace bf
{
  class ingame_view_frame;
  class workspace_environment;

  /**
   * \brief This control display a small global view of the edited level.
   * \author Julien Jorge
   */
  class level_thumbnail_ctrl
    : public wxWindow
  {
  public:
    level_thumbnail_ctrl( wxWindow* parent, workspace_environment& env );
    ~level_thumbnail_ctrl();

    void set_view( ingame_view_frame* view );
    void update();

    const ingame_view_frame* get_level() const;

  private:
    void compute_view_box();

    void render();
    void set_view_position(int x, int y);

    void on_paint(wxPaintEvent& event);
    void on_size(wxSizeEvent& event);
    void on_mouse_down(wxMouseEvent& event);
    void on_mouse_move(wxMouseEvent& event);

  private:
    /** \brief The view on the edited level. */
    ingame_view_frame* m_level;

    /** \brief The position of the view in the level view. */
    wxRect m_view_box;

    /** \brief The thumbnail view of the level. */
    wxBitmap m_thumbnail;

    /** \brief A cache of the sprites of the level. */
    sprite_image_cache* m_image_cache;

    DECLARE_EVENT_TABLE()

  }; // class level_thumbnail_ctrl
} // namespace bf

#endif // __BF_LEVEL_THUMBNAIL_CTRL_HPP__
