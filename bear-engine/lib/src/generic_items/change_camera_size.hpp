/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that changes the size of the camera.
 * \author Julien Jorge
 */
#ifndef __BEAR_CHANGE_CAMERA_SIZE_HPP__
#define __BEAR_CHANGE_CAMERA_SIZE_HPP__

#include "engine/base_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A simple change_camera_size.
   *
   * The valid fields for this item are
   *  - \a wanted_width: (real) the width to give to the camera
   *    (default = 320),
   *  - \a wanted_height: (real) the height to give to the camera
   *    (default = 240),
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT change_camera_size:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(change_camera_size);

  public:
    typedef engine::base_item super;

  public:
    change_camera_size();

    bool set_real_field( const std::string& name, double value );
    void progress( universe::time_type elapsed_time );

  private:
    /** \brief The size to attain. */
    universe::size_box_type m_wanted_size;

  }; // class change_camera_size
} // namespace bear

#endif // __BEAR_CHANGE_CAMERA_SIZE_HPP__
