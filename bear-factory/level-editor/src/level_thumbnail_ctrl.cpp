/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level_thumbnail_ctrl class.
 * \author Julien Jorge
 */
#include "bf/level_thumbnail_ctrl.hpp"

#include "bf/gui_level.hpp"
#include "bf/image_pool.hpp"
#include "bf/ingame_view.hpp"
#include "bf/ingame_view_frame.hpp"
#include "bf/sprite_with_position.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/dcbuffer.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param env The workspace environment used.
 */
bf::level_thumbnail_ctrl::level_thumbnail_ctrl
( wxWindow* parent, workspace_environment& env )
  : wxWindow(parent, wxID_ANY), m_level(NULL),
    m_image_cache(new sprite_image_cache(env))
{

} // level_thumbnail_ctrl::level_thumbnail_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::level_thumbnail_ctrl::~level_thumbnail_ctrl()
{
  delete m_image_cache;
} // level_thumbnail_ctrl::~level_thumbnail_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the displayed level view.
 * \param view The level view to display.
 */
void bf::level_thumbnail_ctrl::set_view( ingame_view_frame* view )
{
  m_level = view;
  compute_view_box();

  Refresh();
} // level_thumbnail_ctrl::set_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the thumbnail.
 */
void bf::level_thumbnail_ctrl::update()
{
  if (m_level != NULL)
    {
      std::list<sprite_with_position> sprites;
      m_level->get_ingame_view()->get_structure_sprites( sprites );

      double r_x =
        (double)GetSize().x /
        m_level->get_ingame_view()->get_level().get_width();
      double r_y =
        (double)GetSize().y /
        m_level->get_ingame_view()->get_level().get_height();

      m_thumbnail = wxBitmap( GetSize().x, GetSize().y );
      wxMemoryDC dc(m_thumbnail);

      dc.SetBackground(wxColour(127, 127, 127));
      dc.Clear();

      std::list<sprite_with_position>::const_iterator it;

      for (it=sprites.begin(); it!=sprites.end(); ++it)
        {
          wxString name( std_to_wx_string(it->spr.get_image_name()) );

          std::pair<wxBitmap, wxPoint> spr =
            m_image_cache->get_image
            ( it->spr, (unsigned int)(it->spr.width() * r_x),
              (unsigned int)(it->spr.height() * r_y) );

          if ( spr.first.IsOk() )
            dc.DrawBitmap
              ( spr.first,
                (unsigned int)((it->pos.x + spr.second.x) * r_x),
                (unsigned int)((it->pos.y + spr.second.y) * r_y), true );
        }

      compute_view_box();
    }

  Refresh();
} // level_thumbnail_ctrl::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the level.
 */
const bf::ingame_view_frame* bf::level_thumbnail_ctrl::get_level() const
{
  return m_level;
} // level_thumbnail_ctrl::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the box of the view, scaled in this control.
 */
void bf::level_thumbnail_ctrl::compute_view_box()
{
  if (m_level != NULL)
    {
      const ingame_view* v = m_level->get_ingame_view();
      const wxSize level_size
        ( v->get_level().get_width(), v->get_level().get_height() );
      const wxSize my_size( GetSize() );

      m_view_box.x = v->get_view_position().x * my_size.x * 100 /
        ( v->get_zoom() * level_size.x );
      m_view_box.y = ( v->get_level().get_height()
                       - 100 * ( v->get_view_position().y +
                                 v->GetSize().y ) / v->get_zoom()
                      ) * my_size.y / level_size.y;
      m_view_box.width =
        (v->GetSize().x * my_size.x * 100 ) / ( v->get_zoom() * level_size.x );
      m_view_box.height =
        (v->GetSize().y * my_size.y * 100 ) / ( v->get_zoom() * level_size.y );
    }
} // level_thumbnail_ctrl::compute_view_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the level.
 */
void bf::level_thumbnail_ctrl::render()
{
  if ( IsShown() )
    {
      wxBufferedPaintDC dc(this);
      dc.SetBackground(*wxBLACK_BRUSH);
      dc.Clear();

      if ( m_level != NULL )
        {
          dc.DrawBitmap( m_thumbnail, 0, 0 );

          dc.SetPen(*wxRED_PEN);
          dc.SetBrush(*wxTRANSPARENT_BRUSH);

          dc.DrawRectangle(m_view_box);
        }
    }
} // level_thumbnail_ctrl::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the view.
 * \param x X-position in the thumbnail.
 * \param y Y-position in the thumbnail.
 */
void bf::level_thumbnail_ctrl::set_view_position(int x, int y)
{
  ingame_view* v = m_level->get_ingame_view();
  const wxSize level_size
    ( v->get_level().get_width(), v->get_level().get_height() );
  const wxSize my_size( GetSize() );

  wxPoint point
    ( ( x - m_view_box.GetSize().x / 2) * level_size.x / my_size.x,
      (GetSize().y - (y + m_view_box.GetSize().y / 2))
      * level_size.y / my_size.y );

  if ( point.x <0 )
    point.x = 0;
  if ( point.y <0 )
    point.y = 0;

  v->set_view_position
    ( point.x * v->get_zoom() / 100, point.y * v->get_zoom() / 100);

  compute_view_box();
  Refresh();
} // level_thumbnail_ctrl::set_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::level_thumbnail_ctrl::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  render();
} // level_thumbnail_ctrl::on_paint()

/*----------------------------------------------------------------------------*/
/**
 * \brief The control has been resized.
 * \param event The size event.
 */
void bf::level_thumbnail_ctrl::on_size( wxSizeEvent& WXUNUSED(event) )
{
  m_image_cache->clear();
  update();
} // level_thumbnail_ctrl::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief The left button of the mouse is pressed.
 * \param event The mouse event.
 */
void bf::level_thumbnail_ctrl::on_mouse_down(wxMouseEvent& event)
{
  set_view_position(event.GetX(), event.GetY());
} // level_thumbnail_ctrl::on_mouse_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief The mouse is moving.
 * \param event The mouse event.
 */
void bf::level_thumbnail_ctrl::on_mouse_move(wxMouseEvent& event)
{
  if ( event.LeftIsDown() )
    set_view_position(event.GetX(), event.GetY());
} // level_thumbnail_ctrl::on_mouse_move()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::level_thumbnail_ctrl, wxWindow)
  EVT_PAINT( bf::level_thumbnail_ctrl::on_paint )
  EVT_SIZE( bf::level_thumbnail_ctrl::on_size )
  EVT_LEFT_DOWN( bf::level_thumbnail_ctrl::on_mouse_down )
  EVT_MOTION( bf::level_thumbnail_ctrl::on_mouse_move )
END_EVENT_TABLE()
