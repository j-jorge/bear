/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_class class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_class.hpp"

#include "bf/item_class.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to modify.
 * \param c The new class of the item.
 */
bf::action_set_item_class::action_set_item_class
( item_instance* item, item_class const* c )
  : m_item(item), m_class(c), m_backup(*m_item)
{
  CLAW_PRECOND( item != NULL );
  CLAW_PRECOND( c != NULL );
} // action_set_item_class::action_set_item_class()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_class::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_item != NULL );

  m_item->set_class( m_class );
} // action_set_item_class::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_class::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_item != NULL );

  *m_item = m_backup;
} // action_set_item_class::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_item_class::is_identity( const gui_level& gui ) const
{
  CLAW_PRECOND( m_item != NULL );

  return m_item->get_class_ptr() == m_class;
} // action_set_item_class::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_class::get_description() const
{
  return
    wxString::Format
    ( _("Set item class to %s"),
      std_to_wx_string(m_class->get_class_name()).c_str() );
} // action_set_item_class::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a generic description of the action.
 */
wxString bf::action_set_item_class::get_action_description()
{
  return _("Set item class");
} // action_set_item_class::get_action_description()
