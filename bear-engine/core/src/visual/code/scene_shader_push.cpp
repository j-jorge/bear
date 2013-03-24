/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_shader_push class.
 * \author Julien Jorge
 */
#include "visual/scene_shader_push.hpp"

#include "visual/base_screen.hpp"
#include "visual/scene_element.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The shader program to use for the next render calls.
 */
bear::visual::scene_shader_push::scene_shader_push( shader_program p )
  : base_scene_element(0, 0), m_shader(p)
{

} // scene_shader_push::scene_shader_push()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocates a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_shader_push::clone() const
{
  return new scene_shader_push(*this);
} // scene_shader_push::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a rectangle where the element is completely opaque.
 */
bear::visual::rectangle_type
bear::visual::scene_shader_push::get_opaque_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // scene_shader_push::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a rectangle bounding the element.
 */
bear::visual::rectangle_type
bear::visual::scene_shader_push::get_bounding_box() const
{
  return rectangle_type( 0, 0, 0, 0 );
} // scene_shader_push::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Splits the element in sub elements.
 * \param boxes The boxes describing how to split the element.
 * \param output The resulting elements (they are inserted to the back).
 */
void bear::visual::scene_shader_push::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // scene_shader_push::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the element on a screen.
 * \param scr The screen on which we render the element.
 */
void bear::visual::scene_shader_push::render( base_screen& scr ) const
{
  scr.push_shader( m_shader );
} // scene_shader_push::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the element must always be displayed.
 */
bool bear::visual::scene_shader_push::always_displayed() const
{
  return true;
} // scene_shader_push::always_displayed()
