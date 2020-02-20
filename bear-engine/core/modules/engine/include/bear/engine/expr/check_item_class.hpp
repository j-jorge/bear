/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An expression that checks if a colliding item is an instance of a
 *        given class.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CHECK_ITEM_CLASS_HPP__
#define __ENGINE_CHECK_ITEM_CLASS_HPP__

#include "bear/expr/base_boolean_expression.hpp"
#include "bear/engine/expr/collision_in_expression.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An expression that checks if a colliding item is an instance of a
     *        given class.
     *
     * \remark This expression does not check the parent classes.
     * \sa check_item_class_hierarchy
     * \author Julien Jorge
     */
    class ENGINE_EXPORT check_item_class:
      public expr::base_boolean_expression
    {
    public:
      void set_class_name( const std::string& class_name );
      const std::string& get_class_name() const;

      void set_collision_data( const collision_in_expression& data );
      const collision_in_expression& get_collision_data() const;

      expr::base_boolean_expression* clone() const;
      bool evaluate() const;

      std::string formatted_string() const;

    private:
      /** \brief The name of the class. */
      std::string m_class_name;

      /** \brief The data on the colliding item. */
      collision_in_expression m_collision;

    }; // class check_item_class

  } // namespace engine
} // namespace bear

#endif // __ENGINE_CHECK_ITEM_CLASS_HPP__
