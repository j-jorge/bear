/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::slope class.
 * \author Julien Jorge
 */
#include "bf/slope.hpp"

#include "bf/item_class.hpp"
#include "bf/item_instance.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::slope::slope()
  : steepness(0), left_handle(0, 0), right_handle(0, 0)
{

} // slope::slope()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads the properties of the slope from the fields of an item.
 * \param item The item.
 */
void bf::slope::read_from( const item_instance& item )
{
  steepness = read_item_field( item, "slope.steepness" );
  left_handle.x = read_item_field( item, "slope.control_point.left.x" );
  left_handle.y = read_item_field( item, "slope.control_point.left.y" );
  right_handle.x = read_item_field( item, "slope.control_point.right.x" );
  right_handle.y = read_item_field( item, "slope.control_point.right.y" );
} // slope::read_from()

/*----------------------------------------------------------------------------*/
/**
 * \brief Scales the slopes by a given ratio.
 * \param ratio The ratio.
 */
void bf::slope::scale( double ratio )
{
  steepness *= ratio;
  left_handle *= ratio;
  right_handle *= ratio;
} // slope::scale()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads a real field of an item.
 * \param item The item.
 * \param f The name of the field.
 */
double
bf::slope::read_item_field( const item_instance& item, std::string f ) const
{
  real_type field_value;

  if ( item.has_value( item.get_class().get_field(f) ) )
    {
      item.get_value( f, field_value );
      return field_value.get_value();
    }

  return 0;
} // slope::read_item_field()
