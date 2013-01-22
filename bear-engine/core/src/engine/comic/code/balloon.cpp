/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::balloon class.
 * \author Sébastien Angibaud
 */
#include "engine/comic/balloon.hpp"

#include "visual/scene_sprite.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor of a balloon.
 */
bear::engine::balloon::balloon()
  : m_size_frame(0, 0), m_has_started(false), m_time(0), m_on_top(true),
    m_on_right(true), m_active(false), m_increasing_duration(0.25)
{
  m_text.set_background_color( claw::graphic::white_pixel );
  m_text.get_text_rendering_attributes().set_intensity(0,0,0);
  
  m_text.set_size( 0, 0 );
  m_text.set_margin(0, 0);
} // balloon::balloon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the size of the balloon.
 * \param elapsed_time The time elapsed since the last call.
 */
void bear::engine::balloon::progress( universe::time_type elapsed_time )
{
  if ( m_has_started )
    {
      m_time += elapsed_time;

      if ( m_time >= m_play_time )
        {
          if ( m_speeches.empty() )
            decrease(elapsed_time);
          else
            {
              m_time = 0;
              write_text();
            }
        }
    }
  else
    {
      increase(elapsed_time);
      m_time += elapsed_time;

      if ( m_time >= m_increasing_duration )
        {
          m_has_started = true;
          m_time = 0;
        }
    }
} // balloon::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visuals of the balloon.
 * \param e (out) The visuals.
 */
void bear::engine::balloon::render( std::list<visual::scene_element>& e )
{
  if ( (m_text.width() == 0) || (m_text.height() == 0) )
    return;

  m_text.render( e );

  m_horizontal_border.flip(false);
  e.push_back
    ( visual::scene_sprite
      (m_text.left(), m_text.bottom() - m_horizontal_border.height(),
       m_horizontal_border) );

  m_horizontal_border.flip(true);
  e.push_back
    ( visual::scene_sprite
      (m_text.left(), m_text.top(), m_horizontal_border) );

  m_vertical_border.mirror(false);
  e.push_back
    ( visual::scene_sprite
      (m_text.left() - m_vertical_border.width(), m_text.bottom(),
       m_vertical_border) );

  m_vertical_border.mirror(true);
  e.push_back
    ( visual::scene_sprite
      (m_text.right(), m_text.bottom(), m_vertical_border) );

  if ( m_on_right )
    {
      if (m_on_top)
        {
          render_bottom_left_corner(e, m_spike);
          render_bottom_right_corner(e, m_corner);
          render_top_left_corner(e, m_corner);
          render_top_right_corner(e, m_corner);
        }
      else
        {
          render_bottom_left_corner(e, m_corner);
          render_bottom_right_corner(e, m_corner);
          render_top_left_corner(e, m_spike);
          render_top_right_corner(e, m_corner);
        }
    }
  else if (m_on_top)
    {
      render_bottom_left_corner(e, m_corner);
      render_bottom_right_corner(e, m_spike);
      render_top_left_corner(e, m_corner);
      render_top_right_corner(e, m_corner);
    }
  else
    {
      render_bottom_left_corner(e, m_corner);
      render_bottom_right_corner(e, m_corner);
      render_top_left_corner(e, m_corner);
      render_top_right_corner(e, m_spike);
    }
} // balloon::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the sprite to use to render the spike of the balloon.
 * \param spr The sprite to use.
 */
void bear::engine::balloon::set_spike_sprite( visual::sprite spr )
{
  m_spike = spr;
} // balloon::set_spike_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the sprite to use to render the corners of the balloon.
 * \param spr The sprite to use. It will be rendered not flipped nor mirrored
 *        for the bottom left corner, and flipped or mirrored for the other
 *        corners.
 */
void bear::engine::balloon::set_corner_sprite( visual::sprite spr )
{
  m_corner = spr;
} // balloon::set_corner_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the sprite to use to render the horizontal borders of the
 *        balloon.
 * \param spr The sprite to use. It will be rendered not flipped for the bottom
 *        border and flipped for the top border.
 */
void bear::engine::balloon::set_horizontal_border_sprite( visual::sprite spr )
{
  m_horizontal_border = spr;
} // balloon::set_horizontal_border_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the sprite to use to render the vertical borders of the
 *        balloon.
 * \param spr The sprite to use. It will be rendered not mirrored for the left
 *        border and mirrored for the right border.
 */
void bear::engine::balloon::set_vertical_border_sprite( visual::sprite spr )
{
  m_vertical_border = spr;
} // balloon::set_vertical_border_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the font to use to render the text.
 * \param f The font to use.
 */
void bear::engine::balloon::set_font( visual::font f )
{
  m_text.set_font( f );
} // balloon::set_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the balloon is finished.
 */
bool bear::engine::balloon::is_finished() const
{
  return !m_active;
} // balloon::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the balloon.
 */
bear::visual::size_box_type bear::engine::balloon::get_size() const
{
  visual::size_box_type result;

  result.x = m_text.width() + m_vertical_border.width() + m_spike.width();
  result.y = m_text.height() + m_horizontal_border.height() + m_spike.height();

  return result;
} // balloon::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the final size of the balloon.
 */
bear::visual::size_box_type bear::engine::balloon::get_final_size() const
{
  visual::size_box_type result(m_size_frame);

  result.x += m_vertical_border.width() + m_spike.width();
  result.y += m_horizontal_border.height() + m_spike.height();

  return result;
} // balloon::get_final_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the balloon.
 * \param pos The position of the balloon.
 * \param on_top Tell that the balloon is on the top of the item.
 * \param on_right Tell that the balloon is on the right of the item.
 */
void bear::engine::balloon::set_position
( const visual::position_type& pos, bool on_top, bool on_right )
{
  m_on_right = on_right;
  m_on_top = on_top;

  visual::position_type delta;

  if (m_on_right)
    delta.x = m_spike.width();
  else
    delta.x = m_vertical_border.width() + (m_size_frame.x - m_text.width());

  if (m_on_top)
    delta.y = m_spike.height();
  else
    delta.y = m_horizontal_border.height() + (m_size_frame.y - m_text.height());

  m_text.set_position(pos + delta);
} // balloon::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the balloon.
 */
bear::visual::position_type bear::engine::balloon::get_position() const
{
  visual::position_type delta;

  if (m_on_right)
    delta.x = -m_spike.width();
  else
    delta.x = -m_vertical_border.width();

  if (m_on_top)
    delta.y = -m_spike.height();
  else
    delta.y = -m_horizontal_border.height();

  return m_text.bottom_left() + delta;
} // balloon::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the balloon is on the top of the item.
 */
bool bear::engine::balloon::is_on_top() const
{
  return m_on_top;
} // balloon::is_on_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the balloon is on the right of the item.
 */
bool bear::engine::balloon::is_on_right() const
{
  return m_on_right;
} // balloon::is_on_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a new list of things to say.
 * \param speeches Speeches said by the speaker.
 */
void
bear::engine::balloon::set_speeches( const std::list<std::string>& speeches )
{
  m_speeches = speeches;
  m_has_started = false;
  m_time = 0;

  if ( !m_speeches.empty() )
    write_text();

  m_size_frame = m_text.get_size();

  m_text.set_size( 0, 0 );
  m_active = true;
} // balloon::set_speeches()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close the balloon. All speeches are removed.
 */
void bear::engine::balloon::close()
{
  m_speeches.clear();
  m_text.set_size( 0, 0 );
  m_active = false;
} // balloon::close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite in the bottom left corner.
 * \param e (out) The visuals.
 * \param s The sprite to render.
 */
void bear::engine::balloon::render_bottom_left_corner
( std::list<visual::scene_element>& e, visual::sprite s ) const
{
  s.mirror(false);
  s.flip(false);

  e.push_back
    ( visual::scene_sprite
      (m_text.left() - s.width(), m_text.bottom() - s.height(), s) );
} // balloon::render_bottom_left_corner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite in the bottom left corner.
 * \param e (out) The visuals.
 * \param s The sprite to render.
 */
void bear::engine::balloon::render_bottom_right_corner
( std::list<visual::scene_element>& e, visual::sprite s ) const
{
  s.mirror(true);
  s.flip(false);

  e.push_back
    ( visual::scene_sprite
      (m_text.right(), m_text.bottom() - s.height(), s) );
} // balloon::render_bottom_right_corner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite in the bottom left corner.
 * \param e (out) The visuals.
 * \param s The sprite to render.
 */
void bear::engine::balloon::render_top_left_corner
( std::list<visual::scene_element>& e, visual::sprite s ) const
{
  s.mirror(false);
  s.flip(true);

  e.push_back
    ( visual::scene_sprite(m_text.left() - s.width(), m_text.top(), s) );
} // balloon::render_top_left_corner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite in the top right corner.
 * \param e (out) The visuals.
 * \param s The sprite to render.
 */
void bear::engine::balloon::render_top_right_corner
( std::list<visual::scene_element>& e, visual::sprite s ) const
{
  s.mirror(true);
  s.flip(true);

  e.push_back( visual::scene_sprite(m_text.right(), m_text.top(), s) );
} // balloon::render_top_right_corner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Increase the balloon.
 * \param elapsed_time The time elapsed since the last call.
 */
void bear::engine::balloon::increase( universe::time_type elapsed_time )
{
  const visual::size_box_type enlargement
    ( m_size_frame / m_increasing_duration * elapsed_time );

  visual::size_box_type size( m_text.get_size() );
  size.x = std::min( size.x + enlargement.x, m_size_frame.x );
  size.y = std::min( size.y + enlargement.y, m_size_frame.y );

  set_content_size(size);
} // balloon::increase()

/*----------------------------------------------------------------------------*/
/**
 * \brief Decrease the balloon.
 * \param elapsed_time The time elapsed since the last call.
 */
void bear::engine::balloon::decrease( universe::time_type elapsed_time )
{
  const visual::size_box_type enlargement
    ( m_size_frame / m_increasing_duration * elapsed_time );

  visual::size_box_type size( m_text.get_size() );

  size.x = std::max( size.x - enlargement.x, 0.0 );
  size.y = std::max( size.y - enlargement.y, 0.0 );

  set_content_size(size);

  m_active = (size.x > 0) || (size.y > 0);
} // balloon::decrease()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the content of the balloon.
 */
void bear::engine::balloon::set_content_size( const visual::size_box_type& s )
{
  m_text.set_size(s);
  m_horizontal_border.set_width(s.x);
  m_vertical_border.set_height(s.y);

  if ( !m_on_top )
    m_text.set_bottom( m_text.top() - m_text.height() );

  if ( !m_on_right )
    m_text.set_left( m_text.right() - m_text.width() );
} // balloon::set_content_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the new text.
 */
void bear::engine::balloon::write_text()
{
  m_text.set_position( 0, 0 );
  m_text.set_auto_size(true);
  m_text.set_text(m_speeches.front());
  m_text.set_auto_size(false);

  if ( m_text.width() > 200 )
    {
      m_text.set_width(200);
      m_text.expand_vertically();
    }

  set_content_size(m_text.get_size());
  m_play_time = m_speeches.front().length()/10;

  if ( m_play_time < 2 )
    m_play_time = 2;
  else if ( m_play_time > 6 )
    m_play_time = 6;

  m_speeches.pop_front();
} // balloon::write_text()
