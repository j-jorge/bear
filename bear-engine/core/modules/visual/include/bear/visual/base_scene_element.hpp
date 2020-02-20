/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for scene elements.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BASE_SCENE_ELEMENT_HPP__
#define __VISUAL_BASE_SCENE_ELEMENT_HPP__

#include "bear/visual/bitmap_rendering_attributes.hpp"
#include "bear/visual/types.hpp"
#include "bear/visual/class_export.hpp"

#include <list>

namespace bear
{
  namespace visual
  {
    class base_screen;
    class scene_element;

    /**
     * \brief The base class for scene elements.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT base_scene_element
    {
    public:
      /** \brief A list of elements of the scene. */
      typedef std::list<scene_element> scene_element_list;

      /** \brief A list of rectangles. */
      typedef std::list<rectangle_type> rectangle_list;

    public:
      base_scene_element( coordinate_type x = 0, coordinate_type y = 0 );
      virtual ~base_scene_element();

      virtual base_scene_element* clone() const;

      virtual rectangle_type get_opaque_box() const;
      virtual rectangle_type get_bounding_box() const;

      virtual void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      virtual void render( base_screen& scr ) const;

      const position_type& get_position() const;
      void set_position( coordinate_type x, coordinate_type y );
      void set_position( position_type );

      const position_type& get_shadow() const;
      void set_shadow( coordinate_type x, coordinate_type y );

      double get_shadow_opacity() const;
      void set_shadow_opacity( double o );
  
      void set_scale_factor( double x, double y );
      double get_scale_factor_x() const;
      double get_scale_factor_y() const;

      void set_rendering_attributes( const bitmap_rendering_attributes& a );
      const bitmap_rendering_attributes& get_rendering_attributes() const;
      bitmap_rendering_attributes& get_rendering_attributes();

      virtual bool always_displayed() const;

    private:
      /** \brief The position of the element on the screen. */
      position_type m_position;

      /** \brief Scale factor on both axis. */
      size_box_type m_scale_factor;

      /** \brief The distance of the projection of the shadow. */
      position_type m_shadow;

      /** \brief The opacity of the shadow. */
      double m_shadow_opacity;

      /** \brief The attributes applied to the element when rendering. */
      bitmap_rendering_attributes m_rendering_attributes;

    }; // class base_scene_element
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BASE_SCENE_ELEMENT_HPP__
