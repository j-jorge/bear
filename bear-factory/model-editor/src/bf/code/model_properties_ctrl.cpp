/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_properties_ctrl class.
 * \author Julien Jorge
 */
#include "bf/model_properties_ctrl.hpp"

#include "bf/action_list_frame.hpp"
#include "bf/mark_list_frame.hpp"
#include "bf/snapshot_frame.hpp"
#include "bf/model.hpp"
#include "bf/model_frame.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 * \param parent The parent window.
 */
bf::model_properties_ctrl::model_properties_ctrl( wxWindow* parent )
  : wxNotebook(parent, wxID_ANY)
{
  create_controls();
} // model_properties_ctrl::model_properties_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the model frame for which the properties are edited.
 */
void bf::model_properties_ctrl::set_model_frame( model_frame* m )
{
  m_actions_frame->set_model_frame(m);
  m_marks_frame->set_model_frame(m);
  m_snapshot_frame->set_model_frame(m);
} // model_properties_ctrl::set_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current action is modified.
 */
void bf::model_properties_ctrl::update_action()
{
  m_marks_frame->update_action();
  m_snapshot_frame->update_action();
  m_actions_frame->update_action();
} // model_properties_ctrl::update_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current snapshot is modified.
 */
void bf::model_properties_ctrl::update_snapshot()
{
  m_snapshot_frame->update_snapshot();
} // model_properties_ctrl::update_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current mark is modified.
 */
void bf::model_properties_ctrl::update_mark()
{
  m_marks_frame->update_mark();
  m_snapshot_frame->update_mark();
} // model_properties_ctrl::update_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::model_properties_ctrl::create_controls()
{ 
  wxPanel* properties = new wxPanel( this );
  
  m_actions_frame = new action_list_frame( properties );
  m_marks_frame = new mark_list_frame( properties, NULL );

  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  sizer->Add
    ( new wxStaticText(properties, wxID_ANY, _("Actions:")), 0 );
  sizer->Add( m_actions_frame, 1, wxEXPAND | wxALL, 0 );
  sizer->AddSpacer(5);
  sizer->Add
    ( new wxStaticText(properties, wxID_ANY, _("Marks:")), 0 );
  sizer->Add( m_marks_frame, 1, wxEXPAND | wxALL, 0 );
  properties->SetSizer(sizer);

  AddPage( properties, _("Properties") );

  m_snapshot_frame = new snapshot_frame( this );
  AddPage( m_snapshot_frame, _("Snapshot") );
} // model_properties_ctrl::create_controls()
