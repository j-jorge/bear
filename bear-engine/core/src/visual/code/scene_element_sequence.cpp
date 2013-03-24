/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_element_sequence.
 * \author Julien Jorge
 */
#include "visual/scene_element_sequence.hpp"

#include "visual/scene_element.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor. The screen coordinates of this element are set to
 *        zero.
 */
bear::visual::scene_element_sequence::scene_element_sequence()
  : base_scene_element(0, 0)
{

} // scene_element_sequence::scene_element_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
bear::visual::base_scene_element*
bear::visual::scene_element_sequence::clone() const
{
  return new scene_element_sequence(*this);
} // scene_element_sequence::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an element at the end of the sequence.
 * \param e The element to insert.
 */
void bear::visual::scene_element_sequence::push_back( const scene_element& e )
{
  m_element.push_back(e);
} // scene_element_sequence::push_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an element at the beginning of the sequence.
 * \param e The element to insert.
 */
void bear::visual::scene_element_sequence::push_front( const scene_element& e )
{
  m_element.push_front(e);
} // scene_element_sequence::push_front()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the element is completely opaque.
 */
bear::visual::rectangle_type
bear::visual::scene_element_sequence::get_opaque_box() const
{
  if ( m_element.empty() || (get_rendering_attributes().get_opacity() != 1) )
    return rectangle_type(0, 0, 0, 0);
  else
    {
      sequence_type::const_iterator it( m_element.begin() );
      rectangle_type result( it->get_opaque_box() );
      ++it;

      for ( ; it!=m_element.end(); ++it )
        {
          const rectangle_type b(it->get_opaque_box());

          if ( b.area() > result.area() )
            result = b;
        }

      coordinate_type left
        ( get_position().x + result.left() * get_scale_factor_x() );
      coordinate_type bottom
        ( get_position().y + result.bottom() * get_scale_factor_y());
      coordinate_type w( result.width() * get_scale_factor_x() );
      coordinate_type h( result.height() * get_scale_factor_y() );

      result.set(left, bottom, left + w, bottom + h);

      return result;
    }
} // scene_element_sequence::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the elements.
 */
bear::visual::rectangle_type
bear::visual::scene_element_sequence::get_bounding_box() const
{
  rectangle_type result(0, 0, 0, 0);

  sequence_type::const_iterator it( m_element.begin() );
  bool initialised(false);

  while ( !initialised && (it!=m_element.end()) )
    {
      const rectangle_type b(it->get_bounding_box());

      if ( (b.width() > 0) && (b.height() > 0) )
        {
          result = b;
          initialised = true;
        }

      ++it;
    }

  while ( it!=m_element.end() )
    {
      const rectangle_type b(it->get_bounding_box());

      if ( (b.width() > 0) && (b.height() > 0) )
        result = result.join(b);

      ++it;
    }

  if (initialised)
    {
      coordinate_type left
        ( get_position().x + result.left() * get_scale_factor_x() );
      coordinate_type bottom
        ( get_position().y + result.bottom() * get_scale_factor_y());
      coordinate_type w( result.width() * get_scale_factor_x() );
      coordinate_type h( result.height() * get_scale_factor_y() );

      result.set(left, bottom, left + w, bottom + h);
    }

  return result;
} // scene_element_sequence::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the sequence in sub sequences.
 * \param boxes The boxes describing how to split the sequence.
 * \param output The resulting element (they are inserted to the back).
 */
void bear::visual::scene_element_sequence::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // scene_element_sequence::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the element_sequence on a screen.
 * \param scr The screen on which we render the element_sequence.
 */
void bear::visual::scene_element_sequence::render( base_screen& scr ) const
{
  sequence_type::const_iterator it;

  for ( it=m_element.begin(); it!=m_element.end(); ++it )
    {
      const double sx( get_scale_factor_x() );
      const double sy( get_scale_factor_y() );
      scene_element e(*it);

      e.get_rendering_attributes().combine(get_rendering_attributes());
      e.set_scale_factor
        ( e.get_scale_factor_x() * sx, e.get_scale_factor_y() * sy );
      e.set_position
        ( get_position().x + e.get_position().x * sx,
          get_position().y + e.get_position().y * sy );

      e.render(scr);
    }
} // scene_element_sequence::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the element must always be displayed.
 */
bool bear::visual::scene_element_sequence::always_displayed() const
{
  sequence_type::const_iterator it;

  for ( it=m_element.begin(); it!=m_element.end(); ++it )
    if ( it->always_displayed() )
      return true;

  return false;
} // scene_element_sequence::always_displayed()
