/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_element.
 * \author Julien Jorge
 */
#include "visual/scene_element.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param e The effective element of the scene.
 */
bear::visual::scene_element::scene_element( const base_scene_element& e )
  : m_elem(e.clone())
{

} // scene_element::scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::visual::scene_element::scene_element( const scene_element& that )
  : m_elem(that.m_elem->clone())
{

} // scene_element::scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::visual::scene_element::~scene_element()
{
  delete m_elem;
} // scene_element::~scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment
 * \param that The instance to copy from.
 */
bear::visual::scene_element&
bear::visual::scene_element::operator=( const scene_element& that )
{
  base_scene_element* tmp( that.m_elem->clone() );

  delete m_elem;
  m_elem = tmp;

  return *this;
} // scene_element::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the element is fully opaque.
 */
bear::visual::rectangle_type bear::visual::scene_element::get_opaque_box() const
{
  return m_elem->get_opaque_box();
} // scene_element::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the element.
 */
bear::visual::rectangle_type
bear::visual::scene_element::get_bounding_box() const
{
  return m_elem->get_bounding_box();
} // scene_element::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the element in sub elements
 * \param boxes The boxes describing how to split the element.
 * \param output The resulting elements (they are inserted to the front.
 */
void bear::visual::scene_element::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  m_elem->burst(boxes, output);
} // scene_element::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the element on a screen.
 * \param scr The screen on which we render the element.
 */
void bear::visual::scene_element::render( base_screen& scr ) const
{
  m_elem->render(scr);
} // scene_element::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position on the element on the screen.
 */
const bear::visual::position_type&
bear::visual::scene_element::get_position() const
{
  return m_elem->get_position();
} // scene_element::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position on the element on the screen.
 * \param p The new position.
 */
void bear::visual::scene_element::set_position( const position_type& p )
{
  m_elem->set_position(p.x, p.y);
} // scene_element::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position on the element on the screen.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::visual::scene_element::set_position
( coordinate_type x, coordinate_type y )
{
  m_elem->set_position(x, y);
} // scene_element::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the element must be displayed with a shadow.
 */
bool bear::visual::scene_element::has_shadow() const
{
  return (get_shadow().x != 0) && (get_shadow().y != 0)
    && (get_shadow_opacity() != 0);
} // scene_element::has_shadow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the distance of the projection of the shadow.
 */
const bear::visual::position_type&
bear::visual::scene_element::get_shadow() const
{
  return m_elem->get_shadow();
} // scene_element::get_shadow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the distance of the projection of the shadow.
 * \param p The distance on both axis.
 */
void bear::visual::scene_element::set_shadow( const position_type& p )
{
  m_elem->set_shadow(p.x, p.y);
} // scene_element::set_shadow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the distance of the projection of the shadow.
 * \param x The distance on the x axis.
 * \param y The distance on the y axis.
 */
void bear::visual::scene_element::set_shadow
( coordinate_type x, coordinate_type y )
{
  m_elem->set_shadow(x, y);
} // scene_element::set_shadow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opacity of the shadow.
 */
double bear::visual::scene_element::get_shadow_opacity() const
{
  return m_elem->get_shadow_opacity();
} // scene_element::get_shadow_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the opacity of the shadow.
 * \param o The new opacity.
 */
void bear::visual::scene_element::set_shadow_opacity( double o )
{
  m_elem->set_shadow_opacity(o);
} // scene_element::set_shadow_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the factor applied to the element when rendered.
 * \param x The factor on the x-axis.
 * \param y The factor on the y-axis.
 */
void bear::visual::scene_element::set_scale_factor( double x, double y )
{
  m_elem->set_scale_factor(x, y);
} // scene_element::set_scale_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the factor applied to the element when rendered.
 * \param r The factor on both axis.
 */
void bear::visual::scene_element::set_scale_factor( double r )
{
  m_elem->set_scale_factor(r, r);
} // scene_element::set_scale_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the factor applied on the x-axis to the element when rendered.
 */
double bear::visual::scene_element::get_scale_factor_x() const
{
  return m_elem->get_scale_factor_x();
} // scene_element::get_scale_factor_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the factor applied on the y-axis to the element when rendered.
 */
double bear::visual::scene_element::get_scale_factor_y() const
{
  return m_elem->get_scale_factor_y();
} // scene_element::get_scale_factor_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the element (before the scaling factor).
 */
bear::visual::size_type bear::visual::scene_element::get_element_width() const
{
  if ( get_scale_factor_x() == 0 )
    return get_bounding_box().width();
  else
    return get_bounding_box().width() / get_scale_factor_x();
} // scene_element::get_element_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the element (before the scaling factor).
 */
bear::visual::size_type bear::visual::scene_element::get_element_height() const
{
  if ( get_scale_factor_y() == 0 )
    return get_bounding_box().height();
  else
    return get_bounding_box().height() / get_scale_factor_y();
} // scene_element::get_element_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the element (with the scaling factor).
 */
bear::visual::size_type bear::visual::scene_element::get_width() const
{
  return get_bounding_box().width();
} // scene_element::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the element (with the scaling factor).
 */
bear::visual::size_type bear::visual::scene_element::get_height() const
{
  return get_bounding_box().height();
} // scene_element::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the attributes applied to the element when rendering.
 * \param a The attributes.
 */
void bear::visual::scene_element::set_rendering_attributes
( const bitmap_rendering_attributes& a )
{
  m_elem->set_rendering_attributes(a);
} // scene_element::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the attributes applied to the element when rendering.
 */
const bear::visual::bitmap_rendering_attributes&
bear::visual::scene_element::get_rendering_attributes() const
{
  return m_elem->get_rendering_attributes();
} // scene_element::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the attributes applied to the element when rendering.
 */
bear::visual::bitmap_rendering_attributes&
bear::visual::scene_element::get_rendering_attributes()
{
  return m_elem->get_rendering_attributes();
} // scene_element::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the element must always be displayed.
 */
bool bear::visual::scene_element::always_displayed() const
{
  return m_elem->always_displayed();
} // scene_element::always_displayed()
