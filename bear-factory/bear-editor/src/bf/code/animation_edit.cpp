/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_edit class.
 * \author Sebastien Angibaud
 */
#include "bf/animation_edit.hpp"

#include "bf/animation_view_ctrl.hpp"
#include "bf/bitmap_rendering_attributes_edit.hpp"
#include "bf/frame_edit.hpp"
#include "bf/human_readable.hpp"
#include "bf/wx_facilities.hpp"

#include <limits>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this window.
 * \param anim The initial animation.
 */
bf::animation_edit::animation_edit( wxWindow& parent, const animation& anim )
  : wxPanel(&parent, wxID_ANY), base_edit<animation>(anim)
{
  create_controls();
  value_updated();
  Fit();
} // animation_edit::animation_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the displayed value is correct and, if it is, set the
 *        value according to the display.
 */
bool bf::animation_edit::validate()
{
  if ( m_rendering_attributes->validate() )
    {
      set_value(make_animation());
      return true;
    }
  else
    return false;
} // animation_edit::validate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation corresponding to the values in the control.
 */
bf::animation bf::animation_edit::make_animation() const
{
  animation result(get_value());

  if ( m_rendering_attributes->validate() )
    {
      result.set_loops( m_loops_spin->GetValue() );
      result.set_loop_back( m_loop_back_box->IsChecked() );
      result.set_first_index( m_first_index_spin->GetValue());
      result.set_last_index( m_last_index_spin->GetValue());

      result.bitmap_rendering_attributes::assign
        (m_rendering_attributes->get_value());

      if ( result.get_auto_size() )
        result.set_size( result.get_max_size() );
    }

  return result;
} // animation_edit::make_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after changing the value by set_value().
 */
void bf::animation_edit::value_updated()
{
  m_rendering_attributes->set_value( get_value() );
  fill_controls();
} // animation_edit::value_updated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update buttons.
 */
void bf::animation_edit::update_buttons() const
{
  m_delete_button->Enable( m_frame_list->GetSelectedItemCount() != 0 );
  m_up_button->Enable( m_frame_list->GetSelectedItemCount() == 1 );
  m_down_button->Enable( m_frame_list->GetSelectedItemCount() == 1 );
  m_edit_button->Enable( m_frame_list->GetSelectedItemCount() == 1 );
  m_copy_button->Enable( m_frame_list->GetSelectedItemCount() == 1 );
  m_reverse_button->Enable( m_frame_list->GetSelectedItemCount() > 1 );
} // animation_edit::update_buttons()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update frame list.
 * \param anim The animation.
 */
void bf::animation_edit::update_frame_list(const animation& anim) const
{
  std::list<long> index_list;
  long index = m_frame_list->GetFirstSelected();

  while ( index != wxNOT_FOUND )
    {
      index_list.push_back(index);
      index = m_frame_list->GetNextSelected(index);
    }

  animation::const_iterator it;
  unsigned int i=0;
  m_frame_list->DeleteAllItems();

  for ( it=anim.begin(); it!=anim.end(); ++i, ++it )
    {
      wxListItem prop;
      wxString string( human_readable<long>::convert(i) );
      m_frame_list->InsertItem(i, string);

      prop.SetId(i);
      m_frame_list->GetItem(prop);
      prop.SetText( human_readable<sprite>::convert(it->get_sprite()) );
      prop.SetColumn(1);
      m_frame_list->SetItem(prop);

      m_frame_list->GetItem(prop);
      prop.SetText( human_readable<double>::convert(it->get_duration()) );
      prop.SetColumn(2);
      m_frame_list->SetItem(prop);
    }

  std::list<long>::iterator it_index;
  for( it_index = index_list.begin(); it_index != index_list.end(); ++it_index )
    if ( (*it_index != wxNOT_FOUND) && (m_frame_list->GetItemCount() != 0) )
      {
        if ( *it_index > m_frame_list->GetItemCount() )
          *it_index = m_frame_list->GetItemCount() - 1;
        
        m_frame_list->Select(*it_index);
      }
} // animation_edit::update_frame_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update spin control.
 */
void bf::animation_edit::update_spin_ctrl() const
{
  if ( m_frame_list->GetItemCount() == 0 )
    m_first_index_spin->SetRange(0, 0);
  else
    m_first_index_spin->SetRange(0, m_frame_list->GetItemCount()-1);

  m_last_index_spin->SetRange( m_first_index_spin->GetValue(),
                               m_frame_list->GetItemCount()-1);
} // animation_edit::update_spin_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the total duration of the animation.
 */
void bf::animation_edit::update_duration() const
{
  m_duration_label->SetLabel
    ( wxString::Format
      ( _("Total duration: %.3f ms."), get_value().get_duration() ) );
} // animation_edit::update_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ajust the size of the last column so there is no empty space on the
 *        right.
 */
void bf::animation_edit::adjust_last_column_size()
{
  m_frame_list->SetColumnWidth
    ( 1, m_frame_list->GetSize().x - m_frame_list->GetColumnWidth(0) -
      m_frame_list->GetColumnWidth(2) );
} // animation_edit::adjust_last_column_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls with the values of the animation.
 */
void bf::animation_edit::fill_controls() const
{
  animation anim( get_value() );
  
  update_frame_list(anim);
  update_buttons();
  update_spin_ctrl();
  update_duration();

  m_loop_back_box->SetValue( anim.get_loop_back() );
  m_loops_spin->SetValue( anim.get_loops() );
  m_first_index_spin->SetValue( anim.get_first_index() );
  m_last_index_spin->SetValue( anim.get_last_index() );

  m_animation_view->set_animation( anim );
} // animation_edit::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::animation_edit::create_controls()
{
  m_frame_list = new wxListView( this, IDC_FRAME, wxDefaultPosition,
         wxDefaultSize, wxLC_REPORT | wxLC_VRULES );

  m_frame_list->InsertColumn(0, _("Index"));
  m_frame_list->InsertColumn(1, _("Sprite"));
  m_frame_list->InsertColumn(2, _("Duration"));

  m_copy_button = new wxButton(this, wxID_COPY);
  m_edit_button = new wxButton(this, wxID_EDIT);
  m_up_button = new wxButton(this, wxID_UP);
  m_down_button = new wxButton(this, wxID_DOWN);
  m_delete_button = new wxButton(this, wxID_DELETE);
  m_reverse_button = new wxButton(this, IDC_REVERSE, _("Reverse"));

  m_loops_spin =
    new spin_ctrl<unsigned int>( this, wxID_ANY, wxDefaultPosition,
         wxDefaultSize, wxEXPAND, 0,
         std::numeric_limits<unsigned int>::max(), 1 );

  m_first_index_spin = new wxSpinCtrl( this, IDC_BOUND_INDEX_CHANGE );
  m_last_index_spin = new wxSpinCtrl( this, IDC_BOUND_INDEX_CHANGE );
  m_loop_back_box = new wxCheckBox( this, wxID_ANY, _("Loop_back") );

  m_rendering_attributes =
    new bitmap_rendering_attributes_edit(*this, get_value());

  m_animation_view = new animation_view_ctrl(*this, get_value());

  m_duration_label = new wxStaticText( this, wxID_ANY, wxEmptyString );

  create_sizer_controls();
} // animation_edit::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::animation_edit::create_sizer_controls()
{
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

  v_sizer->Add( new wxButton(this, wxID_NEW), 1, wxEXPAND | wxALL, 0 );
  v_sizer->Add( m_copy_button, 1, wxEXPAND | wxALL, 0 );
  v_sizer->Add( m_edit_button, 1, wxEXPAND | wxALL, 0 );
  v_sizer->Add( m_delete_button, 1, wxEXPAND | wxALL, 0 );
  v_sizer->Add( m_up_button, 1, wxEXPAND | wxALL, 0 );
  v_sizer->Add( m_down_button, 1, wxEXPAND | wxALL, 0 );
  v_sizer->Add( m_reverse_button, 1, wxEXPAND | wxALL, 0 );

  h_sizer->Add( m_frame_list, 4, wxEXPAND );
  h_sizer->Add( v_sizer, 1, wxEXPAND );

  v_sizer = new wxBoxSizer( wxVERTICAL );
  v_sizer->Add( h_sizer, 1, wxEXPAND );
  v_sizer->Add( create_loops_sizer(), 0, wxEXPAND );
  v_sizer->Add( m_duration_label, 0, wxEXPAND | wxALL, 2);
  v_sizer->Add( m_rendering_attributes, 0, wxEXPAND );

  h_sizer = new wxBoxSizer( wxHORIZONTAL );
  wxBoxSizer* s_v_sizer = new wxBoxSizer( wxVERTICAL );

  s_v_sizer->Add( m_animation_view, 1, wxEXPAND );
  s_v_sizer->Add( new wxButton(this, wxID_REFRESH), 0, wxEXPAND );
  h_sizer->Add( s_v_sizer, 1, wxEXPAND );
  h_sizer->Add( v_sizer, 1, wxEXPAND );

  SetSizer(h_sizer);
} // animation_edit::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and the sizer for the loops part.
 */
wxSizer* bf::animation_edit::create_loops_sizer()
{
  wxBoxSizer* result = new wxBoxSizer( wxHORIZONTAL );

  wxStaticBoxSizer* s_sizer =
    new wxStaticBoxSizer( wxVERTICAL, this, _("Loops") );

  wxBoxSizer* s_h_sizer = new wxBoxSizer( wxHORIZONTAL );

  //number
  wxBoxSizer* s_v_sizer = new wxBoxSizer( wxVERTICAL );
  s_v_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Number:")),
      0, wxALIGN_CENTRE_VERTICAL, 0 );
  s_v_sizer->Add( m_loops_spin, 0, wxALIGN_CENTRE_VERTICAL , 0 );
  s_h_sizer->Add( s_v_sizer, 1, wxALIGN_CENTRE_VERTICAL , 0 );


  //First index
  s_v_sizer = new wxBoxSizer( wxVERTICAL );
  s_v_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("First index:")),
      0, wxALIGN_CENTRE_VERTICAL, 0 );
  s_v_sizer->Add( m_first_index_spin, 0, wxALIGN_CENTRE_VERTICAL, 0 );
  s_h_sizer->Add( s_v_sizer, 1, wxALIGN_CENTRE_VERTICAL , 0 );

  //Last index
  s_v_sizer = new wxBoxSizer( wxVERTICAL );
  s_v_sizer->Add
    ( new wxStaticText(this, wxID_ANY, _("Last index:")),
      0, wxALIGN_CENTRE_VERTICAL | wxALL, 0 );
  s_v_sizer->Add( m_last_index_spin, 0, wxALIGN_CENTRE_VERTICAL, 0 );
  s_h_sizer->Add( s_v_sizer, 1, wxALIGN_CENTRE_VERTICAL , 0 );

  //loop back
  s_h_sizer->Add( m_loop_back_box, 1, wxALIGN_CENTRE_VERTICAL , 0 );

  s_sizer->Add( s_h_sizer, 1, wxEXPAND , 0 );

  result->Add( s_sizer, 1, wxEXPAND , 0 );

  return result;
} // animation_edit::create_loops_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Edit a frame.
 * \param index The index of the frame to edit.
 */
void bf::animation_edit::edit_frame( long index )
{
  CLAW_PRECOND( index < (long)get_value().frames_count() );

  animation anim = get_value();
  animation_frame& frm = anim.get_frame(index);

  frame_edit dlg(*m_parent, frm );

  if ( dlg.ShowModal() == wxID_OK )
    {
      frm.set_sprite(dlg.get_frame().get_sprite());
      frm.set_duration(dlg.get_frame().get_duration());

      set_value(anim);
    }
} // animation_edit::edit_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Up" button.
 * \param event The event.
 */
void bf::animation_edit::on_up( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_frame_list->GetSelectedItemCount() == 1 )
    {
      int index = m_frame_list->GetFocusedItem();
      
      if ( index != wxNOT_FOUND )
        if ( index > 0 )
          {
            animation anim = get_value();
            anim.move_backward(index);
            set_value(anim);
            
            m_frame_list->Select(index,false);
            m_frame_list->Select(index-1);
            m_frame_list->Focus(index-1);
          }
    }
} // animation_edit::on_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Down" button.
 * \param event The event.
 */
void bf::animation_edit::on_down( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_frame_list->GetSelectedItemCount() == 1 )
    {
      long index = m_frame_list->GetFocusedItem();

      if ( index != wxNOT_FOUND )
        if ( index + 1 < m_frame_list->GetItemCount() )
          {
            animation anim = get_value();
            anim.move_forward(index);
            set_value(anim);
            
            m_frame_list->Select(index,false);
            m_frame_list->Select(index+1);
            m_frame_list->Focus(index+1);
          }
    }
} // animation_edit::on_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "New" button.
 * \param event The event.
 */
void bf::animation_edit::on_new( wxCommandEvent& WXUNUSED(event) )
{
  frame_edit dlg(*m_parent);

  if (dlg.ShowModal() == wxID_OK )
    {
      animation anim = get_value();
      animation_frame& f = anim.add_frame();
      f.set_sprite(dlg.get_frame().get_sprite());
      f.set_duration(dlg.get_frame().get_duration());

      if ( get_value().empty() ) // the user is creating its first frame
        anim.set_size( f.get_sprite().get_size() );

      set_value(anim);
    }
} // animation_edit::on_new()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Copy" button.
 * \param event The event.
 */
void bf::animation_edit::on_copy( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_frame_list->GetSelectedItemCount() == 1 )
    {
      long index = m_frame_list->GetFocusedItem();

      if ( index != wxNOT_FOUND )
        {
          animation anim = get_value();
          animation_frame& f = anim.add_frame();
          f = anim.get_frame(index);
          set_value(anim);

          m_frame_list->Select(index, false);
          m_frame_list->Select(m_frame_list->GetItemCount() - 1);
          m_frame_list->Focus(m_frame_list->GetItemCount() - 1);
        }
    }
} // animation_edit::on_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the frame_list.
 * \param event The event.
 */
void bf::animation_edit::on_frame_edit(wxListEvent& event)
{
  long index = event.GetIndex();

  if ( index != wxNOT_FOUND )
    edit_frame(index);
} // animation_edit::on_frame_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user selects on the frame_list.
 * \param event The event.
 */
void bf::animation_edit::on_frame_selected(wxListEvent& event)
{
  update_buttons();
} // animation_edit::on_frame_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user deselects on the frame_list.
 * \param event The event.
 */
void bf::animation_edit::on_frame_deselected(wxListEvent& event)
{
  update_buttons();
} // animation_edit::on_frame_deselected()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Edit" button.
 * \param event The event.
 */
void bf::animation_edit::on_frame_button_edit( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_frame_list->GetSelectedItemCount() == 1 )
    {
      long index = m_frame_list->GetFocusedItem();

      if ( index != wxNOT_FOUND )
        edit_frame(index);
    }
} // animation_edit::on_frame_button_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Delete" button.
 * \param event The event.
 */
void bf::animation_edit::on_delete( wxCommandEvent& WXUNUSED(event) )
{
  std::list<long> index_list;
  long index = m_frame_list->GetFirstSelected();

  while ( index != wxNOT_FOUND )
    {
      index_list.push_back(index);
      index = m_frame_list->GetNextSelected(index);
    }
  
  if ( ! index_list.empty() )
    {
      animation anim = get_value();
      long gap = 0;

      std::list<long>::iterator it_index;
      for( it_index = index_list.begin(); 
           it_index != index_list.end(); ++it_index )
        {            
          anim.delete_frame(*it_index - gap);
          gap = gap + 1;
        }

      set_value(anim);
    }
} // animation_edit::on_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Reverse" button.
 * \param event The event.
 */
void bf::animation_edit::on_reverse( wxCommandEvent& WXUNUSED(event) )
{
  std::list<unsigned int> index_list;
  long index = m_frame_list->GetFirstSelected();

  while ( index != wxNOT_FOUND )
    {
      index_list.push_back(index);
      index = m_frame_list->GetNextSelected(index);
    }
  
  if ( ! index_list.empty() )
    {
      animation anim = get_value();
      anim.reverse_frames(index_list);
      set_value(anim);
    }
} // animation_edit::on_reverse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::animation_edit::on_size(wxSizeEvent& event)
{
  adjust_last_column_size();
  event.Skip();
} // animation_edit::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user starts to resize a column.
 * \param event The event.
 */
void bf::animation_edit::on_column_begin_drag(wxListEvent& event)
{
  if ( event.GetColumn() + 1 == m_frame_list->GetColumnCount() )
    event.Veto();
  else
    event.Skip();
} // animation_edit::on_column_begin_drag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user ends resizing a column.
 * \param event The event.
 */
void bf::animation_edit::on_column_end_drag( wxListEvent& WXUNUSED(event) )
{
  adjust_last_column_size();
} // animation_edit::on_column_begin_drag()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user change the first or the last index.
 * \param event The event.
 */
void bf::animation_edit::on_bound_index_change( wxSpinEvent& WXUNUSED(event) )
{
  update_spin_ctrl();
  update_duration();
} // animation_edit::on_bound_index_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the "Refresh" button.
 * \param event This event occured.
 */
void bf::animation_edit::on_refresh( wxCommandEvent& WXUNUSED(event) )
{
  validate();
  m_animation_view->set_animation( get_value() );
} // animation_edit::on_refresh()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::animation_edit, wxPanel)
  EVT_BUTTON( wxID_UP, bf::animation_edit::on_up )
  EVT_BUTTON( wxID_DOWN, bf::animation_edit::on_down )
  EVT_BUTTON( wxID_NEW, bf::animation_edit::on_new )
  EVT_BUTTON( wxID_COPY, bf::animation_edit::on_copy )
  EVT_BUTTON( wxID_EDIT, bf::animation_edit::on_frame_button_edit )
  EVT_BUTTON( IDC_REVERSE, bf::animation_edit::on_reverse )
  EVT_SIZE( bf::animation_edit::on_size )
  EVT_LIST_COL_BEGIN_DRAG
    ( bf::animation_edit::IDC_FRAME,
      bf::animation_edit::on_column_begin_drag )
  EVT_LIST_COL_END_DRAG
    ( bf::animation_edit::IDC_FRAME,
      bf::animation_edit::on_column_end_drag )
  EVT_LIST_ITEM_ACTIVATED( IDC_FRAME, bf::animation_edit::on_frame_edit )
  EVT_LIST_ITEM_SELECTED( IDC_FRAME, bf::animation_edit::on_frame_selected )
  EVT_LIST_ITEM_DESELECTED( IDC_FRAME, bf::animation_edit::on_frame_deselected )
  EVT_BUTTON( wxID_DELETE, bf::animation_edit::on_delete )
  EVT_SPINCTRL( bf::animation_edit::IDC_BOUND_INDEX_CHANGE,
                bf::animation_edit::on_bound_index_change )
  EVT_BUTTON( wxID_REFRESH, bf::animation_edit::on_refresh )
END_EVENT_TABLE()

