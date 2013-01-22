/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_fixed_attribute class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_fixed_attribute.hpp"

#include "bf/item_instance.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
bf::action_set_item_fixed_attribute::action_set_item_fixed_attribute
( item_instance* item, bool fixed )
  : m_item(item), m_fixed(fixed)
{
  CLAW_PRECOND( item != NULL );
} // action_set_item_fixed_attribute::action_set_item_fixed_attribute()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_fixed_attribute::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_item != NULL );

  const bool old( m_item->get_fixed() );

  m_item->set_fixed( m_fixed );

  m_fixed = old;
} // action_set_item_fixed_attribute::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_fixed_attribute::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_item != NULL );

  /* Calling execute() two times will restore the initial value. */
  execute(lvl);
} // action_set_item_fixed_attribute::undo()

/*----------------------------------------------------------------------------*/
bool
bf::action_set_item_fixed_attribute::is_identity( const gui_level& gui ) const
{
  return m_item->get_fixed() == m_fixed;
} // action_set_item_fixed_attribute::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_fixed_attribute::get_description() const
{
  if ( m_fixed )
    return _("Set the item as fixed");
  else
    return _("Set the item as not fixed");
} // action_set_item_fixed_attribute::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a generic description of the action.
 */
wxString bf::action_set_item_fixed_attribute::get_action_description()
{
  return _("Set item 'fixed' attribute");
} // action_set_item_fixed_attribute::get_action_description()
