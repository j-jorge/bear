/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::activable_sides class.
 * \author Julien Jorge
 */
#include "bear/engine/item_brick/activable_sides.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::engine::activable_sides::activable_sides()
  : m_left_side_is_active(false), m_right_side_is_active(false),
    m_top_side_is_active(false), m_bottom_side_is_active(false)
{

} // activable_sides::activable_sides()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the left side is active.
 */
bool bear::engine::activable_sides::left_side_is_active() const
{
  return m_left_side_is_active;
} // activable_sides::left_side_is_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the right side is active.
 */
bool bear::engine::activable_sides::right_side_is_active() const
{
  return m_right_side_is_active;
} // activable_sides::right_side_is_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the top side is active.
 */
bool bear::engine::activable_sides::top_side_is_active() const
{
  return m_top_side_is_active;
} // activable_sides::top_side_is_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bottom side is active.
 */
bool bear::engine::activable_sides::bottom_side_is_active() const
{
  return m_bottom_side_is_active;
} // activable_sides::bottom_side_is_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set left side activation.
 */
void bear::engine::activable_sides::set_left_side_activation(bool a)
{
  m_left_side_is_active = a;
} // activable_sides::set_left_side_activation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set right side activation.
 */
void bear::engine::activable_sides::set_right_side_activation(bool a)
{
  m_right_side_is_active = a;
} // activable_sides::set_right_side_activation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set tom side activation.
 */
void bear::engine::activable_sides::set_top_side_activation(bool a)
{
  m_top_side_is_active = a;
} // activable_sides::set_top_side_activation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set bottom side activation.
 */
void bear::engine::activable_sides::set_bottom_side_activation(bool a)
{
  m_bottom_side_is_active = a;
} // activable_sides::set_top_side_activation()
