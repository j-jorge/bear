/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item to create a path_trace on the colliding items under a given
 *        condition.
 * \author Julien Jorge
 */
#ifndef __BEAR_PATH_TRACER_HPP__
#define __BEAR_PATH_TRACER_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_trigger.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief An item to create a path_trace on the colliding items under a given
   *        condition.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT path_tracer:
    public engine::item_with_trigger<engine::base_item>
  {
    DECLARE_BASE_ITEM(path_tracer);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_trigger<engine::base_item> super;

  private:
    /** \brief The type of the handle on the traced item. */
    typedef universe::const_item_handle handle_type;

    /** \brief A list of handles. */
    typedef std::list<handle_type> handle_list_type;

  public:
    path_tracer();

    bool set_real_field( const std::string& name, double value );
    bool set_color_field
      ( const std::string& name, visual::color value );

    void progress( universe::time_type elapsed_time );

    void set_fill_color( const visual::color_type& c );
    void set_fade_out_speed( double s );
    void set_length( universe::time_type length );

  private:
    void on_trigger_on( base_item* activator );

  private:
    /** \brief The color of the trace. */
    visual::color_type m_fill_color;

    /** \brief The speed of the fade out when the item is dead. */
    double m_fade_out_speed;    

    /** \brief The maximal length of the trace in seconds. */
    universe::time_type m_maximal_length;

    /** \brief The ratio the size the of the trace between its end and its
        head. */
    double m_tail_ratio;

    /** \brief Handles on the items for which we already have created a
        trace. */
    handle_list_type m_traces;

  }; // class path_tracer
} // namespace bear

#endif // __BEAR_PATH_TRACER_HPP__
