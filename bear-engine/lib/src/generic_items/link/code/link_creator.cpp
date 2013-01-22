/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::link_creator class.
 * \author Julien Jorge
 */
#include "generic_items/link/link_creator.hpp"

#include "universe/link/link.hpp"

#include <climits>
#include <limits>

#include "engine/export.hpp"

BASE_ITEM_EXPORT( link_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::link_creator::link_creator()
  : m_strength(1), m_minimal_length(0),
    m_maximal_length
  (std::numeric_limits<universe::coordinate_type>::infinity()),
    m_first_item(NULL), m_second_item(NULL)
{

} // link_creator::link_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::link_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "link_creator.strength" )
    m_strength = value;
  else if ( name == "link_creator.length.minimal" )
    m_minimal_length = value;
  else if ( name == "link_creator.length.maximal" )
    m_maximal_length = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // link_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::link_creator::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( (name == "link_creator.first_item") && (value != NULL) )
    m_first_item = value;
  else if ( (name == "link_creator.second_item") && (value != NULL) )
    m_second_item = value;
  else
    result = super::set_item_field(name, value);

  return result;
} // link_creator::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::link_creator::is_valid() const
{
  return (m_first_item!=NULL) && (m_second_item!=NULL) && super::is_valid();
} // link_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::link_creator::build()
{
  new universe::link( *m_first_item, *m_second_item, m_strength,
                      m_minimal_length, m_maximal_length );

  kill();
} // link_creator::build()
