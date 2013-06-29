/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This toggle changes the time scale when it is turned on.
 * \author Julien Jorge
 */
#ifndef __BEAR_TIME_SCALE_HPP__
#define __BEAR_TIME_SCALE_HPP__

#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/base_item.hpp"
#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This toggle changes the time scale when it is turned on.
   *
   * The custom fields of this class are :
   * - \a scale: real, the time scale (default = 2),
   * - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT time_scale:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(time_scale);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    time_scale();

    bool set_real_field( const std::string& name, double value );
    bool is_valid() const;

    void progress_on( universe::time_type elapsed_time );
    void set_scale(double scale);

  private:
    /** \brief The scale of the time. */
    double m_scale;

  }; // class time_scale
} // namespace bear

#endif // __BEAR_TIME_SCALE_HPP__
