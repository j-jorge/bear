/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that stores some informations on a collision to be used in an
 *        expression.
 * \author Julien Jorge
 */
#ifndef __ENGINE_COLLISION_IN_EXPRESSION_HPP__
#define __ENGINE_COLLISION_IN_EXPRESSION_HPP__

#include "bear/universe/collision_info.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief A class that stores some informations on a collision to be used in
     *        an expression.
     *
     * The class stores pointers on the data stored by engine::with_trigger. So,
     * this latter class can change its data and the values are automatically
     * changed in collision_in_expression.
     *
     * Moreover, this class can be used as a kind of pointer on the colliding
     * base_item, thanks to operator overloading.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT collision_in_expression
    {
    public:
      typedef base_item const* const_item_pointer;
      typedef universe::collision_info const* const_collision_info_pointer;

    public:
      collision_in_expression();
      collision_in_expression
      ( const_item_pointer const* item,
        const_collision_info_pointer const* info );

      bool is_valid() const;

      const base_item* get_item_ptr() const;

      const base_item* operator->() const;
      const base_item& operator*() const;
      bool operator==( base_item const* that ) const;
      bool operator!=( base_item const* that ) const;

    private:
      /** \brief The item in collision. */
      const_item_pointer const* m_colliding_item;

      /** \brief The informations about the collision. */
      const_collision_info_pointer const* m_collision_info;

    }; // class collision_in_expression

  } // namespace engine
} // namespace bear

#endif // __ENGINE_COLLISION_IN_EXPRESSION_HPP__
