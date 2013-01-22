/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::level_overview_frame class.
 * \author Julien Jorge
 */
#include "bf/level_overview_frame.hpp"

#include "bf/ingame_view.hpp"
#include "bf/ingame_view_frame.hpp"
#include "bf/gui_level.hpp"
#include "bf/level_thumbnail_ctrl.hpp"
#include "bf/icon/adjust_horizontally.xpm"
#include "bf/icon/adjust_vertically.xpm"
#include "bf/icon/refresh.xpm"

#include <wx/artprov.h>
#include <wx/toolbar.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The level for which we want the view.
 */
bf::level_overview_frame::level_overview_frame( ingame_view_frame& parent )
  : wxFrame(&parent, wxID_ANY, _("Level overview"))
{
  m_thumbnail = new level_thumbnail_ctrl(this);
  m_thumbnail->set_view(&parent);

  wxBoxSizer* s = new wxBoxSizer( wxVERTICAL );
  s->Add(m_thumbnail, 1, wxALL | wxEXPAND);
  SetSizer(s);
  SetAutoLayout(true);

  wxToolBar* bar = CreateToolBar();
  bar->AddTool
    ( wxID_REFRESH, _("&Refresh"), wxBitmap(refresh_xpm), _("Refresh") );
  bar->AddTool
    ( ID_UPDATE_WIDTH, _("Update &width"),
      wxBitmap(adjust_horizontally_xpm), _("Update width") );
  bar->AddTool
    ( ID_UPDATE_HEIGHT, _("Update &height"),
      wxBitmap(adjust_vertically_xpm), _("Update height") );

  bar->Realize();
} // level_overview_frame::level_overview_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::level_overview_frame::~level_overview_frame()
{
  m_thumbnail->set_view(NULL);
} // level_overview_frame::~level_overview_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when clicking on the refresh button.
 * \param event This event occured.
 */
void bf::level_overview_frame::on_refresh( wxCommandEvent& WXUNUSED(event) )
{
  m_thumbnail->update();
} // level_overview_frame::on_refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occurred.
 */
void bf::level_overview_frame::on_close(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
  else
    Destroy();
} // level_overview_frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Update width".
 * \param event This event occured.
 */
void bf::level_overview_frame::on_update_width
( wxCommandEvent& WXUNUSED(event) )
{
  wxSize my_size(GetSize());
  wxSize ctrl_size(m_thumbnail->GetSize());
  wxSize size_diff(my_size-ctrl_size);

  const ingame_view* v = m_thumbnail->get_level()->get_ingame_view();
  const wxSize level_size
    ( v->get_level().get_width(), v->get_level().get_height() );
  double rate = (double)level_size.GetX() / (double)level_size.GetY();
  SetSize(wxSize((double)ctrl_size.GetY()*rate, ctrl_size.GetY()) + size_diff);
} // level_overview_frame::on_update_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Update height".
 * \param event This event occured.
 */
void bf::level_overview_frame::on_update_height
( wxCommandEvent& WXUNUSED(event) )
{
  wxSize my_size(GetSize());
  wxSize ctrl_size(m_thumbnail->GetSize());
  wxSize size_diff(my_size-ctrl_size);

  const ingame_view* v = m_thumbnail->get_level()->get_ingame_view();
  const wxSize level_size
    ( v->get_level().get_width(), v->get_level().get_height() );

  double rate = (double)level_size.GetX() / (double)level_size.GetY();
  SetSize(wxSize(ctrl_size.GetX(), (double)ctrl_size.GetX()/rate) + size_diff);
} // level_overview_frame::on_update_height()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::level_overview_frame, wxFrame)
  EVT_BUTTON( wxID_REFRESH, bf::level_overview_frame::on_refresh )
  EVT_CLOSE( bf::level_overview_frame::on_close )
  EVT_TOOL( bf::level_overview_frame::ID_UPDATE_WIDTH,
            bf::level_overview_frame::on_update_width )
  EVT_TOOL( bf::level_overview_frame::ID_UPDATE_HEIGHT,
            bf::level_overview_frame::on_update_height )
  EVT_TOOL( wxID_REFRESH, bf::level_overview_frame::on_refresh )
END_EVENT_TABLE()
