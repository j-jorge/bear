/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that have friction on its sides.
 * \author Sebastien Angibaud
 */
#ifndef __ENGINE_ITEM_WITH_FRICTION_HPP__
#define __ENGINE_ITEM_WITH_FRICTION_HPP__

#include "bear/engine/base_item.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item that have friction its sides.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    engine::base_item,
     *
     * The custom fields of this class are:
     * - left_friction: real, the friction applied to the items in contact
     *   on the left (default = 1).
     * - right_friction: real, the friction applied to the items in contact
     *   on the right (default = 1).
     * - top_friction: real, the friction applied to the items in contact
     *   on the top (default = 1).
     * - bottom_friction: real, the friction applied to the items in contact
     *   on the bottom (default = 1).

     *
     * \author Sebastien Angibaud
     */
    template<class Base>
    class item_with_friction:
      public Base
    {
      typedef Base super;

    public:
      item_with_friction();

      bool set_real_field( const std::string& name, double value );

      double get_left_friction() const;
      void set_left_friction( double f );

      double get_right_friction() const;
      void set_right_friction( double f );

      double get_top_friction() const;
      void set_top_friction( double f );

      double get_bottom_friction() const;
      void set_bottom_friction( double f );

    private:
    /** \brief The friction applied to the item colliding on the left. */
    double m_left_friction;

    /** \brief The friction applied to the item colliding on the right. */
    double m_right_friction;

    /** \brief The friction applied to the item colliding on the top. */
    double m_top_friction;

    /** \brief The friction applied to the item colliding on the bottom. */
    double m_bottom_friction;

    }; // class item_with_friction
  } // namespace engine
} // namespace bear

#include "bear/engine/item_brick/impl/item_with_friction.tpp"

#endif // __ENGINE_ITEM_WITH_FRICTION_HPP__
