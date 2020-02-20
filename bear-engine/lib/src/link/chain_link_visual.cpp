/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::chain_link_visual class.
 * \author Julien Jorge
 */
#include "bear/generic_items/link/chain_link_visual.hpp"

#include "bear/engine/export.hpp"

BASE_ITEM_EXPORT( chain_link_visual, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::chain_link_visual::chain_link_visual()
  : m_link_count(0), m_max_fall(0), m_dynamic_length(false)
{

} // chain_link_visual::chain_link_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::chain_link_visual::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "chain_link_visual.max_fall" )
    m_max_fall = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // chain_link_visual::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "unsigned int".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::chain_link_visual::set_u_integer_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "chain_link_visual.links_count" )
    m_link_count = value;
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // chain_link_visual::set_u_integer_field()
/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::chain_link_visual::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "chain_link_visual.dynamic_length" )
    m_dynamic_length = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // chain_link_visual::set_bool_field()
/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visuals of this item.
 * \param visuals (out) The visuals.
 */
void bear::chain_link_visual::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  const std::size_t n(get_link_count());
  universe::vector_type dir = get_end_position() - get_start_position();
  const double intensity =
    std::abs(get_end_position().x - get_start_position().x)
    / get_end_position().distance(get_start_position());
  const visual::sprite s(get_sprite());

  universe::vector_type ortho;

  if ( get_end_position().x < get_start_position(). x )
    ortho = dir.get_orthonormal_anticlockwise();
  else
    ortho = dir.get_orthonormal_clockwise();

  ortho.normalize();
  dir /= n-1;
  const visual::position_type origin
    ( get_start_position() - s.get_size() / 2 );

  for (std::size_t i=0; i!=n; ++i)
    {
      visual::position_type p = dir * i;
      double fall_distance = std::sin( (double)i / n * 3.14159 )
        * intensity * m_max_fall;
      p += ortho * fall_distance;

      visuals.push_front
        ( engine::scene_visual( origin + p, s, get_z_position() ) );
    }
} // chain_link_visual::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set number of links to display.
 */
void bear::chain_link_visual::set_link_count(std::size_t link_count)
{
  m_link_count = link_count;
} // chain_link_visual::set_link_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set max fall parameter.
 */
void bear::chain_link_visual::set_max_fall(universe::coordinate_type max_fall)
{
  m_max_fall = max_fall;
} // chain_link_visual::set_max_fall()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if link count is dynamically compute.
 */
void bear::chain_link_visual::set_dynamic_length(bool dynamic_length)
{
  m_dynamic_length = dynamic_length;
} // chain_link_visual::set_dynamic_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get number of links to display.
 */
unsigned int bear::chain_link_visual::get_link_count() const
{
  unsigned int result(m_link_count + 2);

  if ( m_dynamic_length )
    {
      universe::vector_type dir = get_end_position() - get_start_position();
      universe::coordinate_type length = dir.length();

      const visual::sprite s(get_sprite());
      unsigned int size
        ( (s.clip_rectangle().size().x + s.clip_rectangle().size().y) / 2 );

      if ( size == 0 )
        result = 2;
      else
        result = (unsigned int)length / size + 2;
    }

  return result;
} // chain_link_visual::get_link_count()
