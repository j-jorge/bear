/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_position_and_size class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_position_and_size.hpp"

#include "bf/history/action_set_item_position.hpp"
#include "bf/history/action_set_item_size.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_set_item_position_and_size::action_set_item_position_and_size
( item_instance* item, double left, double bottom, double width, double height )
{
  add_action( new action_set_item_position(item, left, bottom) );
  add_action( new action_set_item_size(item, width, height) );
} // action_set_item_position_and_size::action_set_item_position_and_size()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_position_and_size::get_description() const
{
  return _("Move and resize");
} // action_set_item_position_and_size::get_description()
