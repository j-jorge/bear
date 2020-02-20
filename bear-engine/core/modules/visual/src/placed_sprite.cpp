/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::placed_sprite class.
 * \author Julien Jorge
 */
#include "bear/visual/placed_sprite.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::visual::placed_sprite::placed_sprite()
{

} // placed_sprite::placed_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x The x-positon of the sprite.
 * \param y The y-position of the sprite.
 * \param s The sprite.
 */
bear::visual::placed_sprite::placed_sprite
( coordinate_type x, coordinate_type y, const sprite& s )
  : m_sprite(s), m_position(x, y)
{

} // placed_sprite::placed_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The positon of the sprite.
 * \param s The sprite.
 */
bear::visual::placed_sprite::placed_sprite
( const position_type& p, const sprite& s )
  : m_sprite(s), m_position(p)
{

} // placed_sprite::placed_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite.
 */
const bear::visual::sprite& bear::visual::placed_sprite::get_sprite() const
{
  return m_sprite;
} // placed_sprite::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite.
 */
bear::visual::sprite& bear::visual::placed_sprite::get_sprite()
{
  return m_sprite;
} // placed_sprite::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sprite.
 * \param spr The new sprite.
 */
void bear::visual::placed_sprite::set_sprite( const sprite& spr )
{
  m_sprite = spr;
} // placed_sprite::set_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position.
 */
const bear::visual::position_type&
bear::visual::placed_sprite::get_position() const
{
  return m_position;
} // placed_sprite::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position.
 */
bear::visual::position_type& bear::visual::placed_sprite::get_position()
{
  return m_position;
} // placed_sprite::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position.
 * \param p The new position.
 */
void bear::visual::placed_sprite::set_position( const position_type& p )
{
  m_position = p;
} // placed_sprite::set_position()
