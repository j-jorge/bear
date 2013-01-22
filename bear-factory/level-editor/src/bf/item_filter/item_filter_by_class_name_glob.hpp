/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A filter on items to check the item's class name matches a pattern.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_FILTER_BY_CLASS_NAME_GLOB_HPP__
#define __BF_ITEM_FILTER_BY_CLASS_NAME_GLOB_HPP__

#include "bf/item_filter/base_item_filter.hpp"

#include <string>

namespace bf
{
  class item_class;

  /**
   * \brief A filter on items to check the item's class name matches a pattern.
   * \author Julien Jorge
   */
  class item_filter_by_class_name_glob:
    public base_item_filter
  {
  public:
    explicit item_filter_by_class_name_glob( const std::string& pattern );

    virtual item_filter_by_class_name_glob* clone() const;
    virtual bool evaluate( const item_instance& item ) const;
    virtual std::string get_description() const;

  private:
    bool evaluate_class( const item_class& c ) const;

  private:
    /** \brief The pattern to be matched by the name of the class of which the
        item must be an instance. */
    const std::string m_class_name_pattern;

  }; // class item_filter_by_class_name_glob
} // namespace bf

#endif // __BF_ITEM_FILTER_BY_CLASS_NAME_GLOB_HPP__

