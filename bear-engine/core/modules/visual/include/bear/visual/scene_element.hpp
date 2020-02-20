/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A proxy for all base_scene_element derivated classes.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_ELEMENT_HPP__
#define __VISUAL_SCENE_ELEMENT_HPP__

#include "bear/visual/base_scene_element.hpp"

#include "bear/visual/class_export.hpp"

#include <list>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A proxy for all base_scene_element derivated classes.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_element
    {
    public:
      /** \brief A list of elements of the scene. */
      typedef base_scene_element::scene_element_list scene_element_list;

      /** \brief A list of rectangles. */
      typedef base_scene_element::rectangle_list rectangle_list;

    public:
      scene_element( const base_scene_element& e = base_scene_element() );
      scene_element( const scene_element& that );
      ~scene_element();

      scene_element& operator=( const scene_element& that );

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void
      burst( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

      const position_type& get_position() const;
      void set_position( const position_type& p );
      void set_position( coordinate_type x, coordinate_type y );

      bool has_shadow() const;
      const position_type& get_shadow() const;
      void set_shadow( const position_type& p );
      void set_shadow( coordinate_type x, coordinate_type y );

      double get_shadow_opacity() const;
      void set_shadow_opacity( double o );

      void set_scale_factor( double x, double y );
      void set_scale_factor( double r );
      double get_scale_factor_x() const;
      double get_scale_factor_y() const;

      size_type get_element_width() const;
      size_type get_element_height() const;

      size_type get_width() const;
      size_type get_height() const;

      void set_rendering_attributes( const bitmap_rendering_attributes& a );
      const bitmap_rendering_attributes& get_rendering_attributes() const;
      bitmap_rendering_attributes& get_rendering_attributes();

      bool always_displayed() const;

    private:
      /** \brief The real visual. */
      base_scene_element* m_elem;

    }; // class scene_element
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_ELEMENT_HPP__
