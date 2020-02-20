/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_prioritize class.
 * \author Julien Jorge
 */
#include "bf/history/action_prioritize.hpp"

#include "bf/gui_level.hpp"
#include "bf/item_class.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to prioritize.
 * \param lay The layer in which the priority is set.
 */
bf::action_prioritize::action_prioritize
( item_instance* item, unsigned int lay )
  : m_item(item), m_layer(lay)
{

} // action_prioritize::action_prioritize()

/*----------------------------------------------------------------------------*/
void bf::action_prioritize::execute( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  lvl.get_layer(m_layer).prioritize(m_item);
} // action_prioritize::execute()

/*----------------------------------------------------------------------------*/
void bf::action_prioritize::undo( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  lvl.get_layer(m_layer).deprioritize(m_item);
} // action_prioritize::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_prioritize::is_identity( const gui_level& lvl ) const
{
  return lvl.get_layer(m_layer).is_prioritized(m_item);
} // action_prioritize::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_prioritize::get_description() const
{
  return wxString::Format
    ( _("Set the compilation priority of %s (%s)"),
      std_to_wx_string(m_item->get_class().get_class_name()).c_str(),
      std_to_wx_string(m_item->get_id()).c_str() );
} // action_prioritize::get_description()
