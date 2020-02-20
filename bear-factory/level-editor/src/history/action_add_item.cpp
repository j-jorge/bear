/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_add_item class.
 * \author Julien Jorge
 */
#include "bf/history/action_add_item.hpp"

#include "bf/item_class.hpp"
#include "bf/gui_level.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
bf::action_add_item::action_add_item( item_instance* item, unsigned int lay )
  : m_item(item), m_layer(lay), m_done(false)
{

} // action_add_item::action_add_item()

/*----------------------------------------------------------------------------*/
bf::action_add_item::~action_add_item()
{
  /* If the action has not been done, the item must be deleted here. Otherwise,
     it will be deleted by the level. */
  if ( !m_done )
    delete m_item;
} // action_add_item::~action_add_item()

/*----------------------------------------------------------------------------*/
void bf::action_add_item::execute( gui_level& lvl )
{
  CLAW_PRECOND( !m_done );
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  lvl.get_layer( m_layer ).add_item( m_item );

  m_done = true;
} // action_add_item::execute()

/*----------------------------------------------------------------------------*/
void bf::action_add_item::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_done );
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  lvl.remove_from_selection( m_item );
  lvl.get_layer( m_layer ).remove_item( m_item );
  m_done = false;
} // action_add_item::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_add_item::is_identity( const gui_level& gui ) const
{
  return false;
} // action_add_item::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_add_item::get_description() const
{
  return _("Add item ")
    + std_to_wx_string(m_item->get_class().get_class_name());
} // action_add_item::get_description()
