/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::scene_visual class.
 * \author Julien Jorge
 */
#include "engine/scene_visual.hpp"

#include "visual/scene_sprite.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two scene_visual instances on their z_position.
 * \return s1.z_position < s2.z_position
 */
bool bear::engine::scene_visual::z_position_compare::operator()
( const scene_visual& s1, const scene_visual& s2 ) const
{
  return s1.z_position < s2.z_position;
} // scene_visual::z_position_compare::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-coordinate of the sprite in the world.
 * \param y Y-coordinate of the sprite in the world.
 * \param spr The sprite to display.
 * \param z The position of the visual in the render procedure.
 */
bear::engine::scene_visual::scene_visual
( universe::coordinate_type x, universe::coordinate_type y,
  const visual::sprite& spr, int z )
  : scene_element( visual::scene_sprite(x, y, spr) ), z_position(z)
{

} // scene_visual::scene_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pos Position of the sprite in the world.
 * \param spr The sprite to display.
 * \param z The position of the visual in the render procedure.
 */
bear::engine::scene_visual::scene_visual
( const universe::position_type& pos,
  const visual::sprite& spr, int z )
  : scene_element( visual::scene_sprite(pos.x, pos.y, spr) ), z_position(z)
{

} // scene_visual::scene_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pos Position of the sprite in the world.
 * \param e The element to display.
 * \param z The position of the visual in the render procedure.
 */
bear::engine::scene_visual::scene_visual
( const visual::scene_element& e, int z )
  : scene_element(e), z_position(z)
{

} // scene_visual::scene_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pos Position of the sprite in the world.
 * \param e The element to display.
 * \param z The position of the visual in the render procedure.
 */
bear::engine::scene_visual::scene_visual
( const visual::base_scene_element& e, int z )
  : scene_element(e), z_position(z)
{

} // scene_visual::scene_visual()
