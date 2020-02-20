/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_group class.
 * \author Julien Jorge
 */
#include "bf/history/action_group.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param desc A description of the actions in the group.
 */
bf::action_group::action_group( const wxString& desc )
  : m_description(desc)
{

} // action_group::action_group()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::action_group::~action_group()
{
  action_collection::iterator it;

  for (it=m_actions.begin(); it!=m_actions.end(); ++it)
    delete *it;
} // action_group::~action_group()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an action at the end.
 * \param a The action to add.
 */
void bf::action_group::add_action( level_action* a )
{
  m_actions.push_back(a);
} // action_group::add_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the actions of an other group into this group.
 * \param g The group from which we take the actions.
 * \post g.m_actions.empty() == true
 */
void bf::action_group::move( action_group& g )
{
  m_actions.splice( m_actions.end(), g.m_actions );
} // action_group::move()

/*----------------------------------------------------------------------------*/
void bf::action_group::execute( gui_level& lvl )
{
  action_collection::iterator it;

  for (it=m_actions.begin(); it!=m_actions.end(); ++it)
    (*it)->execute(lvl);
} // action_group::execute()

/*----------------------------------------------------------------------------*/
void bf::action_group::undo( gui_level& lvl )
{
  action_collection::reverse_iterator it;

  for (it=m_actions.rbegin(); it!=m_actions.rend(); ++it)
    (*it)->undo(lvl);
} // action_group::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_group::is_identity( const gui_level& lvl ) const
{
  bool result(true);
  action_collection::const_reverse_iterator it;

  for (it=m_actions.rbegin(); result && (it!=m_actions.rend()); ++it)
    result = (*it)->is_identity(lvl);

  return result;
} // action_group::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_group::get_description() const
{
  return m_description;
} // action_group::get_description()
