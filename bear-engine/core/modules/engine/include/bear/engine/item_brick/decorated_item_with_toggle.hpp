/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A toggle with an animation for its two states.
 * \author Julien Jorge
 */
#ifndef __ENGINE_DECORATED_ITEM_WITH_TOGGLE_HPP__
#define __ENGINE_DECORATED_ITEM_WITH_TOGGLE_HPP__

#include "bear/engine/scene_visual.hpp"
#include "bear/engine/item_brick/item_with_toggle.hpp"
#include "bear/universe/types.hpp"

#include <list>

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item that can be toggled on or off.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    basic_renderable_item.
     *
     * The custom fields of this class are:
     * - visual_on: animation, the animation played when the item is on
     *   (default = none),
     * - visual_off: animation, the animation played when the item is off
     *   (default = none),
     * - any field supported by the parent classes.
     *
     * \author Julien Jorge
     */
    template<class Base>
    class decorated_item_with_toggle:
      public item_with_toggle<Base>
    {
      /** \brief The type of the parent class. */
      typedef item_with_toggle<Base> super;

    public:
      bool set_animation_field
      ( const std::string& name, const visual::animation& value );

      void get_visual( std::list<scene_visual>& visuals ) const;

    protected:
      void set_toggle_visual_on( const visual::animation& anim );
      void set_toggle_visual_off( const visual::animation& anim );

      void on_toggle_on( base_item* activator );
      void on_toggle_off( base_item* activator );
      void progress_on( universe::time_type elapsed_time );
      void progress_off( universe::time_type elapsed_time );

    private:
      /** \brief The animation played when the item is turned on. */
      visual::animation m_visual_on;

      /** \brief The animation played when the item is turned off. */
      visual::animation m_visual_off;

    }; // class decorated_item_with_toggle
  } // namespace engine
} // namespace bear

#include "bear/engine/item_brick/impl/decorated_item_with_toggle.tpp"

#endif // __ENGINE_DECORATED_ITEM_WITH_TOGGLE_HPP__
