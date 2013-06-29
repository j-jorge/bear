/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item representing a decorative rectangle.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_DECORATIVE_RECTANGLE_HPP__
#define __BEAR_DECORATIVE_RECTANGLE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/scene_visual.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief An item representing a decorative rectangle.
   * \author Sebastien Angibaud
   * The fields of this item are
   * - fill_intensity.red : double, Intensity of the red channel for fill color
   * (default = 1),
   * - fill_intensity.green : double, Intensity of the green channel for fill 
   * color (default = 1),
   * - fill_intensity.blue : double, Intensity of the blue channel for fill 
   * color (default = 1),
   * - fill_opacity : double, Pixel intensity for fill color (default = 0).
   * - border_size : double, Size of the border (default = 1).
   * - any field supported by the parent classes.
   *
   */
  class GENERIC_ITEMS_EXPORT decorative_rectangle:
    public engine::basic_renderable_item<engine::base_item>
  {
    DECLARE_BASE_ITEM(decorative_rectangle);

    /** \brief The type of the parent class. */
    typedef engine::basic_renderable_item<engine::base_item> super;

  public:
    decorative_rectangle();
    void get_visual( std::list<engine::scene_visual>& visuals ) const;
    bool set_real_field( const std::string& name, double value );
    bool set_color_field( const std::string& name, visual::color value );
    void set_fill_color( const visual::color& c );
    void set_border_color( const visual::color& c );

  private:
    /** \brief The fill color. */
    visual::color m_fill_color;

    /** \brief The border color. */
    visual::color m_border_color;

    /** \brief Size of the border. */
    double m_border_size;
  }; // class decorative_rectangle
} // namespace bear

#endif // __BEAR_DECORATIVE_RECTANGLE_HPP__
