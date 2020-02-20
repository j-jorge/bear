/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
#include "bear/visual/sequence_effect.hpp"

#include <boost/math/constants/constants.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds an effect that does nothing.
 */
bear::visual::sequence_effect::sequence_effect()
  : m_time(0), m_decrease_coefficient(0), m_wave_length(0), 
    m_wave_height(0), m_wave_speed(0)
{

} // sequence_effect::sequence_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a color that can be applied in the effect.
 * \param c The color.
 */
void bear::visual::sequence_effect::add_color( color_type c )
{
  m_colors.push_back(c);
} // sequence_effect::add_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a color from the effect.
 * \param i The index of the element in the sequence.
 */
bear::visual::color_type
bear::visual::sequence_effect::get_color( std::size_t i ) const
{
  if ( m_colors.empty() )
    return claw::graphic::white_pixel;
  else
    return m_colors[ i % m_colors.size() ];
} // sequence_effect::get_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the length of the wave applied to the sequence, in term of
 *        elements in the sequence.
 * \param length The length of the wave.
 */
void bear::visual::sequence_effect::set_wave_length( std::size_t length )
{
  m_wave_length = length;
} // sequence_effect::set_wave_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the wave applied to the sequence.
 * \param h The height of the wave.
 */
void bear::visual::sequence_effect::set_wave_height( size_type h )
{
  m_wave_height = h;
} // sequence_effect::set_wave_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the speed of the wave applied to the sequence.
 * \param s The speed of the wave.
 */
void bear::visual::sequence_effect::set_wave_speed( size_type s )
{
  m_wave_speed = s;
} // sequence_effect::set_wave_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height to decrease per second.
 * \param coefficient The height to decrease per second.
 */
void bear::visual::sequence_effect::set_decrease_coefficient
( size_type coefficient )
{
  m_decrease_coefficient = coefficient;
} // sequence_effect::set_decrease_coefficient()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the delta to apply to an element of the sequence.
 * \param i The index of the element in the sequence.
 */
bear::visual::position_type
bear::visual::sequence_effect::get_delta( std::size_t i ) const
{
  if ( m_wave_length == 0 )
    return position_type(0, 0);
  else
    {
      const coordinate_type y =
        (m_wave_height / 2)
        * std::sin
        ( ( (m_time * m_wave_speed)
            + (double)(i % m_wave_length) / m_wave_length )
          * (2 * boost::math::constants::pi<coordinate_type>()) );

      return position_type( 0, y );
    }
} // sequence_effect::get_delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the effect.
 * \param t The elapsed time since the last update.
 */
void bear::visual::sequence_effect::update( double t )
{
  m_time += t;

  size_type height =
    (double)m_decrease_coefficient * t;

  if ( m_wave_height <=  height )
    m_wave_height = 0;
  else
    m_wave_height -= height;
} // sequence_effect::update()
