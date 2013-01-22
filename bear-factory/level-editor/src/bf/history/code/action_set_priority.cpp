/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_priority class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_priority.hpp"

#include "bf/gui_level.hpp"
#include "bf/item_class.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to set_priority.
 * \param lay The layer in which the priority is unset.
 * \param p The priority of the item.
 */
bf::action_set_priority::action_set_priority
( item_instance* item, unsigned int lay, std::size_t p )
  : m_item(item), m_layer(lay), m_priority(p)
{

} // action_set_priority::action_set_priority()

/*----------------------------------------------------------------------------*/
void bf::action_set_priority::execute( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  if ( lvl.get_layer(m_layer).is_prioritized(m_item) )
    m_previous_priority = lvl.get_layer(m_layer).get_priority(m_item);

  lvl.get_layer(m_layer).prioritize( m_item, m_priority );
} // action_set_priority::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_priority::undo( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  if ( m_previous_priority != -1 )
    lvl.get_layer(m_layer).prioritize(m_item, m_previous_priority);
  else
    lvl.get_layer(m_layer).deprioritize(m_item);
} // action_set_priority::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_priority::is_identity( const gui_level& lvl ) const
{
  if ( !lvl.get_layer(m_layer).is_prioritized(m_item) )
    return true;
  else
    return lvl.get_layer(m_layer).get_priority(m_item) == m_priority;
} // action_set_priority::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_priority::get_description() const
{
  return wxString::Format
    ( _("Change the compilation priority of %s (%s)"),
      std_to_wx_string(m_item->get_class().get_class_name()).c_str(),
      std_to_wx_string(m_item->get_id()).c_str() );
} // action_set_priority::get_description()
