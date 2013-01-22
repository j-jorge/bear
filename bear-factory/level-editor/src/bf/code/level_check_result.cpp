/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::level_check_result class.
 * \author Julien Jorge
 */
#include "bf/level_check_result.hpp"

#include "bf/layer.hpp"
#include "bf/layer_check_result.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Check an layer and add the result in the level result.
 * \param lay The layer to check.
 */
void bf::level_check_result::check( layer* lay )
{
  CLAW_PRECOND( lay != NULL );

  layer_check_result result;
  lay->check(result);

  if ( !result.is_ok() )
    m_layers[lay] = result;
} // level_check_result::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the errors.
 */
void bf::level_check_result::clear()
{
  m_layers.clear();
} // level_check_result::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no error in the result.
 */
bool bf::level_check_result::is_ok() const
{
  return m_layers.empty();
} // level_check_result::is_ok()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given item has errors.
 * \param lay The layer containing the item.
 * \param item The item.
 */
bool bf::level_check_result::contains( layer* lay, item_instance* item ) const
{
  layer_result_map_type::const_iterator it = m_layers.find(lay);

  if ( it == m_layers.end() )
    return false;
  else
    return it->second.contains(item);
} // level_result_map_type::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the check results of the layers.
 */
bf::level_check_result::layer_iterator
bf::level_check_result::layer_begin() const
{
  return m_layers.begin();
} // level_check_result::layer_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the check results of the layers.
 */
bf::level_check_result::layer_iterator bf::level_check_result::layer_end() const
{
  return m_layers.end();
} // level_check_result::layer_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the result of the check of a given layer.
 * \param lay The layer.
 */
const bf::layer_check_result&
bf::level_check_result::get_layer_result( layer* lay ) const
{
  CLAW_PRECOND( m_layers.find(lay) != m_layers.end() );

  return m_layers.find(lay)->second;
} // level_check_result::get_layer_result()
