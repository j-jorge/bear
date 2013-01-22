/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that can set target of forced_movement.
 * \author Sebastien Angibaud
 */
#ifndef __ENGINE_FORCED_MOVEMENT_CREATOR_WITH_REFERENCE_HPP__
#define __ENGINE_FORCED_MOVEMENT_CREATOR_WITH_REFERENCE_HPP__

#include "engine/base_item.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item that can set target of forced_movement.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    engine::base_item,
     *
     * The custom fields of this class are:
     * - ratio.x: real, the ratio of width to use
     * (0 for left side, 0.5 for middle, 1 for right side). (default = 0.5)
     * - ratio.y: real, the ratio of height to use
     * (0 for bottom side, 0.5 for middle, 1 for top side). (default = 0.5)
     * - gap.x: real, The x-gap to applied. (default = 0)
     * - gap.y: real, The y-gap to applied. (default = 0)
     * \author Sebastien Angibaud
     */
    template<class Base, class MovementType>
    class forced_movement_creator_with_reference:
      public Base
    {
      typedef Base super;
      typedef MovementType forced_movement_type;

    public:
      forced_movement_creator_with_reference();

      bool set_real_field( const std::string& name, double value );
      bool set_item_field( const std::string& name, engine::base_item* value );
      bool is_valid() const;

      void build();

      forced_movement_type& get_movement();

    private:
      /** \brief The movement finally applied. */
      forced_movement_type m_movement;

      /* \brief The ratio applied to compute reference point. */
      universe::position_type m_ratio;

       /* \brief The gap applied to compute reference point. */
      universe::position_type m_gap;
    }; // class forced_movement_creator_with_reference
  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/forced_movement_creator_with_reference.tpp"

#endif // __ENGINE_FORCED_MOVEMENT_CREATOR_WITH_REFERENCE_HPP__
