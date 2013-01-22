/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to create several bear::line.
 * \author Julien Jorge
 */
#ifndef __BEAR_LINES_HPP__
#define __BEAR_LINES_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A class to create several bear::line.
   *
   * bear::line has to adjust its size to fit all the items to be sure
   * they are displayed. If the set of items is large enough,
   * bear::line tends to cover the whole world. bear::lines will
   * create several instances of bear::line, one for each pair of
   * successive items, each one will thus be restricted to a small
   * part of the world.
   *
   * One reason to not use this class instead of bear::line is that
   * the whole line will be broken if one item dies, whereas
   * bear::line would just adapt the line to the remaining items.
   *
   * The custom fields of this class are:
   * - ends (list of items): the ends of the segments of the lines,
   * - width (real): the width of the lines (default = 1).
   *
   * \sa bear::line
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT lines:
    public engine::basic_renderable_item<engine::base_item>
  {
    DECLARE_BASE_ITEM(lines);

  public:
    /** \brief The type of the parent class. */
    typedef engine::basic_renderable_item<engine::base_item> super;

  private:
    /** \brief The type of the container in which the coordinates of the line
        are stored. */
    typedef std::list<engine::base_item*> point_list_type;

  public:
    lines();

    void build();

    bool set_real_field( const std::string& name, double value );
    bool set_item_list_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

  private:
    /** \brief The ends of the segments of the lines. */
    point_list_type m_points;

    /** \brief The width of the lines. */
    visual::size_type m_width;

  }; // class lines
} // namespace bear

#endif // __BEAR_LINES_HPP__
