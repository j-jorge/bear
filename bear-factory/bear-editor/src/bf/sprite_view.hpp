/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays a sprite.
 * \author Julien Jorge
 */
#ifndef __BF_SPRITE_VIEW_HPP__
#define __BF_SPRITE_VIEW_HPP__

#include <wx/wx.h>

#include "bf/sprite_image_cache.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief This control displays a sprite.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT sprite_view:
    public wxWindow
  {
  public:
    sprite_view
      ( wxWindow& parent, workspace_environment* env, wxWindowID id, 
        const sprite& spr = sprite() );

    void set_sprite( const sprite& spr );
    void set_zoom( unsigned int z );
    unsigned int get_zoom() const;
    void auto_zoom();

    wxPoint get_view_position() const;
    wxSize get_view_size() const;
    void set_view_delta( int x, int y );
    bool convert_position
      ( const wxPoint& mouse_position, wxPoint& position) const;

  private:
    void render();

    void fill_background( wxDC& dc ) const;
    void draw_sprite( wxDC& dc ) const;
    void draw_box( wxDC& dc ) const;

    void make_sprite_image();

    void on_size(wxSizeEvent& event);
    void on_paint(wxPaintEvent& event);

  private:
    /** \brief The pattern of the background. */
    wxBitmap m_background_pattern;

    /** \brief The sprite displayed. */
    sprite m_sprite;

    /** \brief The sprite scaled at the window's size. */
    wxBitmap m_sprite_image;

    /** \brief The position of the sprite in the view. */
    wxPoint m_sprite_position;

    /** \brief A delta applied to the sprite when rendering. */
    wxPoint m_sprite_delta;

    /** \brief A cache of the sprites. */
    sprite_image_cache m_image_cache;

    /** \brief The zoom ratio. */
    unsigned int m_zoom;

    DECLARE_EVENT_TABLE()

  }; // class sprite_view
} // namespace bf

#endif // __BF_SPRITE_VIEW_HPP__
