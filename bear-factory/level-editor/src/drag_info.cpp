/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::ingame_view class.
 * \author Julien Jorge
 */
#include "bf/drag_info.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::drag_info::drag_info()
  : drag_mode(drag_mode_none), picked_item(NULL), x_active(true), y_active(true)
{

} // drag_info::drag_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the difference between the initial mouse position and the current
 *        mouse position.
 */
wxSize bf::drag_info::delta() const
{
  return wxSize
    ( mouse_position.x - mouse_origin.x, mouse_position.y - mouse_origin.y );
} // drag_info::delta()
