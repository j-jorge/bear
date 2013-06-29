/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a color with transparency.
 * \author Julien Jorge
 */
#ifndef __VISUAL_COLOR_HPP__
#define __VISUAL_COLOR_HPP__

#include "visual/class_export.hpp"

#include <claw/pixel.hpp>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class representing a color with transparency.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT color:
      public claw::graphic::rgba_pixel
    {
    public:
      color();
      color( const claw::graphic::rgba_pixel& c );
      color( const color& c );
      explicit color( const std::string& c );
      color( component_type r, component_type g, component_type b );
      color
      ( component_type r, component_type g, component_type b,
        component_type o );

      void set_red_intensity( double i );
      void set_green_intensity( double i );
      void set_blue_intensity( double i );
      void set_opacity( double o );
      void set(double red, double green, double blue, double opacity);

      double get_red_intensity() const;
      double get_green_intensity() const;
      double get_blue_intensity() const;
      double get_opacity() const;
    }; // class color

  } // namespace visual
} // namespace bear

#endif // __VISUAL_COLOR_HPP__
