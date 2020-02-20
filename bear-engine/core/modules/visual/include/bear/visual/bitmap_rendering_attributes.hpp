/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to represent rendering attributes for bitmaps (sprite,
 *        animation).
 * \author Julien Jorge
 */
#ifndef __VISUAL_BITMAP_RENDERING_ATTRIBUTES_HPP__
#define __VISUAL_BITMAP_RENDERING_ATTRIBUTES_HPP__

#include <claw/coordinate_2d.hpp>

#include "bear/visual/types.hpp"
#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class to represent the rendering attributes of a picture.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT bitmap_rendering_attributes
    {
    public:
      bitmap_rendering_attributes();
      explicit bitmap_rendering_attributes( const size_box_type& s );

      double get_opacity() const;
      void set_opacity( double opacity );

      double get_red_intensity() const;
      double get_green_intensity() const;
      double get_blue_intensity() const;
      void set_intensity( double r, double g, double b );
      void set_red_intensity( double i );
      void set_green_intensity( double i );
      void set_blue_intensity( double i );

      void colorize( color_type c );
      void set_color(const color_type& c);
      color_type get_color() const;

      const size_box_type& get_size() const;

      void set_size( const size_box_type& s );
      void set_size( size_type w, size_type h );
      void set_width( size_type w );
      void set_height( size_type h );

      size_type width() const;
      size_type height() const;

      void set_angle( double angle );
      double get_angle() const;

      void flip( bool b );
      void mirror( bool b );

      bool is_flipped() const;
      bool is_mirrored() const;

      void combine( const bitmap_rendering_attributes& that );

      color_type convert_color( const color_type& c ) const;

    private:
      /** \brief The size of the bitmap on the screen. */
      size_box_type m_size;

      /** \brief Is the picture reversed vertically? */
      bool m_flip;

      /** \brief Is the picture reversed horizontally? */
      bool m_mirror;

      /** \brief Pixel intensity. */
      double m_opacity;

      /** \brief Intensity of the red channel. */
      double m_red_intensity;

      /** \brief Intensity of the green channel. */
      double m_green_intensity;

      /** \brief Intensity of the blue channel. */
      double m_blue_intensity;

      /** \brief The angle on the screen. */
      double m_angle;

    }; // class bitmap_rendering_attributes

  } // namespace visual
} // namespace bear

#endif // __VISUAL_BITMAP_RENDERING_ATTRIBUTES_HPP__
