/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_snapshot_size class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_modify_snapshot_size.hpp"

#include "bf/gui_model.hpp"
#include "bf/action.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The snapshot.
 * \param width The new width of the snapshot.
 * \param height The new height of the snapshot.
 */
bf::action_modify_snapshot_size::action_modify_snapshot_size
( snapshot* s, const double width, const double height)
  : m_snapshot(s), m_width(width), m_height(height)
{

} // action_modify_snapshot_size::action_modify_snapshot_size()

/*----------------------------------------------------------------------------*/
void bf::action_modify_snapshot_size::execute( gui_model& mdl )
{
  const double width(m_snapshot->get_width());
  const double height(m_snapshot->get_height());

  m_snapshot->set_size(m_width, m_height);

  m_width = width;
  m_height = height;
} // action_modify_snapshot_size::execute()

/*----------------------------------------------------------------------------*/
void bf::action_modify_snapshot_size::undo( gui_model& mdl )
{
  const double width(m_snapshot->get_width());
  const double height(m_snapshot->get_height());

  m_snapshot->set_size(m_width, m_height);

  m_width = width;
  m_height = height;
} // action_modify_snapshot_size::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_modify_snapshot_size::is_identity
( const gui_model& mdl ) const
{
  return (m_snapshot->get_height() == m_height) &&
    (m_snapshot->get_width() == m_width );
} // action_modify_snapshot_size::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_modify_snapshot_size::get_description() const
{
  return _("Set bounding box size in snapshot");
} // action_modify_snapshot_size::get_description()
