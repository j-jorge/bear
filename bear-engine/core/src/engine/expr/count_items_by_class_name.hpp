/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An expression that returns the status of a toggle.
 * \author Julien Jorge
 */
#ifndef __ENGINE_COUNT_ITEMS_BY_CLASS_NAME_HPP__
#define __ENGINE_COUNT_ITEMS_BY_CLASS_NAME_HPP__

#include "engine/base_item.hpp"
#include "expr/base_linear_expression.hpp"

#include "universe/derived_item_handle.hpp"

#include "engine/class_export.hpp"

#include <string>

namespace bear
{
  namespace engine
  {

    /**
     * \brief An expression that returns the number of items of a given class in
     *        the world.
     *
     * \remark This expression does not check the parent classes.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT count_items_by_class_name:
      public expr::base_linear_expression
    {
    private:
      /** \brief The type of the handle on an item that will give us the
          world. */
      typedef universe::const_derived_item_handle<const base_item> item_handle;

    public:
      count_items_by_class_name();
      count_items_by_class_name( const base_item& w, const std::string& c );

      void set_world_through( const base_item& w );
      void set_class_name( const std::string& n );

      expr::base_linear_expression* clone() const;
      double evaluate() const;

    private:
      /** \brief An item in the world in which the items are searched. */
      item_handle m_world_proxy;

      /** \brief The name of the class of the items to count. */
      std::string m_class_name;

    }; // class count_items_by_class_name

  } // namespace engine
} // namespace bear

#endif // __ENGINE_COUNT_ITEMS_BY_CLASS_NAME_HPP__
