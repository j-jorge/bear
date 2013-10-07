/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_view_ctrl class.
 * \author Julien Jorge
 */
#include "bf/model_view_ctrl.hpp"

#include "bf/model_view.hpp"
#include "bf/workspace_environment.hpp"

#include <wx/sizer.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param id The identifier of the window.
 * \param m The model to display.
 */
bf::model_view_ctrl::model_view_ctrl
( wxWindow& parent, wxWindowID id, gui_model& m, 
  workspace_environment* env )
  : wxPanel(&parent, id), m_workspace(env)
{
  create_controls(m);
  create_sizers();

  adjust_scrollbars();
} // model_view_ctrl::model_view_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the view to reload the action.
 */
void bf::model_view_ctrl::reload_action()
{
  m_model_view->reload_action();
} // model_view_ctrl::reload_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the action to display.
 * \param d The date at which the action is displayed.
 */
void bf::model_view_ctrl::set_date( double d )
{
  m_model_view->set_date(d);
} // model_view_ctrl::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame or snapshot.
 * \brief Return the new date.
 */
double bf::model_view_ctrl::next()
{
  return m_model_view->next();
} // model_view_ctrl::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we are at the end of the action.
 */
bool bf::model_view_ctrl::is_finished() const
{
  return m_model_view->is_finished();
} // model_view_ctrl::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration until the next change.
 */
double bf::model_view_ctrl::get_duration_until_next() const
{
  return m_model_view->get_duration_until_next();
} // model_view_ctrl::get_duration_until_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the zoom ratio.
 * \param z The zoom ratio.
 */
void bf::model_view_ctrl::set_zoom( unsigned int z )
{
  m_model_view->set_zoom(z);
} // model_view_ctrl::set_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoom ratio.
 */
unsigned int bf::model_view_ctrl::get_zoom() const
{
  return m_model_view->get_zoom();
} // model_view_ctrl::get_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the zoom ratio such that the image fits the control.
 */
void bf::model_view_ctrl::auto_zoom()
{
  m_model_view->auto_zoom();
} // model_view_ctrl::auto_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the wireframe drawing.
 */
void bf::model_view_ctrl::toggle_wireframe_mode()
{
  m_model_view->toggle_wireframe_mode();
} // model_view_ctrl::toggle_wireframe_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bounding box and the marks are drawn.
 */
bool bf::model_view_ctrl::get_wireframe_drawing() const
{
  return m_model_view->get_wireframe_drawing();
} // model_view_ctrl::get_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the graphic drawing.
 */
void bf::model_view_ctrl::toggle_graphic_mode()
{
  m_model_view->toggle_graphic_mode();
} // model_view_ctrl::toggle_graphic_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sprites of the marks are drawn.
 */
bool bf::model_view_ctrl::get_graphic_drawing() const
{
  return m_model_view->get_graphic_drawing();
} // model_view_ctrl::get_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the text drawing.
 */
void bf::model_view_ctrl::toggle_text_mode()
{
  m_model_view->toggle_text_mode();
} // model_view_ctrl::toggle_text_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the name and the function of the marks are drawn.
 */
bool bf::model_view_ctrl::get_text_drawing() const
{
  return m_model_view->get_text_drawing();
} // model_view_ctrl::get_text_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 * \param m The model to display.
 */
void bf::model_view_ctrl::create_controls( gui_model& m )
{
  m_model_view = new model_view(*this, ID_MODEL_VIEW, m, m_workspace);

  m_h_scrollbar = new wxScrollBar(this, wxID_ANY);
  m_v_scrollbar = new wxScrollBar
    ( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
} // model_view_ctrl::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers.
 */
void bf::model_view_ctrl::create_sizers()
{
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

  v_sizer->Add(m_v_scrollbar, 1, wxEXPAND);

  h_sizer->Add(m_model_view, 1, wxEXPAND);
  h_sizer->Add(v_sizer, 0, wxEXPAND);

  v_sizer = new wxBoxSizer(wxVERTICAL);
  v_sizer->Add(h_sizer, 1, wxEXPAND);

  h_sizer = new wxBoxSizer(wxHORIZONTAL);
  h_sizer->Add(m_h_scrollbar, 1, wxALIGN_BOTTOM);
  h_sizer->Add
    ( new wxWindow
      ( this, wxID_ANY, wxDefaultPosition,
        wxSize( m_v_scrollbar->GetSize().x, m_h_scrollbar->GetSize().y ) ),
      0
      );

  v_sizer->Add(h_sizer, 0, wxEXPAND);

  SetSizer( v_sizer );
} // model_view_ctrl::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the bounds and the position of the toolbars.
 */
void bf::model_view_ctrl::adjust_scrollbars()
{
  m_h_scrollbar->SetScrollbar
    ( m_model_view->get_view_delta().x, m_model_view->GetSize().x,
      m_model_view->get_view_size().x,
      (3 * m_model_view->GetSize().x) / 4, true );
  m_v_scrollbar->SetScrollbar
    ( m_model_view->get_view_delta().y, m_model_view->GetSize().y,
      m_model_view->get_view_size().y,
      (3 * m_model_view->GetSize().y) / 4, true );

  m_model_view->set_view_delta
    ( m_h_scrollbar->GetThumbPosition(), m_v_scrollbar->GetThumbPosition() );
} // model_view_ctrl::adjust_scrollbars()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the position of the view.
 * \param event The scroll event.
 */
void bf::model_view_ctrl::on_scroll( wxScrollEvent& WXUNUSED(event) )
{
  m_model_view->set_view_delta
    ( m_h_scrollbar->GetThumbPosition(), m_v_scrollbar->GetThumbPosition() );
} // model_view_ctrl::on_scroll()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::model_view_ctrl::on_size( wxSizeEvent& event )
{
  adjust_scrollbars();
  event.Skip();
} // model_view_ctrl::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark is modified.
 * \param event This event occured.
 */
void bf::model_view_ctrl::on_mark_change(mark_event& event)
{
  mark_event e
     ( event.get_value(), mark_event::selection_change_event_type, GetId() );
  e.SetEventObject(this);
  ProcessEvent(e);
} // model_view_ctrl::on_mark_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark move.
 * \param event This event occured.
 */
void bf::model_view_ctrl::on_placement_move(placement_event& event)
{
  placement_event e
    ( event.get_label(), event.get_x(), event.get_y(),
      event.get_width(), event.get_height(),
      placement_event::move_event_type, GetId() );
  e.SetEventObject(this);
  ProcessEvent(e);
} // model_view_ctrl::on_placement_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark is resized.
 * \param event This event occured.
 */
void bf::model_view_ctrl::on_placement_size(placement_event& event)
{
  placement_event e
    ( event.get_label(), event.get_x(), event.get_y(),
      event.get_width(), event.get_height(),
      placement_event::size_event_type, GetId() );
  e.SetEventObject(this);
  ProcessEvent(e);
} // model_view_ctrl::on_placement_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief The bounding box change.
 * \param event This event occured.
 */
void bf::model_view_ctrl::on_bounding_box_change(bounding_box_event& event)
{
  bounding_box_event e
     ( event.get_left(), event.get_bottom(),
       event.get_width(), event.get_height(),
       bounding_box_event::change_event_type, GetId() );
  e.SetEventObject(this);
  ProcessEvent(e);
} // model_view_ctrl::on_bounding_box_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The bounding box has been selected.
 * \param event This event occured.
 */
void bf::model_view_ctrl::on_bounding_box_selection(bounding_box_event& event)
{
  bounding_box_event e
     ( event.get_left(), event.get_bottom(),
       event.get_width(), event.get_height(),
       bounding_box_event::select_event_type, GetId() );
  e.SetEventObject(this);
  ProcessEvent(e);
} // model_view_ctrl::on_bounding_box_selection()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::model_view_ctrl, wxPanel)
  EVT_SCROLL( bf::model_view_ctrl::on_scroll )
  EVT_SIZE( bf::model_view_ctrl::on_size )
  EVT_MARK_SELECTION_CHANGE( bf::model_view_ctrl::ID_MODEL_VIEW,
                             bf::model_view_ctrl::on_mark_change )
  EVT_PLACEMENT_MOVE( bf::model_view_ctrl::ID_MODEL_VIEW,
                      bf::model_view_ctrl::on_placement_move )
  EVT_PLACEMENT_SIZE( bf::model_view_ctrl::ID_MODEL_VIEW,
                      bf::model_view_ctrl::on_placement_size )
  EVT_BOUNDING_BOX_CHANGE( bf::model_view_ctrl::ID_MODEL_VIEW,
                           bf::model_view_ctrl::on_bounding_box_change )
  EVT_BOUNDING_BOX_SELECTED( bf::model_view_ctrl::ID_MODEL_VIEW,
                             bf::model_view_ctrl::on_bounding_box_selection )
END_EVENT_TABLE()
