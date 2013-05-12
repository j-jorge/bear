/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that check if the z-position of the other item is in a given
 *        interval before aligning it.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_WITH_RESTRICTED_Z_COLLISION_HPP__
#define __ENGINE_ITEM_WITH_RESTRICTED_Z_COLLISION_HPP__

#include "engine/base_item.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item that check if the z-position of the other item is in a
     *        given interval before aligning it.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    engine::base_item,
     *
     * The custom fields of this class are:
     * - min_z_for_collision: int, minimum z-position of the other item to align
     *   it (default = -inf.),
     * - max_z_for_collision: int, maximum z-position of the other item to align
     *   it (default = -inf.).
     *
     * \author Julien Jorge
     */
    template<class Base>
    class item_with_restricted_z_collision:
      public Base
    {
      typedef Base super;

    public:
      item_with_restricted_z_collision();

      bool set_integer_field( const std::string& name, int value );

      void set_min_z_for_collision( int z );
      void set_max_z_for_collision( int z );

    protected:
      bool satisfy_collision_condition( const base_item& that ) const;

    private:
      /** \brief Minimum z-position to be aligned. */
      int m_min_z;

      /** \brief Maximum z-position to be aligned. */
      int m_max_z;

    }; // class item_with_restricted_z_collision
  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/item_with_restricted_z_collision.tpp"

#endif // __ENGINE_ITEM_WITH_RESTRICTED_Z_COLLISION_HPP__
