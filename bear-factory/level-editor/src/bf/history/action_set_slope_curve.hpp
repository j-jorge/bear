/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of changing the position and the size of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_SLOPE_CURVE_HPP__
#define __BF_ACTION_SET_SLOPE_CURVE_HPP__

#include "bf/history/action_group.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The action of changing the curve of a slope.
   * \author Sebastien Angibaud
   */
  class action_set_slope_curve:
    public action_group
  {
  public:
    /**
     * \brief Constructor.
     * \param item The item to modify.
     * \param left_x The new x-coordinate of left point control.
     * \param left_y The new y-coordinate of left point control.
     * \param right_x The new x-coordinate of right point control.
     * \param right_y The new y-coordinate of right point control.
     */
    action_set_slope_curve
    ( item_instance* item, double left_x, double left_y, double right_x,
      double right_y );

    wxString get_description() const;

  }; // class action_set_slope_curve
} // namespace bf

#endif // __BF_ACTION_SET_SLOPE_CURVE_HPP__
