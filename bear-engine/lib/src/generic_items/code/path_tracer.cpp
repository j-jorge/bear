/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::path_tracer class.
 * \author Julien Jorge
 */
#include "generic_items/path_tracer.hpp"

#include "generic_items/path_trace.hpp"
#include <algorithm>
#include <limits>

BASE_ITEM_EXPORT(path_tracer, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::path_tracer::path_tracer()
: m_fill_color(claw::graphic::black_pixel), m_fade_out_speed(1),
  m_maximal_length( std::numeric_limits<universe::time_type>::max() ),
  m_tail_ratio(1)
{

} // path_tracer::path_tracer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field to set.
 * \param value The value to give to the field.
 */
bool bear::path_tracer::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if ( name == "path_tracer.fill_color.red" )
    m_fill_color.set_red_intensity(value);
  else if ( name == "path_tracer.fill_color.green" )
    m_fill_color.set_green_intensity(value);
  else if ( name == "path_tracer.fill_color.blue" )
    m_fill_color.set_blue_intensity(value);
  else if ( name == "path_tracer.fill_color.opacity" )
    m_fill_color.set_opacity(value);
  else if ( name == "path_tracer.fade_out_speed" )
    m_fade_out_speed = value;
  else if ( name == "path_tracer.tail_ratio" )
    m_tail_ratio = value;
  else if ( name == "path_tracer.maximal_length" )
    m_maximal_length = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // path_tracer::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::path_tracer::set_color_field
( const std::string& name, visual::color value )
{
  bool ok = true;
  
  if (name == "path_tracer.fill_color")
    m_fill_color = value;
  else
    ok = super::set_color_field(name, value);

  return ok;
} // path_tracer::set_color_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::path_tracer::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  handle_list_type::iterator it=m_traces.begin();

  while ( it != m_traces.end() )
    if ( *it == (universe::physical_item*)NULL )
      {
        handle_list_type::iterator tmp(it);
        ++it;
        m_traces.erase(tmp);
      }
    else
      ++it;
} // path_tracer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the trace.
 * \param c The new color.
 */
void bear::path_tracer::set_fill_color( const visual::color_type& c )
{
  m_fill_color = c;
} // path_tracer::set_fill_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the speed of the fade out of the trace when the traced item is
 *        dead.
 * \param s Units of opacity lost per second.
 */
void bear::path_tracer::set_fade_out_speed( double s )
{
  m_fade_out_speed = s;
} // path_tracer::set_fade_out_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief The condition is verified.
 * \param activator The item on which the condition is verified.
 */
void bear::path_tracer::on_trigger_on( base_item* activator )
{
  if ( activator == NULL )
    return;

  if ( std::find(m_traces.begin(), m_traces.end(), handle_type(activator))
       == m_traces.end() )
    {
      m_traces.push_front(activator);

      path_trace* trace = new path_trace(*activator);
      trace->set_fill_color(m_fill_color);
      trace->set_fade_out_speed( m_fade_out_speed );
      trace->set_length( m_maximal_length );
      trace->set_tail_ratio( m_tail_ratio );

      new_item(*trace);
    }
} // path_tracer::on_trigger_on()
