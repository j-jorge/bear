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
 * \brief Destructor.
 */
bf::action_group::action_group()
  : m_description(_("Group of actions"))
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
 * \brief Set the description.
 * \param description The new description.
 */
void bf::action_group::set_description( const wxString& description )
{
  m_description = description;
} // action_group::set_description()


/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an action at the end.
 * \param a The action to add.
 */
void bf::action_group::add_action( model_action* a )
{
  m_actions.push_back(a);
} // action_group::add_action()

/*----------------------------------------------------------------------------*/
void bf::action_group::execute( gui_model& lvl )
{
  action_collection::iterator it;

  for (it=m_actions.begin(); it!=m_actions.end(); ++it)
    (*it)->execute(lvl);
} // action_group::execute()

/*----------------------------------------------------------------------------*/
void bf::action_group::undo( gui_model& lvl )
{
  action_collection::reverse_iterator it;

  for (it=m_actions.rbegin(); it!=m_actions.rend(); ++it)
    (*it)->undo(lvl);
} // action_group::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_group::is_identity( const gui_model& lvl ) const
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
