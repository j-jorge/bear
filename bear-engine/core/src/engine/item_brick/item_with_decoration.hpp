/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item with a decoration (a sprite or an animation).
 * \author Sebastien Angibaud
 */
#ifndef __ENGINE_ITEM_WITH_DECORATION_HPP__
#define __ENGINE_ITEM_WITH_DECORATION_HPP__

#include "engine/base_item.hpp"
#include "engine/scene_visual.hpp"
#include "universe/types.hpp"
#include "visual/animation.hpp"

#include <list>

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item with a decoration (a sprite or an animation).
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    engine::base_item and basic_renderable_item.
     *
     * The custom fields of this class are:
     * - sprite: sprite, the sprite of this item,
     * - animation: animation, the animation of this item,
     * - item_to_mimic: item, an item from which we copy the visuals,
     * - any field supported by the parent classes.
     *
     * \author Sebastien Angibaud
     */
    template<class Base>
    class item_with_decoration:
      public Base
    {
    private:
      typedef Base super;

    public:
      item_with_decoration();

      void build();

      bool set_sprite_field
      ( const std::string& name, const visual::sprite& value );
      bool set_animation_field
      ( const std::string& name, const visual::animation& value );
      bool set_item_field( const std::string& name, base_item* value );
      bool set_bool_field( const std::string& name, bool value );

      void progress( universe::time_type elapsed_time );
      void get_visual( std::list<scene_visual>& visuals ) const;

      void set_animation( const visual::animation& anim );
      void set_sprite( const visual::sprite& spr );
      const visual::animation& get_animation() const;
      visual::sprite get_sprite() const;

      void auto_size();
      void extend_to_bounding_box( bool extend );

    protected:
      visual::animation& get_animation();

    private:
      /** \brief The decoration. */
      visual::animation m_animation;

      /** \brief An item from which we take the sprites. */
      base_item* m_item_to_mimic;

      /** \brief Indicates if the decoration must be extended 
          on the bounding box. */
      bool m_extend_on_bounding_box;

    }; // class item_with_decoration
  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/item_with_decoration.tpp"

#endif // __ENGINE_ITEM_WITH_DECORATION_HPP__
