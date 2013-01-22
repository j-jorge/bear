/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_filter class.
 * \author Julien Jorge
 */
#include "bf/item_filter/item_filter.hpp"

#include "bf/item_filter/base_item_filter.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::item_filter::item_filter()
  : m_filter(NULL)
{

} // item_filter::item_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param filter The filter effectively evaluated.
 */
bf::item_filter::item_filter( const base_item_filter& filter )
  : m_filter(filter.clone())
{

} // item_filter::item_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of the filter.
 */
bf::item_filter bf::item_filter::copy() const
{
  if ( m_filter == NULL )
    return item_filter();
  else
    return item_filter(*m_filter);
} // item_filter::copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the filter on an item.
 * \param item The item on which the filter is evaluated.
 */
bool bf::item_filter::evaluate( const item_instance& item ) const
{
  if ( m_filter == NULL )
    return true;
  else
    return m_filter->evaluate(item);
} // item_filter::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a description of the filter.
 */
std::string bf::item_filter::get_description() const
{
  if ( m_filter == NULL )
    return "NULL filter";
  else
    return m_filter->get_description();
} // item_filter::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compares with an other filter for equality.
 * \param that The filter with which the comparison is done.
 *
 * \return true if both filters point to the same instance of base_item_filter.
 */
bool bf::item_filter::operator==( const item_filter& that ) const
{
  return m_filter == that.m_filter;
} // item_filter::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compares with an other filter for disequality.
 * \param that The filter with which the comparison is done.
 */
bool bf::item_filter::operator!=( const item_filter& that ) const
{
  return !(*this == that);
} // item_filter::operator!=()
