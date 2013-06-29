/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::item_picking_filter class.
 * \author Julien Jorge
 */
#include "universe/item_picking_filter.hpp"

#include "universe/physical_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::item_picking_filter::item_picking_filter()
  : m_check_artificial(false), m_check_phantom(false),
    m_check_can_move_items(false), m_check_fixed(false),
    m_check_forbidden_position(false)
{

} // item_picking_filter::item_picking_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::item_picking_filter::~item_picking_filter()
{

} // item_picking_filter::~item_picking_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item satisfies the condition of the filter.
 * \param item The item to check.
 */
bool bear::universe::item_picking_filter::satisfies_condition
( const physical_item& item ) const
{
  return (!m_check_artificial || (item.is_artificial() == m_artificial_value))
    && (!m_check_phantom || (item.is_phantom() == m_phantom_value))
    && (!m_check_can_move_items
        || (item.can_move_items() == m_can_move_items_value))
    && (!m_check_fixed || (item.is_fixed() == m_fixed_value))
    && (!m_check_forbidden_position ||
        !item.get_bounding_box().includes(m_forbidden_position) )
    && do_satisfies_condition(item);
} // item_picking_filter::satisfies_condition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the flag "artificial" for the selected items.
 * \param v The value this flag must have.
 */
void bear::universe::item_picking_filter::set_artificial_value( bool v )
{
  m_check_artificial = true;
  m_artificial_value = v;
} // item_picking_filter::set_artificial_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the flag "phantom" for the selected items.
 * \param v The value this flag must have.
 */
void bear::universe::item_picking_filter::set_phantom_value( bool v )
{
  m_check_phantom = true;
  m_phantom_value = v;
} // item_picking_filter::set_phantom_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the flag "can_move_items" for the selected items.
 * \param v The value this flag must have.
 */
void bear::universe::item_picking_filter::set_can_move_items_value( bool v )
{
  m_check_can_move_items = true;
  m_can_move_items_value = v;
} // item_picking_filter::set_can_move_items_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the flag "fixed" for the selected items.
 * \param v The value this flag must have.
 */
void bear::universe::item_picking_filter::set_fixed_value( bool v )
{
  m_check_fixed = true;
  m_fixed_value = v;
} // item_picking_filter::set_fixed_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a position where the item must not be.
 * \param pos The position of the origin.
 */
void bear::universe::item_picking_filter::set_forbidden_position
( const position_type& pos )
{
  m_check_forbidden_position = true;
  m_forbidden_position = pos;
} // item_picking_filter::set_forbidden_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Overridden condition checking.
 * \param The item to check
 */
bool bear::universe::item_picking_filter::do_satisfies_condition
( const physical_item& item ) const
{
  return true;
} // item_picking_filter::do_satisfies_condition()

