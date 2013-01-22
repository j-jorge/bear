/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The value of a field of type "color".
 * \author Sébastien Angibaud
 */
#ifndef __BF_COLOR_HPP__
#define __BF_COLOR_HPP__

#include "bf/libeditor_export.hpp"

#include <string>

namespace bf
{
  class compiled_file;

  /**
   * \brief Color type.
   * \author Sébastien Angibaud
   */
  class BEAR_EDITOR_EXPORT color
  {
  public:
    color();
    color(double red, double green, double blue, double opacity);

    double get_opacity() const;
    void set_opacity( double opacity );

    double get_red_intensity() const;
    double get_green_intensity() const;
    double get_blue_intensity() const;
    void set_intensity( double r, double g, double b );

    void compile( compiled_file& f ) const;

    bool operator==( const color& that ) const;
    bool operator!=( const color& that ) const;
    bool operator<( const color& that ) const;

  private:
    /** \brief Pixel intensity. */
    double m_opacity;

    /** \brief Intensity of the red channel. */
    double m_red_intensity;

    /** \brief Intensity of the green channel. */
    double m_green_intensity;

    /** \brief Intensity of the blue channel. */
    double m_blue_intensity;
  }; // class color
} // namespace bf

#endif // __BF_COLOR_HPP__
