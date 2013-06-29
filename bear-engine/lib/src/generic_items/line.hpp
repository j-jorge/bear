/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to display a line between items.
 * \author Julien Jorge
 */
#ifndef __BEAR_LINE_HPP__
#define __BEAR_LINE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "universe/const_item_handle.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A class to display a line between items.
   *
   * The custom fields of this class are:
   * - ends (list of items): the ends of the segments of the line,
   * - width (real): the width of the line (default = 1).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT line:
    public engine::basic_renderable_item<engine::base_item>
  {
    DECLARE_BASE_ITEM(line);

  public:
    /** \brief The type of the parent class. */
    typedef engine::basic_renderable_item<engine::base_item> super;

  private:
    /** \brief The type of the container in which the coordinates of the line
        are stored. */
    typedef std::list<universe::const_item_handle> point_list_type;

  public:
    line();
    explicit line( const super& that );

    void build();

    bool set_real_field( const std::string& name, double value );
    bool set_item_list_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

    void progress( universe::time_type elapsed_time );
    void get_visual( std::list<engine::scene_visual>& visuals ) const;

    void push_back( engine::base_item* item );
    void set_line_width( visual::size_type w );

  private:
    void adjust_position_and_size();

  private:
    /** \brief The ends of the segments of the line. */
    point_list_type m_points;

    /** \brief The width of the line. */
    visual::size_type m_width;

  }; // class line
} // namespace bear

#endif // __BEAR_LINE_HPP__
