/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_snapshot_alignment class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_modify_snapshot_alignment.hpp"

#include "bf/gui_model.hpp"
#include "bf/action.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The snapshot.
 * \param x_alignment The type of alignment on x-coordinate.
 * \param x_value The value of alignment on x-coordinate.
 * \param y_alignment The type of alignment on y-coordinate.
 * \param y_value The value of alignment on y-coordinate.
 */
bf::action_modify_snapshot_alignment::action_modify_snapshot_alignment
( snapshot* s, const std::string& x_alignment, double x_value,
  const std::string& y_alignment, double y_value)
  : m_snapshot(s),
    m_x_alignment(x_alignment), m_x_alignment_value(x_value),
    m_y_alignment(y_alignment), m_y_alignment_value(y_value)
{
} // action_modify_snapshot_alignment::action_modify_snapshot_alignment()

/*----------------------------------------------------------------------------*/
void bf::action_modify_snapshot_alignment::execute( gui_model& mdl )
{
  const std::string x_alignment(m_snapshot->get_x_alignment());
  double x_alignment_value(m_snapshot->get_x_alignment_value());
  const std::string y_alignment(m_snapshot->get_y_alignment());
  double y_alignment_value(m_snapshot->get_y_alignment_value());

  m_snapshot->set_x_alignment(m_x_alignment);
  m_snapshot->set_x_alignment_value(m_x_alignment_value);
  m_snapshot->set_y_alignment(m_y_alignment);
  m_snapshot->set_y_alignment_value(m_y_alignment_value);

  m_x_alignment = x_alignment;
  m_x_alignment_value = x_alignment_value;
  m_y_alignment = y_alignment;
  m_y_alignment_value = y_alignment_value;
} // action_modify_snapshot_alignment::execute()

/*----------------------------------------------------------------------------*/
void bf::action_modify_snapshot_alignment::undo( gui_model& mdl )
{
    const std::string x_alignment(m_snapshot->get_x_alignment());
  double x_alignment_value(m_snapshot->get_x_alignment_value());
  const std::string y_alignment(m_snapshot->get_y_alignment());
  double y_alignment_value(m_snapshot->get_y_alignment_value());

  m_snapshot->set_x_alignment(m_x_alignment);
  m_snapshot->set_x_alignment_value(m_x_alignment_value);
  m_snapshot->set_y_alignment(m_y_alignment);
  m_snapshot->set_y_alignment_value(m_y_alignment_value);

  m_x_alignment = x_alignment;
  m_x_alignment_value = x_alignment_value;
  m_y_alignment = y_alignment;
  m_y_alignment_value = y_alignment_value;
} // action_modify_snapshot_alignment::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_modify_snapshot_alignment::is_identity
( const gui_model& mdl ) const
{
  return (m_x_alignment == m_snapshot->get_x_alignment()) &&
    (m_x_alignment_value == m_snapshot->get_x_alignment_value()) &&
    (m_y_alignment == m_snapshot->get_y_alignment()) &&
    (m_y_alignment_value == m_snapshot->get_y_alignment_value());
} // action_modify_snapshot_alignment::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_modify_snapshot_alignment::get_description() const
{
  return _("Set bounding box alignment in snapshot");
} // action_modify_snapshot_alignment::get_description()
