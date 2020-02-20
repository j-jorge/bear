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
#ifndef __BF_BITMAP_RENDERING_ATTRIBUTES_HPP__
#define __BF_BITMAP_RENDERING_ATTRIBUTES_HPP__

#include <claw/coordinate_2d.hpp>
#include "bf/color.hpp"
#include "bf/libeditor_export.hpp"
#include "bf/trinary_logic.hpp"

namespace bf
{
  class compiled_file;

  /**
   * \brief A class to represent the rendering attributes of a picture.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT bitmap_rendering_attributes
  {
  public:
    bitmap_rendering_attributes();

    bool operator==( const bitmap_rendering_attributes& that ) const;
    bool operator!=( const bitmap_rendering_attributes& that ) const;
    bool operator<( const bitmap_rendering_attributes& that ) const;

    const bf::color& get_color() const;
    bf::color& get_color();
    void set_color(const bf::color& c);    
    
    const claw::math::coordinate_2d<unsigned int>& get_size() const;

    void set_auto_size( bool a );
    bool get_auto_size() const;

    void set_size( const claw::math::coordinate_2d<unsigned int>& s );
    void set_size( unsigned int w, unsigned int h );
    void set_width( unsigned int w );
    void set_height( unsigned int h );

    unsigned int width() const;
    unsigned int height() const;

    void set_angle( double angle );
    double get_angle() const;

    void flip( trinary_logic::value_type b );
    void mirror( trinary_logic::value_type b );

    trinary_logic::value_type get_flipped_status() const;
    trinary_logic::value_type get_mirrored_status() const;

    void compile( compiled_file& f ) const;

    bitmap_rendering_attributes&
    assign( const bitmap_rendering_attributes& that );

    void combine( const bitmap_rendering_attributes& that );

  private:
    /** \brief The size of the bitmap on the screen. */
    claw::math::coordinate_2d<unsigned int> m_size;

    /** \brief Is the picture reversed along Y axis ? */
    trinary_logic::value_type m_flip;

    /** \brief Is the picture reversed along X axis ? */
    trinary_logic::value_type m_mirror;

    /** \brief The color. */
    color m_color;

    /** \brief The angle on the screen. */
    double m_angle;

    /** \brief Tell if the size is automatically computed. */
    bool m_auto_size;

  }; // class bitmap_rendering_attributes

} // namespace bf

#endif // __BF_BITMAP_RENDERING_ATTRIBUTES_HPP__
