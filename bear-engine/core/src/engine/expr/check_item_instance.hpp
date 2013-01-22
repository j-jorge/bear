/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An expression that checks if a colliding item is a given item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CHECK_ITEM_INSTANCE_HPP__
#define __ENGINE_CHECK_ITEM_INSTANCE_HPP__

#include "expr/base_boolean_expression.hpp"
#include "engine/expr/collision_in_expression.hpp"

#include "universe/const_item_handle.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An expression that checks if a colliding item is a given item.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT check_item_instance:
      public expr::base_boolean_expression
    {
    public:
      /** \brief The type of the handles on the item in the expression. */
      typedef universe::const_item_handle handle_type;

    public:
      void set_instance( handle_type item );
      handle_type get_instance() const;

      void set_collision_data( const collision_in_expression& data );
      const collision_in_expression& get_collision_data() const;

      expr::base_boolean_expression* clone() const;
      bool evaluate() const;

    private:
      /** \brief The instance to check. */
      handle_type m_instance;

      /** \brief The data on the colliding item. */
      collision_in_expression m_collision;

    }; // class check_item_instance

  } // namespace engine
} // namespace bear

#endif // __ENGINE_CHECK_ITEM_INSTANCE_HPP__
