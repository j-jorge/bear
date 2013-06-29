/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::scene_element class.
 * \author Julien Jorge
 */
#include "gui/scene_element.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param e The element to display.
 */
bear::gui::scene_element::scene_element( const visual::scene_element& e )
  : visual_component(), m_element(e)
{
  set_size( e.get_element_width(), e.get_element_height() );
} // scene_element::scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the displayed scene_element.
 * \param e The scene_element to display.
 */
void
bear::gui::scene_element::set_scene_element( const visual::scene_element& e )
{
  m_element = e;
  stretch_element();
} // scene_element::set_scene_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::scene_element::display
( std::list<visual::scene_element>& e ) const
{
  visual::scene_element elem(m_element);
  elem.set_position(left(), bottom());
  e.push_back(elem);
} // scene_element::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the component has been resized.
 */
void bear::gui::scene_element::on_resized()
{
  visual_component::on_resized();

  stretch_element();
} // visual_component::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make the scene element to fit the component
 */
void bear::gui::scene_element::stretch_element()
{
  const double rx = width() / m_element.get_element_width();
  const double ry = height() / m_element.get_element_height();

  m_element.set_scale_factor( std::min(rx, ry) );

  m_element.set_position
    ( (width() - m_element.get_width()) / 2,
      (height() - m_element.get_height()) / 2 );
} // visual_component::stretch_element()
