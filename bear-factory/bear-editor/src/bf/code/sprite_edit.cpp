/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sprite_edit class.
 * \author Sebastien Angibaud
 */
#include "bf/sprite_edit.hpp"

#include "bf/bitmap_rendering_attributes_edit.hpp"
#include "bf/image_pool.hpp"
#include "bf/image_selection_dialog.hpp"
#include "bf/path_configuration.hpp"
#include "bf/sprite_view_ctrl.hpp"
#include "bf/wx_facilities.hpp"

#include <limits>
#include <sstream>
#include <fstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param env The workspace environment to use.
 * \param spr The initial sprite.
 */
bf::sprite_edit::sprite_edit
( wxWindow& parent, workspace_environment* env, const sprite& spr )
  : wxPanel(&parent, wxID_ANY), base_edit<sprite>(spr), 
    m_workspace(env)
{
  create_controls();
  Fit();
} // sprite_edit::sprite_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::sprite_edit::validate()
{
  if ( m_rendering_attributes->validate() )
    {
      set_value(make_sprite());
      return true;
    }
  else
    return false;
} // sprite_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite corresponding to the values in the control.
 */
bf::sprite bf::sprite_edit::make_sprite() const
{
  sprite result;

  if ( m_rendering_attributes->validate() )
    {
      result.set_left( m_left_text->GetValue() );
      result.set_top( m_top_text->GetValue() );
      result.set_clip_width( m_clip_width_text->GetValue() );
      result.set_clip_height( m_clip_height_text->GetValue() );
      result.set_spritepos_entry
        ( wx_to_std_string(m_spritepos_combo->GetStringSelection()) );
      result.set_image_name( wx_to_std_string(m_image_name_text->GetValue()) );

      result.assign(m_rendering_attributes->get_value());
    }

  return result;
} // sprite_edit::make_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::sprite_edit::value_updated()
{
  m_rendering_attributes->set_value( get_value() );
  fill_controls();
} // sprite_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the sprite.
 */
void bf::sprite_edit::fill_controls()
{
  sprite spr = get_value();

  m_left_text->SetValue( spr.get_left() );
  m_top_text->SetValue( spr.get_top() );

  m_clip_width_text->SetValue( spr.get_clip_width() );
  m_clip_height_text->SetValue( spr.get_clip_height() );
  m_image_name_text->SetValue( std_to_wx_string(spr.get_image_name()) );

  control_sprite_size();

  m_sprite_view->set_sprite( get_value() );

  fill_spritepos();
  m_spritepos_combo->SetStringSelection
    ( std_to_wx_string(spr.get_spritepos_entry()) );
} // sprite_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::sprite_edit::create_controls()
{
  m_spritepos_combo =
    new wxChoice( this, IDC_SPRITEPOS_CHOICE, wxDefaultPosition, wxDefaultSize,
                  wxArrayString() );
  m_left_text = new wxSpinCtrl( this, IDC_RECTANGLE_CLIP_CHANGE );
  m_top_text = new wxSpinCtrl( this, IDC_RECTANGLE_CLIP_CHANGE );
  m_clip_width_text = new wxSpinCtrl( this, IDC_RECTANGLE_CLIP_CHANGE );
  m_clip_height_text = new wxSpinCtrl( this, IDC_RECTANGLE_CLIP_CHANGE );

  m_left_text->SetRange( 0, std::numeric_limits<int>::max() );
  m_top_text->SetRange( 0, std::numeric_limits<int>::max() );
  m_clip_width_text->SetRange( 0, std::numeric_limits<int>::max() );
  m_clip_height_text->SetRange( 0, std::numeric_limits<int>::max() );

  m_rendering_attributes =
    new bitmap_rendering_attributes_edit(*this, get_value());

  m_image_name_text = new wxTextCtrl( this, wxID_ANY );
  m_sprite_view = new sprite_view_ctrl(*this, m_workspace, get_value());

  create_sizer_controls();
  fill_controls();
} // sprite_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::sprite_edit::create_sizer_controls()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  v_sizer->Add( m_sprite_view, 1, wxEXPAND );
  v_sizer->Add( new wxButton(this, wxID_REFRESH), 0, wxEXPAND );

  h_sizer->Add( v_sizer, 1, wxEXPAND | wxALL, 5 );

  v_sizer = new wxBoxSizer( wxVERTICAL );
  v_sizer->Add( m_spritepos_combo, 0, wxEXPAND );
  v_sizer->Add( create_position_sizer(), 0, wxEXPAND );
  v_sizer->Add( m_rendering_attributes, 0, wxEXPAND );
  v_sizer->Add( create_image_name_sizer(), 0, wxEXPAND );

  h_sizer->Add( v_sizer, 1, wxEXPAND | wxALL, 5 );

  SetSizer(h_sizer);
} // sprite_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the position part.
 */
wxSizer* bf::sprite_edit::create_position_sizer()
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxVERTICAL, this, _("Rectangle in image source") );

  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("X:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  s_sizer->Add( m_left_text, 0, wxALL, 5 );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Y:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  s_sizer->Add( m_top_text, 0, wxALL, 5 );
  result->Add( s_sizer, 0, wxEXPAND, 0 );

  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Width:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  s_sizer->Add( m_clip_width_text, 0, wxALL, 5 );
  s_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Height:")),
      1, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  s_sizer->Add( m_clip_height_text, 0, wxALL, 5 );
  result->Add( s_sizer, 0, wxEXPAND, 0 );

  return result;
} // sprite_edit::create_position_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the image name.
 */
wxSizer* bf::sprite_edit::create_image_name_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  wxButton* image_name_browse = new wxButton
    ( this, IDC_IMAGE_SELECT, wxT("..."), wxDefaultPosition, wxSize(30, -1) );

  result->Add
    ( new wxStaticText(this, wxID_ANY, _("Image:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 5 );
  result->Add( m_image_name_text, 1, wxEXPAND | wxALL, 5 );
  result->Add( image_name_browse, 0, wxALL, 5 );

  return result;
} // sprite_edit::create_image_name_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the content of the potential spritepos.
 */
void bf::sprite_edit::fill_spritepos()
{
  m_spritepos_combo->Clear();

  image_pool::spritepos_entries e =
    m_workspace->pool.get_spritepos_entries(m_image_name_text->GetValue());
  
  image_pool::spritepos_entries::const_iterator it;

  for ( it=e.begin(); it!=e.end(); ++it )
      m_spritepos_combo->Append( it->first );
} // sprite_edit::fill_spritepos()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test validity of the sprite size.
 */
void bf::sprite_edit::control_sprite_size()
{
  wxBitmap bmp =
    m_workspace->pool.get_image(m_image_name_text->GetValue());

  if ( bmp.IsOk() )
    {
      m_left_text->SetRange( 0, bmp.GetWidth() - 1 );
      m_top_text->SetRange( 0, bmp.GetHeight() - 1 );

      m_clip_width_text->SetRange
        ( 0, bmp.GetWidth() - m_left_text->GetValue() );
      m_clip_height_text->SetRange
        ( 0, bmp.GetHeight() - m_top_text->GetValue() );

      check_sprite_pos();
    }
} // sprite_edit::control_sprite_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if current rectangle is in spritepos.
 */
void bf::sprite_edit::check_sprite_pos()
{
  bool found = false;

  for ( unsigned int i=0; (i < m_spritepos_combo->GetCount()) && !found; ++i )
    {
      const claw::math::rectangle<unsigned int> r =
        m_workspace->pool.get_spritepos_entries
        ( m_image_name_text->GetValue())[m_spritepos_combo->GetString(i)];
      
      if ( ( r.position.x == (unsigned int)m_left_text->GetValue() )
           && ( r.position.y == (unsigned int)m_top_text->GetValue() )
           && ( r.width == (unsigned int)m_clip_width_text->GetValue() )
           && ( r.height == (unsigned int)m_clip_height_text->GetValue() ) )
        {
          found = true;
          m_spritepos_combo->SetSelection(i);
        }
    }

  if ( ( !found ) && ( !m_spritepos_combo->GetStringSelection().IsEmpty() ) )
    {
      m_spritepos_combo->Clear();
      fill_spritepos();
    }
} // sprite_edit::check_sprite_pos()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the button to select an image.
 * \param event This event occured.
 */
void bf::sprite_edit::on_image_select( wxCommandEvent& WXUNUSED(event) )
{
  image_selection_dialog dlg
    (*this, &m_workspace->pool, m_image_name_text->GetValue());

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_image_name_text->SetValue( dlg.get_image_name() );

      wxBitmap bmp =
        m_workspace->pool.get_image(dlg.get_image_name());

      if ( bmp.IsOk() )
        {
          bitmap_rendering_attributes att(m_rendering_attributes->get_value());

          if ( att.width() == 0 )
            att.set_width(bmp.GetWidth());

          if ( att.height() == 0 )
            att.set_height(bmp.GetHeight());

          if ( m_clip_width_text->GetValue() == 0 )
            m_clip_width_text->SetValue(bmp.GetWidth());

          if ( m_clip_height_text->GetValue() == 0 )
            m_clip_height_text->SetValue(bmp.GetHeight());

          if ( m_left_text->GetValue() + m_clip_width_text->GetValue() >
               bmp.GetWidth() )
            {
              m_left_text->SetValue(0);
              m_clip_width_text->SetValue(bmp.GetWidth());            
            }

          if ( m_top_text->GetValue() + m_clip_height_text->GetValue() >
               bmp.GetHeight() )
            {
              m_top_text->SetValue(0);
              m_clip_height_text->SetValue(bmp.GetHeight());              
            }

          control_sprite_size();

          if ( att.get_auto_size() )
            {
              att.set_width( m_clip_width_text->GetValue() );
              att.set_height( m_clip_height_text->GetValue() );
            }
          m_rendering_attributes->set_value(att);
        }

      fill_spritepos();
      check_sprite_pos();
    }
} // sprite_edit::on_image_select()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Refresh" button.
 * \param event This event occured.
 */
void bf::sprite_edit::on_refresh( wxCommandEvent& WXUNUSED(event) )
{
  m_sprite_view->set_sprite( make_sprite() );
} // sprite_edit::on_refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user change a parameter of rectangle clip.
 * \param event The event.
 */
void bf::sprite_edit::on_rectangle_clip_change( wxSpinEvent& WXUNUSED(event) )
{
  control_sprite_size();
} // sprite_edit::on_rectangle_clip_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user has selected an entry of the spritepos file.
 * \param event The event.
 */
void bf::sprite_edit::on_select_sprite_pos( wxCommandEvent& WXUNUSED(event) )
{
  //Set the maximum size to avoid control_sprite_size() to bound the changes.
  m_left_text->SetValue(0);
  m_top_text->SetValue(0);
  
  const claw::math::rectangle<unsigned int> r =
    m_workspace->pool.get_spritepos_entries
    ( m_image_name_text->GetValue())
    [m_spritepos_combo->GetStringSelection()];
  
  m_clip_width_text->SetRange( 0,r.width - m_left_text->GetValue() );
  m_clip_height_text->SetRange( 0, r.height - m_top_text->GetValue() );
  
  m_clip_width_text->SetValue(m_clip_width_text->GetMax());
  m_clip_height_text->SetValue(m_clip_height_text->GetMax());
  
  m_left_text->SetValue( r.position.x );
  m_top_text->SetValue( r.position.y );
  m_clip_width_text->SetValue( r.width );
  m_clip_height_text->SetValue( r.height );

  bitmap_rendering_attributes att(m_rendering_attributes->get_value());

  if ( att.get_auto_size() )
    {
      att.set_width( r.width );
      att.set_height( r.height );
    }

  m_rendering_attributes->set_value(att);
  m_sprite_view->set_sprite( make_sprite() );  
} // sprite_edit::on_select_sprite_pos()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::sprite_edit, wxPanel)
  EVT_BUTTON( bf::sprite_edit::IDC_IMAGE_SELECT,
              bf::sprite_edit::on_image_select )
  EVT_BUTTON( wxID_REFRESH, bf::sprite_edit::on_refresh )
  EVT_SPINCTRL( bf::sprite_edit::IDC_RECTANGLE_CLIP_CHANGE,
                bf::sprite_edit::on_rectangle_clip_change )
  EVT_CHOICE( bf::sprite_edit::IDC_SPRITEPOS_CHOICE,
              bf::sprite_edit::on_select_sprite_pos )
END_EVENT_TABLE()
