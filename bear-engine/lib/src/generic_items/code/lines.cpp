/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::lines class.
 * \author Julien Jorge
 */
#include "generic_items/lines.hpp"

#include "generic_items/line.hpp"

BASE_ITEM_EXPORT(lines, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::lines::lines()
: m_width(1)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // lines::lines()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::lines::build()
{
  super::build();

  if ( !m_points.empty() )
    {
      point_list_type::const_iterator it=m_points.begin();
      point_list_type::const_iterator it_next(it);

      for ( ++it_next; it_next!=m_points.end(); ++it, ++it_next )
        {
          line* new_line = new line(*this);

          new_line->push_back(*it);
          new_line->push_back(*it_next);
          new_line->set_line_width(m_width);

          if ( (*it)->get_insert_as_static()
               && (*it_next)->get_insert_as_static() )
            new_line->set_insert_as_static();

          new_item(*new_line);
        }
    }

  kill();
} // lines::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::lines::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if ( name == "lines.width" )
    m_width = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // lines::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "list of items".
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::lines::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result(true);

  if ( name == "lines.ends" )
    m_points = point_list_type(value.begin(), value.end());
  else
    result = super::set_item_list_field(name, value);

  return result;
} // lines::set_item_list_field()
