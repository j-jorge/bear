/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::rolling_credits class.
 * \author Julien Jorge
 */
#include "generic_items/rolling_credits.hpp"

#include "engine/level_globals.hpp"
#include "engine/resource_pool.hpp"

#include "visual/scene_writing.hpp"
#include "visual/text_layout.hpp"
#include "visual/text_layout_display_size.hpp"

#include <claw/string_algorithm.hpp>
#include <libintl.h>

BASE_ITEM_EXPORT( rolling_credits, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param text The text displayed in the credits.
 * \param font The font used to display the text.
 * \param r The intensity of the red component of the displayed line.
 * \param g The intensity of the green component of the displayed line.
 * \param b The intensity of the blue component of the displayed line.
 * \param o The opacity of the displayed line.
 * \param pos The bottom-left position of the line in the layer.
 * \param width The maximum width of the line.
 * \param a The alignment of the text.
 */
bear::rolling_credits::credit_line::credit_line
( const std::string& text, const visual::font& font, double r, double g,
  double b, double o, const universe::position_type& pos,
  universe::size_type width, visual::text_align::horizontal_align a )
: m_visual
  ( visual::scene_writing
    ( pos.x, pos.y, create_writing(text, font, width, a)) ),
  m_is_on(false)
{
  m_visual.get_rendering_attributes().set_intensity(r, g, b);
  m_visual.get_rendering_attributes().set_opacity(o);
} // rolling_credits::credit_line::credit_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a scene element to display the text.
 */
const bear::visual::scene_element&
bear::rolling_credits::credit_line::create_scene_element() const
{
  return m_visual;
} // rolling_credits::credit_line::create_scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bottom left position of the line in the layer.
 */
const bear::universe::position_type&
bear::rolling_credits::credit_line::get_bottom_left() const
{
  return m_visual.get_position();
} // rolling_credits::credit_line::get_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bottom left position of the line in the layer.
 * \param p The position.
 */
void bear::rolling_credits::credit_line::set_bottom_left
( const universe::position_type& p )
{
  m_visual.set_position(p);
} // rolling_credits::credit_line::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the line on the screen.
 */
bear::universe::size_box_type
bear::rolling_credits::credit_line::get_size() const
{
  return m_visual.get_bounding_box().size();
} // rolling_credits::credit_line::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the line has to be displayed.
 */
bool bear::rolling_credits::credit_line::is_on() const
{
  return m_is_on;
} // rolling_credits::credit_line::is_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the line has to be displayed.
 */
void bear::rolling_credits::credit_line::turn_on()
{
  m_is_on = true;
} // rolling_credits::credit_line::turn_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param text The text displayed in the credits.
 * \param font The font used to display the text.
 * \param width The maximum width of the line.
 * \param a The alignment of the text.
 */
bear::visual::writing bear::rolling_credits::credit_line::create_writing
( const std::string& text, const visual::font& font,
  universe::size_type width, visual::text_align::horizontal_align a ) const
{
  visual::size_box_type s( width, font.get_line_spacing() * text.length() );

  visual::text_layout_display_size func( text, font, s.y );
  visual::text_layout layout( font, text, s, a );

  layout.arrange_text<visual::text_layout_display_size&>( func );

  s = func.get_bounding_box().size();

  if ( s.y < font.get_line_spacing() )
    s.y = font.get_line_spacing();

  return visual::writing( font, text, visual::size_box_type( width, s.y ), a );
} // rolling_credits::credit_line::create_writing()





/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::rolling_credits::rolling_credits()
: m_movement_duration(1), m_fading_frac(-1), m_elapsed_time(0)
{

} // rolling_credits::rolling_credits()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::rolling_credits::build()
{
  super::build();

  std::stringstream iss;

  engine::resource_pool::get_instance().get_file(m_file, iss);
  if ( m_fading_frac < 0 )
    m_fading_frac = 1.0 / 4;
  else
    // initially, m_fading_frac contains the duration of the fading
    m_fading_frac /= m_movement_duration;

  std::string line;
  visual::font font;
  double font_size(12);
  double red(1), green(1), blue(1), opacity(1);
  visual::text_align::horizontal_align horizontal_align
    ( visual::text_align::align_center );
  visual::size_type width( get_width() );

  while ( claw::text::getline(iss, line) )
    if ( line.empty() )
      m_lines.push_back
        ( credit_line
          ( line, font, red, green, blue, opacity,  get_bottom_left(), width,
            horizontal_align ) );
    else if ( line[0] == '#' )
      {
        std::string s;
        std::istringstream iss_line(line);
        iss_line >> s;

        if ( s == "#color" )
          {
            red = green = blue = opacity = 1;
            iss_line >> red >> green >> blue >> opacity;
          }
        else if ( s == "#font" )
          {
            iss_line >> s >> font_size;
            font = get_level_globals().get_font(s, font_size);
          }
        else if ( s == "#align" )
          {
            iss_line >> s;
            horizontal_align =
              visual::text_align::horizontal_align_from_string
              ( s, visual::text_align::align_center );
          }
      }
    else
      m_lines.push_back
        ( credit_line
          ( line, font, red, green, blue, opacity, get_bottom_left(), width,
            horizontal_align ) );
} // rolling_credits::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the state of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::rolling_credits::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( m_lines.empty() )
    return;

  m_elapsed_time += elapsed_time;

  const universe::time_type speed = get_height() / m_movement_duration;
  bool stop(false);
  std::list<credit_line>::iterator it(m_lines.begin());

  for ( it=m_lines.begin(); !stop; )
    {
      universe::position_type p(it->get_bottom_left());
      p.y += speed * elapsed_time;
      it->set_bottom_left( p );
      it->turn_on();

      if ( p.y + it->get_size().y > get_top() )
        it = m_lines.erase(it);
      else
        ++it;

      if (it==m_lines.end())
        stop = true;
      else if ( p.y - get_bottom() < it->get_size().y )
        stop = true;
    }
} // rolling_credits::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::rolling_credits::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  bool stop(false);
  std::list<credit_line>::const_iterator it;

  for (it=m_lines.begin(); !stop && (it!=m_lines.end()); ++it)
    if ( !it->is_on() )
      stop = true;
    else
      {
        visual::scene_element vis(it->create_scene_element());
        double f = (it->get_bottom_left().y - get_bottom())
          / (get_height() - it->get_size().y);

        if ( f < m_fading_frac )
          vis.get_rendering_attributes().set_opacity
            ( vis.get_rendering_attributes().get_opacity()
              * (f / m_fading_frac) );
        else if ( f > 1 - m_fading_frac )
          vis.get_rendering_attributes().set_opacity
            ( vis.get_rendering_attributes().get_opacity()
              * (1 - ((f - (1 - m_fading_frac)) / m_fading_frac)) );

        visuals.push_back(vis);
      }
} // rolling_credits::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::rolling_credits::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "rolling_credits.credits" )
    m_file = value;
  else
    result = super::set_string_field( name, value );

  return result;
} // rolling_credits::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::rolling_credits::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "rolling_credits.movement_duration" )
    m_movement_duration = value;
  else if ( name == "rolling_credits.fading_duration" )
    m_fading_frac = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // rolling_credits::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::rolling_credits::is_valid()
{
  return !m_file.empty() && (m_movement_duration > 0) && super::is_valid();
} // rolling_credits::is_valid()
