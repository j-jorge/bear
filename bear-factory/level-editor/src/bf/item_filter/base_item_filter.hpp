/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base object for filters on items.
 * \author Julien Jorge
 */
#ifndef __BF_BASE_ITEM_FILTER_HPP__
#define __BF_BASE_ITEM_FILTER_HPP__

#include <string>

namespace bf
{
  class item_instance;

  /**
   * \brief Base object for filters on items.
   * \author Julien Jorge
   */
  class base_item_filter
  {
  public:
    virtual ~base_item_filter() {}
    virtual base_item_filter* clone() const = 0;
    virtual bool evaluate( const item_instance& item ) const = 0;
    virtual std::string get_description() const = 0;

  }; // class base_item_filter
} // namespace bf

#endif // __BF_BASE_ITEM_FILTER_HPP__

