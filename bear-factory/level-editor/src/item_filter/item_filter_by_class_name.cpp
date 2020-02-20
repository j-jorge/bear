/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_filter_by_class_name class.
 * \author Julien Jorge
 */
#include "bf/item_filter/item_filter_by_class_name.hpp"

#include "bf/item_instance.hpp"
#include "bf/item_class.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param class_name The name of the class of which the item must be an
 *        instance.
 */
bf::item_filter_by_class_name::item_filter_by_class_name
( const std::string& class_name )
  : m_class_name(class_name)
{

} // item_filter_by_class_name::item_filter_by_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this instance.
 */
bf::item_filter_by_class_name* bf::item_filter_by_class_name::clone() const
{
  return new item_filter_by_class_name(*this);
} // item_filter_by_class_name::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the filter on an item.
 * \param item The item on which the filter is evaluated.
 */
bool bf::item_filter_by_class_name::evaluate( const item_instance& item ) const
{
  return (item.get_class().get_class_name() == m_class_name)
  || item.get_class().inherits_from(m_class_name);
} // item_filter_by_class_name::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a description of the filter.
 */
std::string bf::item_filter_by_class_name::get_description() const
{
  return "class=" + m_class_name;
} // item_filter_by_class_name::get_description()
