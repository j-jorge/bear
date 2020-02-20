/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_deprioritize class.
 * \author Julien Jorge
 */
#include "bf/history/action_deprioritize.hpp"

#include "bf/gui_level.hpp"
#include "bf/item_class.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to deprioritize.
 * \param lay The layer in which the priority is unset.
 */
bf::action_deprioritize::action_deprioritize
( item_instance* item, unsigned int lay )
  : m_item(item), m_layer(lay), m_priority(-1)
{

} // action_deprioritize::action_deprioritize()

/*----------------------------------------------------------------------------*/
void bf::action_deprioritize::execute( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  if ( lvl.get_layer(m_layer).is_prioritized(m_item) )
    {
      m_priority = lvl.get_layer(m_layer).get_priority(m_item);
      lvl.get_layer(m_layer).deprioritize(m_item);
    }
} // action_deprioritize::execute()

/*----------------------------------------------------------------------------*/
void bf::action_deprioritize::undo( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  if ( m_priority != -1 )
    lvl.get_layer(m_layer).prioritize(m_item, m_priority);
} // action_deprioritize::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_deprioritize::is_identity( const gui_level& lvl ) const
{
  return !lvl.get_layer(m_layer).is_prioritized(m_item);
} // action_deprioritize::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_deprioritize::get_description() const
{
  return wxString::Format
    ( _("Remove the compilation priority of %s (%s)"),
      std_to_wx_string(m_item->get_class().get_class_name()).c_str(),
      std_to_wx_string(m_item->get_id()).c_str() );
} // action_deprioritize::get_description()
