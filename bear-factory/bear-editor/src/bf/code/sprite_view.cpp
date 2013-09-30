/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sprite_view class.
 * \author Julien Jorge
 */
#include "bf/sprite_view.hpp"

#include "bf/image_pool.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/dcbuffer.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param pool The image poll to use.
 * \param id Window identifier.
 * \param spr The sprite to display.
 */
bf::sprite_view::sprite_view
( wxWindow& parent, const image_pool& pool, wxWindowID id, const sprite& spr )
  : wxWindow(&parent, id), m_background_pattern( 20, 20 ),
    m_sprite_position(0, 0), m_sprite_delta(0, 0), 
    m_image_cache(pool), m_zoom(100)
{
  const wxColour dark_gray(127, 127, 127);
  const wxColour light_gray(192, 192, 192);

  wxMemoryDC dc(m_background_pattern);
  const unsigned int w = m_background_pattern.GetWidth() / 2;
  const unsigned int h = m_background_pattern.GetHeight() / 2;

  dc.SetBrush( wxBrush(dark_gray) );
  dc.SetPen( wxPen(dark_gray) );
  dc.DrawRectangle( 0, 0, w, h );
  dc.DrawRectangle( w, h, w, h );

  dc.SetBrush( wxBrush(light_gray) );
  dc.SetPen( wxPen(light_gray) );
  dc.DrawRectangle( 0, h, w, h );
  dc.DrawRectangle( w, 0, w, h );

  set_sprite(spr);
} // sprite_view::sprite_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sprite to display.
 * \param spr The sprite to display.
 */
void bf::sprite_view::set_sprite( const sprite& spr )
{
  m_sprite = spr;
  make_sprite_image();
  Refresh();
} // sprite_view::set_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the zoom ratio.
 * \param z The zoom ratio.
 */
void bf::sprite_view::set_zoom( unsigned int z )
{
  m_zoom = z;
  make_sprite_image();
  Refresh();
} // sprite_view::set_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoom ratio.
 */
unsigned int bf::sprite_view::get_zoom() const
{
  return m_zoom;
} // sprite_view::get_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the zoom ratio such that the image fits the control.
 */
void bf::sprite_view::auto_zoom()
{
  const unsigned int z = GetSize().x * 100 / m_sprite.width();
  m_zoom = GetSize().y * 100 / m_sprite.height();

  if (z < m_zoom)
    m_zoom = z;

  make_sprite_image();
  Refresh();
} // sprite_view::auto_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the sprite, as rendered in the view.
 */
wxPoint bf::sprite_view::get_view_position() const
{
  return m_sprite_delta;
} // sprite_view::get_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the sprite, as rendered in the view.
 */
wxSize bf::sprite_view::get_view_size() const
{
  return wxSize( m_sprite_image.GetWidth(), m_sprite_image.GetHeight() );
} // sprite_view::get_view_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the delta applied on the position of the sprite when rendering.
 * \param x The delta on the X-axis.
 * \param y The delta on the Y-axis.
 */
void bf::sprite_view::set_view_delta( int x, int y )
{
  if (get_view_size().x > GetSize().x)
    x -= (get_view_size().x - GetSize().x) / 2;
  if (get_view_size().y > GetSize().y)
    y -= (get_view_size().y - GetSize().y) / 2;

  m_sprite_delta.x = x;
  m_sprite_delta.y = y;
  Refresh();
} // sprite_view::set_view_delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a given position into initial sprite coordinate.
 * \param mouse_position The position of the mouse.
 * \param position The position into initial sprite coordinate.
 */
bool bf::sprite_view::convert_position
(const wxPoint& mouse_position, wxPoint& position) const
{
  bool result = true;

  claw::math::coordinate_2d<double> pos;
  pos.x = 
    ( (double)mouse_position.x + (double)m_sprite_delta.x - 
      (double)m_sprite_position.x ) * 100.0 / (double)m_zoom;
  pos.y = 
    ( (double)mouse_position.y + (double)m_sprite_delta.y - 
      (double)m_sprite_position.y ) * 100.0 / (double)m_zoom;
  
  claw::math::coordinate_2d<double> size
    (m_sprite_image.GetWidth(), m_sprite_image.GetHeight());
  size *= (100.0 / (double)m_zoom);

  // rotation
  const double a = - m_sprite.get_angle();
  pos.x = pos.x - size.x / 2;
  pos.y = - pos.y + size.y / 2;
  claw::math::coordinate_2d<double> center(0,0);
  pos.rotate(center, a);
  position.x = pos.x + m_sprite.get_clip_width() / 2;
  position.y = - pos.y + m_sprite.get_clip_height() / 2;

  // flip and mirror
  if ( trinary_logic::to_bool(m_sprite.get_mirrored_status()) )
    position.x = m_sprite.get_clip_width() - position.x;

  if ( trinary_logic::to_bool(m_sprite.get_flipped_status()) )
    position.y = m_sprite.get_clip_height() - position.y;
  
  // Check if position is in the sprite
  if ( position.x < 0 || position.y < 0 ||
       position.x > (int)m_sprite.get_clip_width() || 
       position.y > (int)m_sprite.get_clip_height() ) 
    result = false;

  return result;
} // sprite_view::convert_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the sprite.
 */
void bf::sprite_view::render()
{
  wxBufferedPaintDC dc( this );

  if( IsShown() )
    {
      fill_background(dc);
      draw_sprite(dc);
      draw_box(dc);
    }
} // sprite_view::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the background of the control.
 * \param dc The device context for the drawings.
 */
void bf::sprite_view::fill_background( wxDC& dc ) const
{
  for( int y=0; y<dc.GetSize().y; y+=m_background_pattern.GetHeight() )
    for( int x=0; x<dc.GetSize().x; x+=m_background_pattern.GetWidth() )
      dc.DrawBitmap(m_background_pattern, x, y, false);
} // sprite_view::fill_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the sprite.
 * \param dc The device context for the drawings.
 */
void bf::sprite_view::draw_sprite( wxDC& dc ) const
{
  const wxPoint pos(m_sprite_position - m_sprite_delta);

  if ( m_sprite_image.IsOk() )
    dc.DrawBitmap(m_sprite_image, pos.x, pos.y, true);
} // sprite_view::draw_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the box around the sprite.
 * \param dc The device context for the drawings.
 */
void bf::sprite_view::draw_box( wxDC& dc ) const
{
  const wxPoint pos(m_sprite_position - m_sprite_delta);

  dc.SetBrush( wxBrush(*wxBLACK, wxTRANSPARENT) );

  dc.SetPen( wxPen(*wxWHITE, 1, wxSOLID) );
  dc.DrawRectangle( pos.x - 1, pos.y - 1, m_sprite_image.GetWidth() + 2,
                    m_sprite_image.GetHeight() + 2 );

  dc.SetPen( wxPen(*wxBLACK, 1, wxSHORT_DASH) );
  dc.DrawRectangle( pos.x - 1, pos.y - 1, m_sprite_image.GetWidth() + 2,
                    m_sprite_image.GetHeight() + 2 );

} // sprite_view::draw_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the image of the sprite.
 */
void bf::sprite_view::make_sprite_image()
{
  const wxString name( std_to_wx_string(m_sprite.get_image_name()) );

  const unsigned int w = m_sprite.width() * m_zoom / 100;
  const unsigned int h = m_sprite.height() * m_zoom / 100;

  m_sprite_image = m_image_cache.get_image( m_sprite, w, h ).first;

  m_sprite_position.x = (GetSize().x - m_sprite_image.GetWidth()) / 2;
  m_sprite_position.y = (GetSize().y - m_sprite_image.GetHeight()) / 2;
} // sprite_view::make_sprite_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::sprite_view::on_size( wxSizeEvent& WXUNUSED(event) )
{
  m_sprite_position.x = (GetSize().x - m_sprite_image.GetWidth()) / 2;
  m_sprite_position.y = (GetSize().y - m_sprite_image.GetHeight()) / 2;

  Refresh();
} // sprite_view::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::sprite_view::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  render();
} // sprite_view::on_paint()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::sprite_view, wxWindow)
  EVT_SIZE( bf::sprite_view::on_size )
  EVT_PAINT( bf::sprite_view::on_paint )
END_EVENT_TABLE()
