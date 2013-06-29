/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A sprite on the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_SPRITE_HPP__
#define __VISUAL_SCENE_SPRITE_HPP__

#include "visual/base_scene_element.hpp"
#include "visual/sprite.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A sprite on the screen.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_sprite:
      public base_scene_element
    {
    public:
      scene_sprite( coordinate_type x, coordinate_type y, const sprite& s );

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

    private:
      void set_sprite( const sprite& spr );

      void update_side_box
      ( const position_type& pos, const position_type& center,
        position_type& left_bottom, position_type& right_top ) const;

      void flip_values_on_axis
        ( coordinate_type& a, coordinate_type& b, coordinate_type z ) const;
    
      rectangle_type scale_rectangle( const rectangle_type& r ) const;
      rectangle_type unscale_rectangle( const rectangle_type& r ) const;

      scene_sprite burst
        ( const rectangle_type& box, const rectangle_type& clip_inter ) const;

      rectangle_type get_burst_opaque_box
        ( const rectangle_type& box,
          const rectangle_type& unscaled_clip ) const;

      claw::math::rectangle<coordinate_type> get_burst_clip
        ( const rectangle_type& unscaled_clip ) const;

    private:
      /** \brief The sprite. */
      sprite m_sprite;

    }; // class scene_sprite
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_SPRITE_HPP__
