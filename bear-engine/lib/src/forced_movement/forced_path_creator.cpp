/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_path_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/forced_movement/forced_path_creator.hpp"

#include "bear/universe/forced_movement/forced_join.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param mvt The movement loaded by this loader.
 */
bear::forced_path_creator::loader::loader( universe::forced_sequence& mvt )
  : super("forced_path_creator"), m_movement(mvt), m_speed(1)
{

} // forced_path_creator::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::forced_path_creator::loader*
bear::forced_path_creator::loader::clone() const
{
  return new loader( *this );
} // forced_path_creator::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::forced_path_creator::loader::set_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "loops" )
    m_movement.set_loops(value);
  else
    result = super::set_field(name, value);

  return result;
} // forced_path_creator::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::forced_path_creator::loader::set_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "speed" )
    m_speed = value;
  else
    result = super::set_field(name, value);

  return result;
} // forced_path_creator::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c list of items.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::forced_path_creator::loader::set_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "path" )
    result = build_sequence( value );
  else
    result = super::set_field(name, value);

  return result;
} // forced_path_creator::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds the movements in the sequence.
 * \param items The items to join in the sequence.
 * \return true if the movement has been built, false otherwise.
 */
bool bear::forced_path_creator::loader::build_sequence
( const std::vector<engine::base_item*>& items )
{
  if ( m_speed <= 0 )
    {
      claw::logger << claw::log_error
                   << "forced_path_creator: Cannot build a movement with a non "
        "positive speed."
                   << std::endl;
      return false;
    }
  
  if ( items.empty() )
    {
      claw::logger << claw::log_error
                   << "forced_path_creator: Cannot build a sequence with no "
        "item."
                   << std::endl;
      return false;
    }

  if ( items[0] == NULL )
    {
      claw::logger << claw::log_error
                   << "forced_path_creator: Cannot build a sequence without an "
        "initial item."
                   << std::endl;
      return false;
    }

  add_movement_in_sequence( *items[0], 0 );
  universe::position_type previous_position( items[0]->get_center_of_mass() );

  for ( std::size_t i(1); i!=items.size(); ++i )
    if ( items[i] == NULL )
      claw::logger << claw::log_warning << "forced_path_creator: item #"
                   << i << " is NULL." << std::endl;
    else
      {
        const universe::position_type new_position
          ( items[i]->get_center_of_mass() );

        add_movement_in_sequence
          ( *items[i], new_position.distance( previous_position ) / m_speed );
        previous_position = new_position;
      }

  return true;
} // forced_path_creator::loader::build_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Appends a movement to reach a given item in the sequence.
 * \param item The item to join.
 * \param duration The duration of the movement.
 */
void bear::forced_path_creator::loader::add_movement_in_sequence
( engine::base_item& item, universe::time_type duration )
{
  universe::forced_join mvt( duration );
  mvt.set_reference_point_on_center( item );
  
  m_movement.push_back( mvt );
} // forced_path_creator::loader::add_movement_in_sequence()




BASE_ITEM_EXPORT( forced_path_creator, bear )


/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the item.
 */
void bear::forced_path_creator::build()
{
  set_forced_movement(m_movement);

  kill();
} // forced_path_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::forced_path_creator::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader(m_movement) );
} // forced_path_creator::populate_loader_map()
