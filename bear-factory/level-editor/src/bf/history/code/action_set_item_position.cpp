/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_position class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_position.hpp"

#include "bf/history/action_set_item_bottom.hpp"
#include "bf/history/action_set_item_left.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_set_item_position::action_set_item_position
( item_instance* item, double left, double bottom )
{
  add_action( new action_set_item_left(item, left) );
  add_action( new action_set_item_bottom(item, bottom) );
} // action_set_item_position::action_set_item_position()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_position::get_description() const
{
  return _("Move item");
} // action_set_item_position::get_description()
