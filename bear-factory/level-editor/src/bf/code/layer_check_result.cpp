/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::layer_check_result class.
 * \author Julien Jorge
 */
#include "bf/layer_check_result.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Check an item and add the result in the layer result.
 * \param item The item to check.
 * \param map_id All the item identifiers.
 */
void bf::layer_check_result::check
( item_instance* item, const std::set<std::string>& map_id )
{
  CLAW_PRECOND( item != NULL );

  if ( m_items.find(item) != m_items.end() )
    item->check( m_items[item], map_id );
  else
    {
      item_check_result result;
      item->check(result, map_id);

      if ( !result.is_ok() )
        m_items[item] = result;
    }
} // layer_check_result::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an error on an item of the layer.
 * \param item The item.
 * \param e The error.
 */
void bf::layer_check_result::add
( item_instance* item, const check_error& e )
{
  m_items[item].add(e);
} // layer_check_result::add()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no error in the result.
 */
bool bf::layer_check_result::is_ok() const
{
  return m_items.empty();
} // layer_check_result::is_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given item has errors.
 * \param item The item.
 */
bool bf::layer_check_result::contains( item_instance* item ) const
{
  return m_items.find(item) != m_items.end();
} // layer_result_map_type::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the check results of the items.
 */
bf::layer_check_result::item_iterator bf::layer_check_result::item_begin() const
{
  return m_items.begin();
} // layer_check_result::item_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the check results of the items.
 */
bf::layer_check_result::item_iterator bf::layer_check_result::item_end() const
{
  return m_items.end();
} // layer_check_result::item_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the result of the check of a given item.
 * \param item The item.
 */
const bf::item_check_result&
bf::layer_check_result::get_item_result( item_instance* item ) const
{
  CLAW_PRECOND( m_items.find(item) != m_items.end() );

  return m_items.find(item)->second;
} // layer_check_result::get_item_result()
