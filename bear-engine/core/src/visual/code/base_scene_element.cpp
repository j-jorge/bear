/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::base_scene_element class.
 * \author Julien Jorge
 */
#include "visual/base_scene_element.hpp"

#include "visual/scene_element.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the element on the screen.
 * \param y Y-position of the element on the screen.
 */
bear::visual::base_scene_element::base_scene_element
( coordinate_type x, coordinate_type y )
  : m_position(x, y), m_scale_factor(1, 1), m_shadow(0, 0), m_shadow_opacity(1)
{

} // base_scene_element::base_scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::visual::base_scene_element::~base_scene_element()
{
  // nothing to do
} // base_scene_element::~base_scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy on the element.
 */
bear::visual::base_scene_element*
bear::visual::base_scene_element::clone() const
{
  return new base_scene_element(*this);
} // base_scene_element::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a box where the element is fully opaque.
 */
bear::visual::rectangle_type
bear::visual::base_scene_element::get_opaque_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // base_scene_element::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bounding box of the element.
 */
bear::visual::rectangle_type
bear::visual::base_scene_element::get_bounding_box() const
{
  return
    rectangle_type
    ( m_position.x, m_position.y,
      m_position.x + m_rendering_attributes.width() * get_scale_factor_x(),
      m_position.y + m_rendering_attributes.height() * get_scale_factor_y() );
} // base_scene_element::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the element into subelements.
 * \param boxes The boxes describing how to split the element.
 * \param output The resulting elements.
 */
void bear::visual::base_scene_element::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // base_scene_element::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the sprite on a screen.
 * \param scr The screen on which we render the sprite.
 */
void bear::visual::base_scene_element::render( base_screen& scr ) const
{
  // nothing to do
} // base_scene_element::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position on the element on the screen.
 */
const bear::visual::position_type&
bear::visual::base_scene_element::get_position() const
{
  return m_position;
} // base_scene_element::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position on the element on the screen.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::visual::base_scene_element::set_position
( coordinate_type x, coordinate_type y )
{
  m_position.set(x, y);
} // base_scene_element::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position on the element on the screen.
 * \param p The new position.
 */
void bear::visual::base_scene_element::set_position( position_type p )
{
  set_position( p.x, p.y );
} // base_scene_element::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the distance of the projection of the shadow.
 */
const bear::visual::position_type&
bear::visual::base_scene_element::get_shadow() const
{
  return m_shadow;
} // base_scene_element::get_shadow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the distance of the projection of the shadow.
 * \param x The distance on the x axis.
 * \param y The distance on the y axis.
 */
void bear::visual::base_scene_element::set_shadow
( coordinate_type x, coordinate_type y )
{
  m_shadow.set(x, y);
} // base_scene_element::set_shadow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opacity of the shadow.
 */
double bear::visual::base_scene_element::get_shadow_opacity() const
{
  return m_shadow_opacity;
} // base_scene_element::get_shadow_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the opacity of the shadow.
 * \param o The new opacity.
 */
void bear::visual::base_scene_element::set_shadow_opacity( double o )
{
  m_shadow_opacity = std::max( 0.0, std::min( 1.0, o ) );
} // base_scene_element::set_shadow_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the factor applied to the element when rendered.
 * \param x The factor on the x-axis.
 * \param y The factor on the y-axis.
 */
void bear::visual::base_scene_element::set_scale_factor( double x, double y )
{
  m_scale_factor.x = x;
  m_scale_factor.y = y;
} // base_scene_element::set_scale_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the factor applied on the x-axis to the element when rendered.
 */
double bear::visual::base_scene_element::get_scale_factor_x() const
{
  return m_scale_factor.x;
} // base_scene_element::get_scale_factor_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the factor applied on the y-axis to the element when rendered.
 */
double bear::visual::base_scene_element::get_scale_factor_y() const
{
  return m_scale_factor.y;
} // base_scene_element::get_scale_factor_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the attributes applied to the element when rendering.
 * \param a The attributes.
 */
void bear::visual::base_scene_element::set_rendering_attributes
( const bitmap_rendering_attributes& a )
{
  m_rendering_attributes = a;
} // base_scene_element::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the attributes applied to the element when rendering.
 */
const bear::visual::bitmap_rendering_attributes&
bear::visual::base_scene_element::get_rendering_attributes() const
{
  return m_rendering_attributes;
} // base_scene_element::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the attributes applied to the element when rendering.
 */
bear::visual::bitmap_rendering_attributes&
bear::visual::base_scene_element::get_rendering_attributes()
{
  return m_rendering_attributes;
} // base_scene_element::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the element must always be displayed.
 */
bool bear::visual::base_scene_element::always_displayed() const
{
  return false;
} // base_scene_element::always_displayed()
