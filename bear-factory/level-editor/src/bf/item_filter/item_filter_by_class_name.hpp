/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A filter on items to check the item's class name.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_FILTER_BY_CLASS_NAME_HPP__
#define __BF_ITEM_FILTER_BY_CLASS_NAME_HPP__

#include "bf/item_filter/base_item_filter.hpp"

#include <string>

namespace bf
{
  /**
   * \brief A filter on items to check the item's class name.
   * \author Julien Jorge
   */
  class item_filter_by_class_name:
    public base_item_filter
  {
  public:
    explicit item_filter_by_class_name( const std::string& class_name );

    virtual item_filter_by_class_name* clone() const;
    virtual bool evaluate( const item_instance& item ) const;
    virtual std::string get_description() const;

  private:
    /** \brief The name of the class of which the item must be an instance. */
    const std::string m_class_name;

  }; // class item_filter_by_class_name
} // namespace bf

#endif // __BF_ITEM_FILTER_BY_CLASS_NAME_HPP__

