/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An expression that checks if a colliding item inherit from a given
 *        class.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CHECK_ITEM_CLASS_HIERARCHY_HPP__
#define __ENGINE_CHECK_ITEM_CLASS_HIERARCHY_HPP__

#include "expr/base_boolean_expression.hpp"
#include "engine/expr/collision_in_expression.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An expression that checks if a colliding item inherit from a given
     *        class.
     *
     * \b Template \b arguments
     * - \a T the type from which the colliding item must inherit.
     *
     * \sa check_item_class
     * \author Julien Jorge
     */
    template<typename T>
    class check_item_class_hierarchy:
      public expr::base_boolean_expression
    {
    public:
      void set_collision_data( const collision_in_expression& data );
      const collision_in_expression& get_collision_data() const;

      expr::base_boolean_expression* clone() const;
      bool evaluate() const;

    private:
      /** \brief The data on the colliding item. */
      collision_in_expression m_collision;

    }; // class check_item_class_hierarchy

  } // namespace engine
} // namespace bear

#include "engine/expr/impl/check_item_class_hierarchy.tpp"

#endif // __ENGINE_CHECK_ITEM_CLASS_HIERARCHY_HPP__
