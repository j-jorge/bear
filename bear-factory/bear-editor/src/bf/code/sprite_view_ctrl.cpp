/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sprite_view_ctrl class.
 * \author Julien Jorge
 */
#include "bf/sprite_view_ctrl.hpp"

#include "bf/sprite_view.hpp"

#include "bf/icon/zoom_original.xpm"
#include "bf/icon/zoom_in.xpm"
#include "bf/icon/zoom_out.xpm"
#include "bf/wx_facilities.hpp"
#include "bf/icon/zoom_fit.xpm"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param env The workspace environment to use.
 * \param spr The sprite to display.
 */
bf::sprite_view_ctrl::sprite_view_ctrl
( wxWindow& parent, workspace_environment& env, const sprite& spr )
  : wxPanel(&parent), m_sprite_view(NULL), m_workspace(env)
{
  create_controls();
  create_sizers();
  set_sprite(spr);

  m_combo_zoom->SetSelection( m_combo_zoom->FindString(wxT("100")) );
  adjust_scrollbars();
} // sprite_view_ctrl::sprite_view_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sprite to display.
 * \param spr The sprite to display.
 */
void bf::sprite_view_ctrl::set_sprite( const sprite& spr )
{
  m_sprite_view->set_sprite(spr);
} // sprite_view_ctrl::set_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::sprite_view_ctrl::create_controls()
{
  wxArrayString choices;

  choices.Add( wxT("10") );
  choices.Add( wxT("25") );
  choices.Add( wxT("33") );
  choices.Add( wxT("50") );
  choices.Add( wxT("66") );
  choices.Add( wxT("75") );
  choices.Add( wxT("100") );
  choices.Add( wxT("200") );
  choices.Add( wxT("400") );

  m_sprite_view = new sprite_view(*this, m_workspace, ID_SPRITE_VIEW);
  m_sprite_view->Connect
    ( ID_SPRITE_VIEW, wxEVT_MOTION,
      wxMouseEventHandler(sprite_view_ctrl::on_mouse_move), NULL, this );
  
  m_combo_zoom =
    new wxComboBox
    ( this, ID_COMBO_ZOOM, wxT("100"), wxDefaultPosition, wxDefaultSize,
      choices, wxCB_DROPDOWN | wxTE_PROCESS_ENTER );

  m_mouse_position = 
    new wxStaticText
    ( this, wxID_ANY, wxT("( X = ? ; Y = ? )"), 
      wxDefaultPosition, 
      wxDefaultSize, wxALIGN_RIGHT);

  m_h_scrollbar = new wxScrollBar(this, wxID_ANY);
  m_v_scrollbar = new wxScrollBar
    ( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
} // sprite_view_ctrl::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers.
 */
void bf::sprite_view_ctrl::create_sizers()
{
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

  v_sizer->Add(m_v_scrollbar, 1, wxEXPAND);

  h_sizer->Add(m_sprite_view, 1, wxEXPAND);
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

  h_sizer = new wxBoxSizer(wxHORIZONTAL);
  h_sizer->Add
    ( new wxBitmapButton(this, ID_ZOOM_100, wxBitmap(zoom_original_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_ZOOM_FIT, wxBitmap(zoom_fit_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_ZOOM_IN, wxBitmap(zoom_in_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_ZOOM_OUT, wxBitmap(zoom_out_xpm)) );
  h_sizer->Add( m_combo_zoom, 1, wxEXPAND );
  h_sizer->Add( m_mouse_position, 2, wxEXPAND );

  //v_sizer->Add( m_sprite_view, 1, wxEXPAND );
  v_sizer->Add( h_sizer, 0, wxEXPAND );

  SetSizer( v_sizer );
} // sprite_view_ctrl::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the zoom according to the value in the combo box.
 */
void bf::sprite_view_ctrl::set_zoom_from_combo()
{
  long z;

  if ( m_combo_zoom->GetValue().ToLong(&z) )
    {
      if ((int)z <= 0)
        z = 0;

      m_sprite_view->set_zoom( z );
    }

  adjust_scrollbars();
} // sprite_view_ctrl::set_zoom_from_combo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the bounds and the position of the toolbars.
 */
void bf::sprite_view_ctrl::adjust_scrollbars()
{
  m_h_scrollbar->SetScrollbar
    ( m_sprite_view->get_view_position().x, m_sprite_view->GetSize().x,
      m_sprite_view->get_view_size().x,
      (3 * m_sprite_view->GetSize().x) / 4, true );
  m_v_scrollbar->SetScrollbar
    ( m_sprite_view->get_view_position().y, m_sprite_view->GetSize().y,
      m_sprite_view->get_view_size().y,
      (3 * m_sprite_view->GetSize().y) / 4, true );
} // sprite_view_ctrl::adjust_scrollbars()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Zoom 100%".
 * \param event This event occured.
 */
void bf::sprite_view_ctrl::on_zoom_100( wxCommandEvent& WXUNUSED(event) )
{
  m_combo_zoom->SetSelection( m_combo_zoom->FindString(wxT("100")) );
  m_sprite_view->set_zoom( 100 );

  adjust_scrollbars();
} // sprite_view_ctrl::on_zoom_100()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Zoom fit".
 * \param event This event occured.
 */
void bf::sprite_view_ctrl::on_zoom_fit( wxCommandEvent& WXUNUSED(event) )
{
  m_sprite_view->auto_zoom();
  m_combo_zoom->SetValue
    ( wxString::Format(wxT("%d"), m_sprite_view->get_zoom()) );

  adjust_scrollbars();
} // sprite_view_ctrl::on_zoom_fit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Zoom in".
 * \param event This event occured.
 */
void bf::sprite_view_ctrl::on_zoom_in( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_combo_zoom->GetSelection();

  if( index == wxNOT_FOUND )
    {
      m_sprite_view->set_zoom( m_sprite_view->get_zoom() + 50 );

      m_combo_zoom->SetValue
        ( wxString::Format(wxT("%d"), m_sprite_view->get_zoom()) );
    }
  else if ( (unsigned int)index + 1 < m_combo_zoom->GetCount() )
    {
      m_combo_zoom->SetSelection(index + 1);
      set_zoom_from_combo();
    }

  adjust_scrollbars();
} // sprite_view_ctrl::on_zoom_in()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on "Zoom out".
 * \param event This event occured.
 */
void bf::sprite_view_ctrl::on_zoom_out( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_combo_zoom->GetSelection();

  if( index == wxNOT_FOUND )
    {
      if ( m_sprite_view->get_zoom() > 50 )
        m_sprite_view->set_zoom( m_sprite_view->get_zoom() - 50 );
      else
        m_sprite_view->set_zoom( 1 );

      m_combo_zoom->SetValue
        ( wxString::Format(wxT("%d"), m_sprite_view->get_zoom()) );
    }
  else  if ( index > 0 )
    {
      m_combo_zoom->SetSelection(index - 1);
      set_zoom_from_combo();
    }

  adjust_scrollbars();
} // sprite_view_ctrl::on_zoom_out()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user selected a zoom in the combo box.
 * \param event This event occured.
 */
void bf::sprite_view_ctrl::on_zoom_selection( wxCommandEvent& WXUNUSED(event) )
{
  set_zoom_from_combo();
} // sprite_view_ctrl::on_zoom_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the position of the view.
 * \param event The scroll event.
 */
void bf::sprite_view_ctrl::on_scroll( wxScrollEvent& WXUNUSED(event) )
{
  m_sprite_view->set_view_delta
    ( m_h_scrollbar->GetThumbPosition(), m_v_scrollbar->GetThumbPosition() );
} // sprite_view_ctrl::on_scroll()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::sprite_view_ctrl::on_size( wxSizeEvent& event )
{
  adjust_scrollbars();
  event.Skip();
} // sprite_view_ctrl::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief The mouse move in the sprite_view.
 * \param event The mouse event that occured.
 */
void bf::sprite_view_ctrl::on_mouse_move(wxMouseEvent& event)
{
  if ( m_sprite_view != NULL )
    {
      wxPoint pos;
      std::ostringstream out;
     
      if ( m_sprite_view->convert_position(event.GetPosition(), pos ) )
        out << "( X = " << pos.x << " ; Y = " << pos.y << " )";
      else
        out << "( X = ? ; Y = ? )";

      m_mouse_position->SetLabel( std_to_wx_string(out.str()) );
    }
  else
    std::cout << "NULL" << std::endl;
} // sprite_view_ctrl::on_mouse_move()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::sprite_view_ctrl, wxPanel)
  EVT_BUTTON( bf::sprite_view_ctrl::ID_ZOOM_100,
              bf::sprite_view_ctrl::on_zoom_100 )
  EVT_BUTTON( bf::sprite_view_ctrl::ID_ZOOM_FIT,
              bf::sprite_view_ctrl::on_zoom_fit )
  EVT_BUTTON( bf::sprite_view_ctrl::ID_ZOOM_IN,
              bf::sprite_view_ctrl::on_zoom_in )
  EVT_BUTTON( bf::sprite_view_ctrl::ID_ZOOM_OUT,
              bf::sprite_view_ctrl::on_zoom_out )
  EVT_COMBOBOX( bf::sprite_view_ctrl::ID_COMBO_ZOOM,
                bf::sprite_view_ctrl::on_zoom_selection )
  EVT_TEXT_ENTER( bf::sprite_view_ctrl::ID_COMBO_ZOOM,
                  bf::sprite_view_ctrl::on_zoom_selection )
  EVT_SCROLL( bf::sprite_view_ctrl::on_scroll )
  EVT_SIZE( bf::sprite_view_ctrl::on_size )
END_EVENT_TABLE()
