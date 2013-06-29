/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_shader_pop class.
 * \author Julien Jorge
 */
#include "visual/scene_shader_pop.hpp"

#include "visual/base_screen.hpp"
#include "visual/scene_element.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::scene_shader_pop::scene_shader_pop()
  : base_scene_element(0, 0)
{

} // scene_shader_pop::scene_shader_pop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocates a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_shader_pop::clone() const
{
  return new scene_shader_pop(*this);
} // scene_shader_pop::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a rectangle where the element is completely opaque.
 */
bear::visual::rectangle_type
bear::visual::scene_shader_pop::get_opaque_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // scene_shader_pop::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a rectangle bounding the element.
 */
bear::visual::rectangle_type
bear::visual::scene_shader_pop::get_bounding_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // scene_shader_pop::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Splits the element in sub elements.
 * \param boxes The boxes describing how to split the element.
 * \param output The resulting elements (they are inserted to the back).
 */
void bear::visual::scene_shader_pop::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // scene_shader_pop::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the element on a screen.
 * \param scr The screen on which we render the element.
 */
void bear::visual::scene_shader_pop::render( base_screen& scr ) const
{
  scr.pop_shader();
} // scene_shader_pop::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the element must always be displayed.
 */
bool bear::visual::scene_shader_pop::always_displayed() const
{
  return true;
} // scene_shader_pop::always_displayed()
