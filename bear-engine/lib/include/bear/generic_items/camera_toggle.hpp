/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A toggle that changes the current camera.
 * \author Julien Jorge
 */
#ifndef __BEAR_CAMERA_TOGGLE_HPP__
#define __BEAR_CAMERA_TOGGLE_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/item_with_toggle.hpp"
#include "bear/universe/derived_item_handle.hpp"

#include "bear/generic_items/class_export.hpp"
#include "bear/engine/export.hpp"

namespace bear
{
  class camera;

  /**
   * \brief A toggle that changes the current camera.
   *
   * The following fields are supported by this item:
   *  - camera (item) [required], the camera to switch to,
   *  - transition_duration (real), the time passed to the smooth activation
   *    (default = 0.5).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT camera_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(camera_toggle);

    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

    /** \brief The type of the handle on the item. */
    typedef universe::derived_item_handle<camera> handle_type;

  public:
    camera_toggle();

    bool set_item_field( const std::string& name, engine::base_item* value );
    bool set_real_field( const std::string& name, double value );

    bool is_valid() const;

  private:
    void build_off();
    void on_toggle_on( engine::base_item* activator );
    void on_toggle_off( engine::base_item* activator );

    void switch_camera();

  private:
    /** \brief The time passed to the starting smooth activation. */
    universe::time_type m_starting_smooth_delay;

    /** \brief The time passed to the ending smooth activation. */
    universe::time_type m_ending_smooth_delay;

    /** \brief The camera to activate. */
    handle_type m_camera;

  }; // class camera_toggle
} // namespace bear

#endif // __BEAR_CAMERA_TOGGLE_HPP__
