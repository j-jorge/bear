/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This toggle changes the time scale if an input is pressed when it is
 *        turned on.
 * \author Julien Jorge
 */
#ifndef __BEAR_TIME_SCALE_ON_INPUT_TOGGLE_HPP__
#define __BEAR_TIME_SCALE_ON_INPUT_TOGGLE_HPP__

#include "bear/engine/item_brick/item_with_toggle.hpp"
#include "bear/engine/base_item.hpp"
#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  class time_scale;

  /**
   * \brief This toggle changes the time scale if an input is pressed when it is
   *        turned on.
   *
   * The custom fields of this class are :
   * - \a time_scale [required]: item of type type_scale, the item to use to
   *      scale the time,
   * - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT time_scale_on_input_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(time_scale_on_input_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    ~time_scale_on_input_toggle();

    void build();

    bool set_item_field( const std::string& name, engine::base_item* value );
    bool is_valid() const;
    void set_time_scale_item(const time_scale* time_scale_item);

  private:
    /** \brief The item used to scale the time. */
    time_scale* m_time_scale;

  }; // class time_scale_on_input_toggle
} // namespace bear

#endif // __BEAR_TIME_SCALE_ON_INPUT_TOGGLE_HPP__
