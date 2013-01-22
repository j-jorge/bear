/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class to display a link between two items.
 * \author Julien Jorge
 */
#ifndef __BEAR_BASE_LINK_VISUAL_HPP__
#define __BEAR_BASE_LINK_VISUAL_HPP__

#include "engine/base_item.hpp"
#include "engine/export.hpp"

#include <functional>

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief Base class to display a link between two items.
   *
   * The custom fields of this class are:
   * - start_item: item, the item to use as reference for the start extremity,
   * - end_item: item, the item to use as reference for the end extremity,
   * - start_x_origin: string in {"left", "right", "middle"}, the x position of
   *   the start extremity in the reference item. (default = "middle"),
   * - start_y_origin: string in {"top", "bottom", "middle"}, the y position of
   *   the start extremity in the reference item. (default = "middle"),
   * - end_x_origin: string in {"left", "right", "middle"}, the x position of
   *   the end extremity in the reference item. (default = "middle"),
   * - end_y_origin: string in {"top", "bottom", "middle"}, the y position of
   *   the end extremity in the reference item. (default = "middle"),
   * - start_x_delta: real, delta to apply on the x-position of the start
   *   extremity (default = 0),
   * - start_y_delta: real, delta to apply on the y-position of the start
   *   extremity (default = 0),
   * - end_x_delta: real, delta to apply on the x-position of the end
   *   extremity (default = 0),
   * - end_y_delta: real, delta to apply on the y-position of the end
   *   extremity (default = 0),
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT base_link_visual:
    public engine::base_item
  {
    DECLARE_BASE_ITEM( base_link_visual);

  public:
    typedef engine::base_item super;

  private:
    typedef std::const_mem_fun_t
    <universe::coordinate_type, universe::physical_item_state>
    position_function_type;

  public:
    base_link_visual();

    void set_items( engine::base_item* s, engine::base_item* e );
    void set_items
    ( engine::base_item* s, engine::base_item* e, std::size_t link_id );

    bool set_real_field( const std::string& name, double value );
    bool set_string_field( const std::string& name, const std::string& value );
    bool set_item_field( const std::string& name, base_item* value );

    void progress( universe::time_type elapsed_time );

  protected:
    void update_size( universe::time_type elapsed_time );

    universe::position_type get_start_position() const;
    universe::position_type get_end_position() const;

  private:
    /** \brief The item to use for the start extremity. */
    universe::item_handle m_start;

    /** \brief The item to use for the end extremity. */
    universe::item_handle m_end;

    /** \brief Method to call on the start item to get the x-position of the
        start extremity. */
    position_function_type m_start_x_position;

    /** \brief Method to call on the start item to get the y-position of the
        start extremity. */
    position_function_type m_start_y_position;

    /** \brief Method to call on the end item to get the x-position of the end
        extremity. */
    position_function_type m_end_x_position;

    /** \brief Method to call on the end item to get the y-position of the end
        extremity. */
    position_function_type m_end_y_position;

    /** \brief Delta to apply to the start item's origin. */
    universe::position_type m_start_delta;

    /** \brief Delta to apply to the end item's origin. */
    universe::position_type m_end_delta;

    /** \brief The identifier of the link displayed by this item. */
    std::size_t m_link_id;

  }; // class base_link_visual
} // namespace bear

#endif // __BEAR_BASE_LINK_VISUAL_HPP__
