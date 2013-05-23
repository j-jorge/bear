/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_remove_layer class.
 * \author Julien Jorge
 */
#include "bf/history/action_remove_layer.hpp"

#include "bf/gui_level.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_remove_layer::action_remove_layer( unsigned int index )
  : m_layer(NULL), m_index(index)
{

} // action_remove_layer::action_remove_layer()

/*----------------------------------------------------------------------------*/
bf::action_remove_layer::~action_remove_layer()
{
  /* If the action has not been done, the layer must be deleted here. Otherwise,
     it will be deleted by the level. */
  if ( m_layer != NULL )
    delete m_layer;
} // action_remove_layer::~action_remove_layer()

/*----------------------------------------------------------------------------*/
void bf::action_remove_layer::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_layer == NULL );
  CLAW_PRECOND( lvl.layers_count() > m_index );

  m_selection = lvl.get_selection( m_index );
  m_layer = lvl.remove_layer( m_index );
} // action_remove_layer::execute()

/*----------------------------------------------------------------------------*/
void bf::action_remove_layer::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_layer != NULL );
  CLAW_PRECOND( lvl.layers_count() >= m_index );

  lvl.add_layer( m_layer, m_index );
  lvl.add_to_selection( m_selection );

  m_layer = NULL;
} // action_remove_layer::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_remove_layer::is_identity( const gui_level& lvl ) const
{
  return false;
} // action_remove_layer::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_remove_layer::get_description() const
{
  return _("Remove layer");
} // action_remove_layer::get_description()
