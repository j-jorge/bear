/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::linear_movement_sequence_loop class.
 * \author Sébastien Angibaud
 */
#include "bear/generic_items/linear_movement_sequence_loop.hpp"

#include "bear/universe/forced_movement/forced_sequence.hpp"
#include "bear/universe/forced_movement/forced_goto.hpp"
#include "bear/universe/forced_movement/forced_join.hpp"

BASE_ITEM_EXPORT( linear_movement_sequence_loop, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::linear_movement_sequence_loop::linear_movement_sequence_loop()
  : m_item(NULL), m_duration(1), m_x_length(0) ,  m_y_length(0),
    m_auto_angle(false)
{

} // linear_movement_sequence_loop::linear_movement_sequence_loop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::linear_movement_sequence_loop::build()
{
  super::build();

  create_items();
  m_item->kill();
} // linear_movement_sequence_loop::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field of type double.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::linear_movement_sequence_loop::set_real_field
( const std::string& name, const double value)
{
  bool result(true);

  if ( name == "linear_movement_sequence_loop.duration" )
    m_duration = value;
  else if ( name == "linear_movement_sequence_loop.length.x" )
    m_x_length = value;
  else if ( name == "linear_movement_sequence_loop.length.y" )
    m_y_length = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // linear_movement_sequence_loop::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::linear_movement_sequence_loop::set_real_list_field
( const std::string& name, const std::vector<double>& value )
{
   bool ok = true;

  if (name == "linear_movement_sequence_loop.positions")
    m_positions = value;
  else
    ok = super::set_real_list_field(name, value);

  return ok;
} // linear_movement_sequence_loop::set_real_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field of type item.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::linear_movement_sequence_loop::set_item_field
( const std::string& name, bear::engine::base_item* value)
{
  bool result(true);

  if ( name == "linear_movement_sequence_loop.item" )
    m_item = value;
  else
    result = super::set_item_field(name, value);

  return result;
} // linear_movement_sequence_loop::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::linear_movement_sequence_loop::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "linear_movement_sequence_loop.auto_angle" )
    m_auto_angle = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // linear_movement_sequence_loop::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all fields have a correct value.
 */
bool bear::linear_movement_sequence_loop::is_valid() const
{
  return ( !m_positions.empty() )
    && ( m_item != NULL )
    && super::is_valid();
} // linear_movement_sequence_loop::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create items.
 */
void bear::linear_movement_sequence_loop::create_items()
{
  for ( unsigned int i = 0; i != m_positions.size(); ++i )
    {
      base_item* item;

      item = m_item->clone();
      item->set_horizontal_middle
        ( get_horizontal_middle() + m_positions[i]*m_x_length/m_duration);
      item->set_vertical_middle
        ( get_vertical_middle() + m_positions[i]*m_y_length/m_duration);

      new_item( *item );

      bear::universe::forced_sequence movement;

      bear::universe::forced_goto mvt_1;
      mvt_1.set_total_time(m_duration-m_positions[i]);
      mvt_1.set_acceleration_time(0);
      mvt_1.set_x_length((m_duration-m_positions[i])*m_x_length/m_duration);
      mvt_1.set_y_length((m_duration-m_positions[i])*m_y_length/m_duration);
      mvt_1.set_auto_angle(m_auto_angle);

      bear::universe::forced_join mvt_2;
      mvt_2.set_total_time(0);
      mvt_2.set_reference_point_on_center(*this);
      mvt_2.set_auto_angle(m_auto_angle);

      bear::universe::forced_goto mvt_3;
      mvt_3.set_total_time(m_positions[i]);
      mvt_3.set_acceleration_time(0);
      mvt_3.set_x_length(m_positions[i]*m_x_length/m_duration);
      mvt_3.set_y_length(m_positions[i]*m_y_length/m_duration);
      mvt_3.set_auto_angle(m_auto_angle);

      movement.push_back(mvt_1);
      movement.push_back(mvt_2);
      movement.push_back(mvt_3);
      movement.set_item(*item);

      item->set_forced_movement( movement );
    }
} // linear_movement_sequence_loop::create_items()
