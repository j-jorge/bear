/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_sequence_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/forced_movement/forced_sequence_creator.hpp"

#include "bear/engine/export.hpp"

#include <claw/logger.hpp>

BASE_ITEM_EXPORT( forced_sequence_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "unsigned int".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_sequence_creator::set_u_integer_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "forced_sequence_creator.loops" )
    m_movement.set_loops(value);
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // forced_sequence_creator::set_u_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "list of item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_sequence_creator::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "forced_sequence_creator.movements" )
    {
      for (unsigned int i=0; result && (i!=value.size()); ++i)
        if ( value[i] == NULL )
          claw::logger << claw::log_warning << "forced_sequence_creator: item #"
                       << i << " is NULL." << std::endl;
        else if ( value[i]->has_forced_movement() )
          m_movement.push_back( value[i]->get_forced_movement() );
        else
          claw::logger << claw::log_warning << "forced_sequence_creator: item #"
                       << i << " has no movement." << std::endl;
    }
  else
    result = super::set_item_list_field(name, value);

  return result;
} // forced_sequence_creator::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the item.
 */
void bear::forced_sequence_creator::build()
{
  set_forced_movement(m_movement);

  kill();
} // forced_sequence_creator::build()
