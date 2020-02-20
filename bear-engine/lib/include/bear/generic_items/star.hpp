/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to display a star.
 * \author Julien Jorge
 */
#ifndef __BEAR_STAR_HPP__
#define __BEAR_STAR_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/basic_renderable_item.hpp"
#include "bear/visual/star.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A class to display a star.
   *
   * The custom fields of this class are:
   * - branches (unsigned int): the number of branches in the star
   *   (default = 5),
   * - ratio (real): the ratio of the inner vertices relatively to the outer
   *   ones (default = 0.5),
   * - border_width (real): the width of the border of the star (default = 1),
   * - border_color.red (real): the red component of the color of the border
   *   (default = 0),
   * - border_color.green (real): the green component of the color of the border
   *   (default = 0),
   * - border_color.blue (real): the blue component of the color of the border
   * - border_color.opacity (real): the opacity of the border (default = 1),
   * - fill_color.red (real): the red component of the color of the inside
   *   (default = 0),
   * - fill_color.green (real): the green component of the color of the inside
   *   (default = 0),
   * - fill_color.blue (real): the blue component of the color of the inside
   * - fill_color.opacity (real): the opacity of the inside (default = 1),
   * - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT star:
    public engine::basic_renderable_item<engine::base_item>
  {
    DECLARE_BASE_ITEM(star);

  public:
    /** \brief The type of the parent class. */
    typedef engine::basic_renderable_item<engine::base_item> super;

  public:
    star();
    star
      ( std::size_t branches, double ratio, const visual::color_type& color,
        double border_width = 1,
        const visual::color_type& fill_color
        = claw::graphic::transparent_pixel );

    void on_enters_layer();

    bool set_u_integer_field( const std::string& name, unsigned int value );
    bool set_real_field( const std::string& name, double value );

    void get_visual( std::list<engine::scene_visual>& visuals ) const;
    void set_ratio( double r );

  private:
    /** \brief The star to display. */
    visual::star m_star;

    /** \brief The color of the inside. */
    visual::color_type m_fill_color;

    /** \brief The color of the border. */
    visual::color_type m_border_color;

    /** \brief The width of the border. */
    double m_border_width;

  }; // class star
} // namespace bear

#endif // __BEAR_STAR_HPP__
