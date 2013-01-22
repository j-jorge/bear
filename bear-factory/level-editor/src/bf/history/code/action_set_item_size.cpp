/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_size class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_size.hpp"

#include "bf/item_rendering_parameters.hpp"
#include "bf/history/action_set_item_field.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
bf::action_set_item_size::action_set_item_size
( item_instance* item, double width, double height )
{
  CLAW_PRECOND( item != NULL );
  add_action
    ( new action_set_item_field<real_type>
      (item, item_rendering_parameters::s_field_name_width, width) );
  add_action
    ( new action_set_item_field<real_type>
      (item, item_rendering_parameters::s_field_name_height, height) );
} // action_set_item_size::action_set_item_size()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_size::get_description() const
{
  return _("Resize item");
} // action_set_item_size::get_description()
