/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A renderable item.
 * \author Sebastien Angibaud
 */
#ifndef __ENGINE_BASIC_RENDERABLE_ITEM_HPP__
#define __ENGINE_BASIC_RENDERABLE_ITEM_HPP__

#include "engine/scene_visual.hpp"
#include "engine/item_brick/with_rendering_attributes.hpp"
#include "visual/sprite_sequence.hpp"

#include <list>
#include <string>
#include <claw/coordinate_2d.hpp>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A renderable item.
     *
     * \b template \b parameters :
     * - \a Base : the base class for this item. Must inherit from
     *    engine::base_item,
     *
     * \author Sebastien Angibaud
     */
    template<class Base>
    class basic_renderable_item:
      public Base,
      public with_rendering_attributes
    {
      typedef Base super;

    public:
      basic_renderable_item();

      void on_enters_layer();
      void progress( bear::universe::time_type elapsed_time );

      bool set_real_field( const std::string& name, double value );
      bool set_integer_field( const std::string& name, int value );
      bool set_bool_field( const std::string& name, bool value );
      bool set_color_field
      ( const std::string& name, visual::color value );

      const visual::position_type& get_gap() const;
      visual::position_type get_oriented_gap
      ( visual::coordinate_type w, visual::coordinate_type h ) const;
      void set_gap( const visual::position_type& gap );
      void set_gap( visual::coordinate_type x, visual::coordinate_type y );
      void set_gap_x( visual::coordinate_type x );
      void set_gap_y( visual::coordinate_type y );

      void set_auto_mirror( bool b );
      void set_auto_flip( bool b );

      double get_visual_angle() const;
      void set_system_angle_as_visual_angle( bool b );

      void set_auto_angular_speed_factor(double factor);

    protected:
      void add_visual
      ( const visual::scene_element& v,
        std::list<scene_visual>& visuals ) const;
      void add_visual
      ( const visual::sprite& spr, std::list<scene_visual>& visuals ) const;
      void add_visual
      ( const visual::sprite_sequence& seq,
        std::list<scene_visual>& visuals ) const;
      scene_visual get_scene_visual( const visual::scene_element& e ) const;
      scene_visual get_scene_visual( const visual::sprite& spr ) const;
      scene_visual get_scene_visual( const visual::sprite_sequence& seq ) const;

    private:
      visual::bitmap_rendering_attributes get_auto_rendering_attributes() const;

    private:
      /** \brief The gap between position of the item and  the position of his
          sprite. */
      visual::position_type m_gap;

      /** \brief Tell if we use the system angle of the item as the visual
          angle. */
      bool m_system_angle_as_visual;

      /** \brief Tell if we automatically mirror the item if needed. */
      bool m_auto_mirror;

      /** \brief Tell if we automatically flip the item if needed. */
      bool m_auto_flip;

      /** \brief The factor of movement applied at angular speed. */
      double m_auto_angular_speed_factor;

      /** \brief The visual position in the last iteration. */
      bear::universe::position_type m_last_visual_position;
    }; // class basic_renderable_item
  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/basic_renderable_item.tpp"

#endif // __ENGINE_BASIC_RENDERABLE_ITEM_HPP__
