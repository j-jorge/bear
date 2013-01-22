/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An object to be passed to layers to filter the items among which the
 *        operations are done.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_FILTER_HPP__
#define __BF_ITEM_FILTER_HPP__

#include "bf/item_filter/base_item_filter.hpp"

#include <claw/smart_ptr.hpp>
#include <string>

namespace bf
{
  class item_instance;

  /**
   * \brief An object to be passed to layers to filter the items among which the
   *        operations are done.
   * \author Julien Jorge
   */
  class item_filter
  {
  private:
    /** \brief The type of the pointer on the effective filter. */
    typedef claw::memory::smart_ptr<base_item_filter> filter_ptr_type;

  public:
    item_filter();
    item_filter( const base_item_filter& filter );

    item_filter copy() const;

    bool evaluate( const item_instance& item ) const;

    std::string get_description() const;

    bool operator==( const item_filter& that ) const;
    bool operator!=( const item_filter& that ) const;

  private:
    /** \brief The filter effectively applied. */
    filter_ptr_type m_filter;

  }; // class item_filter
} // namespace bf

#endif // __BF_ITEM_FILTER_HPP__

