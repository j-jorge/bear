/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that shakes the camera.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_CAMERA_SHAKER_HPP__
#define __BEAR_CAMERA_SHAKER_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that shakes the camera.
   *
   * The valid fields for this item are
   *  - \a shaker_force: (real) force of the shaker (default = 10),
   *  - \a check_camera_intersection: (bool) Indicates if the shaker checks
   * camera intersection.
   *  - any field supported by the parent classes.
   *
   * \author Sebastien Angiabud
   */
  class GENERIC_ITEMS_EXPORT camera_shaker:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(camera_shaker);

  public:
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    camera_shaker();

    static void shake_around
      ( const engine::base_item& item, double force,
        universe::time_type duration, audio::sample* s = NULL );

    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    void set_shaker_force(double force);
    void check_camera_intersection(bool value);

    void progress_on( universe::time_type elapsed_time );

  private:
    /** \brief The force of the agitation. */
    double m_shaker_force;

    /** \brief Indicates if the shaker checks camera intersection. */
    bool m_camera_intersection;

  }; // class camera_shaker
} // namespace bear

#endif // __BEAR_CAMERA_SHAKER_HPP__
