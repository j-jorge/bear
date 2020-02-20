/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_filter_by_class_name_glob class.
 * \author Julien Jorge
 */
#include "bf/item_filter/item_filter_by_class_name_glob.hpp"

#include "bf/item_instance.hpp"
#include "bf/item_class.hpp"

#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pattern The pattern that must be matched by the name of the class of
 *        which the item must be an instance.
 */
bf::item_filter_by_class_name_glob::item_filter_by_class_name_glob
( const std::string& pattern )
  : m_class_name_pattern(pattern)
{

} // item_filter_by_class_name_glob::item_filter_by_class_name_glob()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this instance.
 */
bf::item_filter_by_class_name_glob*
bf::item_filter_by_class_name_glob::clone() const
{
  return new item_filter_by_class_name_glob(*this);
} // item_filter_by_class_name_glob::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the filter on an item.
 * \param item The item on which the filter is evaluated.
 */
bool
bf::item_filter_by_class_name_glob::evaluate( const item_instance& item ) const
{
  return evaluate_class(item.get_class());
} // item_filter_by_class_name_glob::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a description of the filter.
 */
std::string bf::item_filter_by_class_name_glob::get_description() const
{
  return "class~" + m_class_name_pattern;
} // item_filter_by_class_name_glob::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the filter on a class.
 * \param c The class to check.
 */
bool
bf::item_filter_by_class_name_glob::evaluate_class( const item_class& c ) const
{
  bool result =
    claw::text::glob_match( m_class_name_pattern, c.get_class_name() );
  item_class::const_super_class_iterator it;

  for ( it=c.super_class_begin(); !result && (it!=c.super_class_end()); ++it )
    result = evaluate_class(*it);

  return result;
} // item_filter_by_class_name_glob::evaluate_class()
