/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A path_trace shows the path used by an item.
 * \author Julien Jorge
 */
#ifndef __BEAR_PATH_TRACE_HPP__
#define __BEAR_PATH_TRACE_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/universe/const_item_handle.hpp"

#include "bear/generic_items/class_export.hpp"
#include "bear/engine/export.hpp"

namespace bear
{
  /**
   * \brief A path_trace shows the path used by an item.
   *
   * A path_trace is a filled polygon made with the previous top and bottom
   * positions of a given item.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT path_trace:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(path_trace);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

    /** \brief The type of the handle on the traced item. */
    typedef universe::const_item_handle handle_type;

  private:
    /** \brief A list of positions. */
    typedef std::list<universe::position_type> position_list;

    /** \brief A list of date. */
    typedef std::list<universe::time_type> date_list;

    /** \brief The type of a function to use to do the progression of this
        instance. */
    typedef void (path_trace::*progress_function_type)
    ( universe::time_type elapsed_time );

  public:
    path_trace();
    explicit path_trace( const base_item& ref );

    void set_fill_color( const visual::color_type& c );
    void set_use_horizontal_axis( bool b );
    void set_tail_ratio( double r );
    void set_item( const base_item& ref );
    void set_fade_out_speed( double s );
    void set_length( universe::time_type length );

    void progress( universe::time_type elapsed_time );
    void get_visual( std::list<engine::scene_visual>& visuals ) const;

    virtual void move( universe::time_type elapsed_time );

  private:
    void progress_void( universe::time_type elapsed_time );
    void progress_alive( universe::time_type elapsed_time );
    void progress_dead( universe::time_type elapsed_time );
    void update_length();

    std::vector<visual::position_type>
      get_polygon
      ( std::size_t i, universe::position_type first_bottom,
        universe::position_type second_bottom,
        universe::position_type first_top,
        universe::position_type second_top ) const;
    std::vector<visual::position_type>
      get_vertical_polygon
      ( std::size_t i, universe::position_type first_bottom,
        universe::position_type second_bottom,
        universe::position_type first_top,
        universe::position_type second_top ) const;
    std::vector<visual::position_type>
      get_horizontal_polygon
      ( std::size_t i, universe::position_type first_bottom,
        universe::position_type second_bottom,
        universe::position_type first_top,
        universe::position_type second_top ) const;

    void set_auto_axis();
    void push_position();

  private:
    /** \brief The function to use to do the progression of this instance. */
    progress_function_type m_progress;

    /** \brief The item we are following. */
    handle_type m_item;

    /** \brief The previous positions of the top of the item. */
    position_list m_previous_top;

    /** \brief The previous positions of the bottom of the item. */
    position_list m_previous_bottom;

    /** \brief The previous date. */
    date_list m_previous_date;

    /** \brief The color of the trace. */
    visual::color_type m_fill_color;

    /** \brief The opacity of the trace. */
    double m_opacity;

    /** \brief The speed of the fade out when the item is dead. */
    double m_fade_out_speed;

    /** \brief The maximal length of the trace in seconds. */
    universe::time_type m_maximal_length;

    /** \brief The current date. */
    universe::time_type m_date;

    /** \brief The ratio the size the of the trace between its end and its
        head. */
    double m_tail_ratio;

    /** \brief Tells if the trace uses the horizontal axis of the moving item
        for its computations, instead of the vertical axis. */
    bool m_use_horizontal_axis;

  }; // class path_trace
} // namespace bear

#endif // __BEAR_PATH_TRACE_HPP__
