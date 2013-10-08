/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::snapshot_frame class.
 * \author Sébastien Angibaud
 */
#include "bf/snapshot_frame.hpp"

#include "bf/accordion.hpp"
#include "bf/easing_combo.hpp"
#include "bf/gui_model.hpp"
#include "bf/mark.hpp"
#include "bf/model_frame.hpp"
#include "bf/snapshot.hpp"
#include "bf/snapshot_date_frame.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/history/action_modify_placement.hpp"
#include "bf/history/action_add_snapshot.hpp"
#include "bf/history/action_move_snapshot.hpp"
#include "bf/history/action_remove_snapshot.hpp"
#include "bf/history/action_modify_function.hpp"
#include "bf/history/action_modify_snapshot_alignment.hpp"
#include "bf/history/action_modify_snapshot_size.hpp"
#include "bf/history/action_modify_snapshot_sound.hpp"
#include "bf/history/action_set_angle_easing.hpp"
#include "bf/history/action_set_height_easing.hpp"
#include "bf/history/action_set_placement_function.hpp"
#include "bf/history/action_set_width_easing.hpp"
#include "bf/history/action_set_x_easing.hpp"
#include "bf/history/action_set_y_easing.hpp"
#include "bf/history/action_group.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The frame owning this one.
 */
bf::snapshot_frame::snapshot_frame( wxWindow* parent )
  : wxPanel( parent ), m_model_frame(NULL), m_snapshot(NULL)
{
  create_controls();
} // snapshot_frame::snapshot_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the model on which we work.
 * \param f The model frame in which we take the actions.
 */
void bf::snapshot_frame::set_model_frame( model_frame* f )
{
  m_model_frame = f;

  if ( m_model_frame )
    m_sound_frame->set_workspace( m_model_frame->get_workspace() );
  
  fill();
} // snapshot_frame::set_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update when the current action is modified.
 */
void bf::snapshot_frame::update_action( )
{
  fill();
} // snapshot_frame::update_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update when the current snapshot is modified.
 */
void bf::snapshot_frame::update_snapshot( )
{
  fill();
} // snapshot_frame::update_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update when the current mark is modified.
 */
void bf::snapshot_frame::update_mark( )
{
  fill();
} // snapshot_frame::update_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::snapshot_frame::create_controls()
{
  accordion* a = new accordion( this, wxVERTICAL );

  create_member_controls(a);
  create_sizer_controls(a);
} // snapshot_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 * \param a The accordion in which the groups are added.
 */
void bf::snapshot_frame::create_member_controls( accordion* a )
{
  m_action_choice = new wxChoice( this, IDC_ACTION_CHOICE );
  
  create_snapshot_controls(a);
  create_marks_controls(a);
  create_sound_controls(a);
  create_item_box_controls(a);

  enable_controls(false);
} // snapshot_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 * \param a The accordion in which the groups are added.
 */
void bf::snapshot_frame::create_sizer_controls( accordion* a )
{
  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  a->add( create_snapshot_sizer(a), _("Snapshot") );
  a->add( create_marks_sizer(a), _("Mark") );
  a->add( create_sound_sizer(a), _("Sound") );
  a->add( create_item_box_sizer(a), _("Bounding box") );

  sizer->Add( create_action_sizer(this), 0, wxEXPAND );
  sizer->Add( a, 1, wxEXPAND );
  SetSizer(sizer);
} // snapshot_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the snapshot.
 * \param parent The parent window of the created controls.
 */
void bf::snapshot_frame::create_snapshot_controls( wxWindow* parent )
{
  m_date_button = new wxButton( parent, IDC_DATE_CTRL, wxT("0"));
  m_function_button = new wxButton( parent, IDC_FUNCTION, wxEmptyString );
} // snapshot_frame::create_snapshot_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the marks.
 * \param parent The parent window of the created controls.
 */
void bf::snapshot_frame::create_marks_controls( wxWindow* parent )
{
  m_modify_whole_action =
    new wxCheckBox( parent, wxID_ANY, _("Modify all the snapshots") );
  m_mark_choice = new wxChoice( parent, IDC_MARK_CHOICE );

  m_mark_left_spin =
    new spin_ctrl<double>
    ( parent, IDC_MARK_X, wxDefaultPosition, wxDefaultSize, 0,
      -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 1 );
  m_mark_bottom_spin =
    new spin_ctrl<double>
    ( parent, IDC_MARK_Y, wxDefaultPosition, wxDefaultSize, 0,
      -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 1  );
  m_mark_width_spin =
    new spin_ctrl<double>
    ( parent, IDC_MARK_WIDTH, wxDefaultPosition, wxDefaultSize, 0,
      -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 1 );
  m_mark_height_spin =
    new spin_ctrl<double>
    ( parent, IDC_MARK_HEIGHT, wxDefaultPosition, wxDefaultSize, 0,
      -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 1  );
  m_mark_depth_spin = new wxSpinCtrl( parent, IDC_MARK_DEPTH );
  m_mark_angle_spin =
    new spin_ctrl<double>
    ( parent, IDC_MARK_ANGLE, wxDefaultPosition, wxDefaultSize, 0,
      -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 0.01 );

  m_mark_visible_box = new wxCheckBox( parent, IDC_VISIBILITY, _("Visible") );
  m_collision_function_button =
    new wxButton( parent, IDC_MARK_FUNCTION, wxEmptyString );
  m_mark_depth_spin->SetRange
    ( std::numeric_limits<int>::min(), std::numeric_limits<int>::max() );

  m_mark_x_easing = create_easing_combo( parent );
  m_mark_x_easing->GetPopupControl()->GetControl()->Connect
    ( wxEVT_COMMAND_COMBOBOX_SELECTED,
      wxCommandEventHandler(snapshot_frame::on_x_easing), NULL, this );

  m_mark_y_easing = create_easing_combo( parent );
  m_mark_y_easing->GetPopupControl()->GetControl()->Connect
    ( wxEVT_COMMAND_COMBOBOX_SELECTED,
      wxCommandEventHandler(snapshot_frame::on_y_easing), NULL, this );

  m_mark_width_easing = create_easing_combo( parent );
  m_mark_width_easing->GetPopupControl()->GetControl()->Connect
    ( wxEVT_COMMAND_COMBOBOX_SELECTED,
      wxCommandEventHandler(snapshot_frame::on_width_easing), NULL, this );

  m_mark_height_easing = create_easing_combo( parent );
  m_mark_height_easing->GetPopupControl()->GetControl()->Connect
    ( wxEVT_COMMAND_COMBOBOX_SELECTED,
      wxCommandEventHandler(snapshot_frame::on_height_easing), NULL, this );

  m_mark_angle_easing = create_easing_combo( parent );
  m_mark_angle_easing->GetPopupControl()->GetControl()->Connect
    ( wxEVT_COMMAND_COMBOBOX_SELECTED,
      wxCommandEventHandler(snapshot_frame::on_angle_easing), NULL, this );
} // snapshot_frame::create_marks_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the sound.
 * \param parent The parent window of the created controls.
 */
void bf::snapshot_frame::create_sound_controls( wxWindow* parent )
{
  m_sound_frame = new sound_frame( parent, IDC_SOUND );
} // snapshot_frame::create_sound_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the item box.
 * \param parent The parent window of the created controls.
 */
void bf::snapshot_frame::create_item_box_controls( wxWindow* parent )
{
  m_width_spin = new spin_ctrl<double>
    ( parent, IDC_SIZE, wxDefaultPosition,
      wxDefaultSize, 0, 0, std::numeric_limits<double>::infinity(), 0, 1 );
  m_height_spin = new spin_ctrl<double>
    ( parent, IDC_SIZE, wxDefaultPosition,
      wxDefaultSize, 0, 0, std::numeric_limits<double>::infinity(), 0, 1 );

   m_x_alignment_value_spin = new spin_ctrl<double>
    ( parent, IDC_ALIGNMENT, wxDefaultPosition,
      wxDefaultSize, 0, -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 1 );

  m_y_alignment_value_spin = new spin_ctrl<double>
    ( parent, IDC_ALIGNMENT, wxDefaultPosition,
      wxDefaultSize, 0, -std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity(), 0, 1 );

  wxArrayString array;
  array.Add( std_to_wx_string( snapshot::vertical_alignment::align_bottom) );
  array.Add( std_to_wx_string( snapshot::vertical_alignment::align_center) );
  array.Add( std_to_wx_string( snapshot::vertical_alignment::align_top) );
  m_y_alignment_choice = new wxChoice
    ( parent, IDC_ALIGNMENT, wxDefaultPosition, wxDefaultSize, array );

  array.Clear();
  array.Add( std_to_wx_string( snapshot::horizontal_alignment::align_left) );
  array.Add( std_to_wx_string( snapshot::horizontal_alignment::align_center) );
  array.Add( std_to_wx_string( snapshot::horizontal_alignment::align_right) );

  m_x_alignment_choice = new wxChoice
    ( parent, IDC_ALIGNMENT, wxDefaultPosition, wxDefaultSize, array );
} // snapshot_frame::create_item_box_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer for the action part.
 * \param parent The parent window of the created controls.
 */
wxSizer* bf::snapshot_frame::create_action_sizer( wxWindow* parent )
{
  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Action:")), 0,
      wxALIGN_CENTRE_VERTICAL | wxALL );
  sizer->Add( m_action_choice, 1, wxALL );

  return sizer;
} // snapshot_frame::create_action_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer for the snapshot part part.
 * \param parent The parent window of the created controls.
 */
wxSizer* bf::snapshot_frame::create_snapshot_sizer( wxWindow* parent )
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxVERTICAL, parent, _("Snapshot") );

  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Date:")), 0,
      wxALIGN_CENTER_VERTICAL );
  sizer->Add( m_date_button, 1, wxEXPAND );
  result->Add( sizer, 0, wxEXPAND );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Function:")), 0,
      wxALIGN_CENTER_VERTICAL );
  sizer->Add( m_function_button, 1, wxEXPAND );
  result->Add( sizer, 0, wxEXPAND );

  return result;
} // snapshot_frame::create_snapshot_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer for the marks part.
 * \param parent The parent window of the created controls.
 */
wxSizer* bf::snapshot_frame::create_marks_sizer( wxWindow* parent )
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxVERTICAL, parent, _("Marks") );

  result->Add( m_modify_whole_action, 0, wxEXPAND );

  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Mark:")), 0,
      wxALIGN_CENTRE_VERTICAL | wxALL );
  sizer->Add( m_mark_choice, 1, wxALL );
  sizer->Add( m_mark_visible_box, 0, wxALIGN_CENTRE_VERTICAL | wxALL );
  result->Add( sizer, 0, wxEXPAND );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("X:")), 0, wxALIGN_CENTRE_VERTICAL );
  sizer->Add( m_mark_left_spin, 1 );
  sizer->Add( m_mark_x_easing, 1 );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Y:")), 0,
      wxALIGN_CENTRE_VERTICAL | wxLEFT, 5 );
  sizer->Add( m_mark_bottom_spin, 1 );
  sizer->Add( m_mark_y_easing, 1 );
  result->Add( sizer, 0, wxEXPAND | wxTOP, 3 );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText
      (parent, wxID_ANY, _("Width:")), 0, wxALIGN_CENTRE_VERTICAL );
  sizer->Add( m_mark_width_spin, 1 );
  sizer->Add( m_mark_width_easing, 1 );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Height:")), 0,
      wxALIGN_CENTRE_VERTICAL | wxLEFT, 5 );
  sizer->Add( m_mark_height_spin, 1 );
  sizer->Add( m_mark_height_easing, 1 );
  result->Add( sizer, 0, wxEXPAND | wxTOP, 3 );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Depth:")), 0,
      wxALIGN_CENTRE_VERTICAL );
  sizer->Add( m_mark_depth_spin, 1 );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Angle:")), 0,
      wxALIGN_CENTRE_VERTICAL | wxLEFT, 5 );
  sizer->Add( m_mark_angle_spin, 1 );
  sizer->Add( m_mark_angle_easing, 1 );
  result->Add( sizer, 0, wxEXPAND | wxTOP, 3 );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Collision function:")), 0,
      wxALIGN_CENTRE_VERTICAL );
  sizer->Add( m_collision_function_button, 1 );
  result->Add( sizer, 0, wxEXPAND | wxTOP, 3 );

  return result;
} // snapshot_frame::create_marks_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer for sound part.
 * \param parent The parent window of the created controls.
 */
wxSizer* bf::snapshot_frame::create_sound_sizer( wxWindow* parent )
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxVERTICAL, parent, _("Sound") );

  result->Add( m_sound_frame, 1, wxEXPAND );

  return result;
} // snapshot_frame::create_sound_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizer for bounding box part.
 * \param parent The parent window of the created controls.
 */
wxSizer* bf::snapshot_frame::create_item_box_sizer( wxWindow* parent )
{
  wxStaticBoxSizer* result =
    new wxStaticBoxSizer( wxVERTICAL, parent, _("Item box") );

  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Width:")), 0,
      wxALIGN_CENTER_VERTICAL );
  sizer->Add( m_width_spin, 1 );
  sizer->AddSpacer(5);
  sizer->Add
    ( new wxStaticText(parent, wxID_ANY, _("Height:")), 0,
      wxALIGN_CENTER_VERTICAL );
  sizer->Add( m_height_spin, 1 );
  result->Add( sizer, 0, wxEXPAND );

  result->Add
    ( new wxStaticText(parent, wxID_ANY, _("Horizontal alignment policy:")), 0,
      wxALIGN_CENTER_VERTICAL | wxTOP, 3 );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add( m_x_alignment_choice, 1 );
  sizer->AddSpacer(5);
  sizer->Add
    ( new wxStaticText
      (parent, wxID_ANY, _("Gap:")), 0, wxALIGN_CENTER_VERTICAL );
  sizer->Add( m_x_alignment_value_spin, 1 );
  result->Add( sizer, 0, wxEXPAND );

  result->Add
    ( new wxStaticText(parent, wxID_ANY, _("Vertical alignment policy:")), 0,
      wxALIGN_CENTER_VERTICAL | wxTOP, 3 );

  sizer = new wxBoxSizer( wxHORIZONTAL );
  sizer->Add( m_y_alignment_choice, 1 );
  sizer->AddSpacer(5);
  sizer->Add
    ( new wxStaticText
      (parent, wxID_ANY, _("Gap:")), 0, wxALIGN_CENTER_VERTICAL );
  sizer->Add( m_y_alignment_value_spin, 1 );

  result->Add( sizer, 0, wxEXPAND );

  return result;
} // snapshot_frame::create_item_box_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a combo for the easing functions.
 * \param parent The parent control of the combo.
 */
wxComboCtrl*
bf::snapshot_frame::create_easing_combo( wxWindow* parent )
{
  return bf::create_easing_combo( parent );
} // snapshot_frame::create_easing_combo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list with the marks of the model.
 */
void bf::snapshot_frame::fill()
{
  bool enable = false;
  m_snapshot = NULL;

  if ( m_model_frame != NULL )
    if ( m_model_frame->get_model().has_active_action() )
      {
        m_snapshot = m_model_frame->get_model().get_selected_snapshot();

        if ( m_snapshot != NULL )
          {
            enable = true;

            m_date_button->SetLabel
              ( wxString::Format(wxT("%.3f"), m_snapshot->get_date()) );

            m_function_button->SetLabel
              (std_to_wx_string(m_snapshot->get_function()));

            m_function_button->SetLabel
              (std_to_wx_string(m_snapshot->get_function()));

            m_sound_frame->set_sound_description( m_snapshot->get_sound() );

            m_width_spin->SetValue(m_snapshot->get_width());
            m_height_spin->SetValue(m_snapshot->get_height());
            m_x_alignment_choice->SetStringSelection
              (std_to_wx_string(m_snapshot->get_x_alignment()));
            m_y_alignment_choice->SetStringSelection
              (std_to_wx_string(m_snapshot->get_y_alignment()));
            m_x_alignment_value_spin->SetValue
              (m_snapshot->get_x_alignment_value());
            m_y_alignment_value_spin->SetValue
              (m_snapshot->get_y_alignment_value());
          }
      }

  enable_controls(enable);

  fill_mark();
  fill_action();
} // snapshot_frame::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill controls of current mark.
 */
void bf::snapshot_frame::fill_mark()
{
  bool enable = false;

  m_mark_choice->Clear();

  if ( (m_model_frame != NULL)
       && m_model_frame->get_model().has_active_action()
       && (m_snapshot != NULL) )
    {
      wxArrayString arr;
      const snapshot::const_mark_placement_iterator end_it = 
        m_snapshot->mark_placement_end();
      snapshot::const_mark_placement_iterator it;
      
      for ( it = m_snapshot->mark_placement_begin(); it != end_it; ++it )
        arr.Add( std_to_wx_string( it->get_mark()->get_label() ) );
          
      arr.Sort();
      m_mark_choice->Append(arr);

      if ( m_model_frame->get_model().has_mark_selection() )
        {
          enable = true;
          const mark* m
            ( m_model_frame->get_model().get_main_mark_selection() );
          
          m_mark_choice->SetStringSelection
            ( std_to_wx_string(m->get_label()) );

          mark_placement p = m_snapshot->get_placement(m->get_label());

          m_mark_left_spin->SetValue(p.get_x_position());
          m_mark_x_easing->SetValue
            ( std_to_wx_string(p.get_x_easing().to_string()) );
          
          m_mark_bottom_spin->SetValue(p.get_y_position());
          m_mark_y_easing->SetValue
            ( std_to_wx_string(p.get_y_easing().to_string()) );

          m_mark_width_spin->SetValue(p.get_width());
          m_mark_width_easing->SetValue
            ( std_to_wx_string(p.get_width_easing().to_string()) );

          m_mark_height_spin->SetValue(p.get_height());
          m_mark_height_easing->SetValue
            ( std_to_wx_string(p.get_height_easing().to_string()) );

          m_mark_depth_spin->SetValue(p.get_depth_position());

          m_mark_angle_spin->SetValue(p.get_angle());
          m_mark_angle_easing->SetValue
            ( std_to_wx_string(p.get_angle_easing().to_string()) );
          
          m_mark_visible_box->SetValue(p.is_visible());
          m_collision_function_button->SetLabel
            ( std_to_wx_string(p.get_collision_function()) );
        }
    }

  m_modify_whole_action->Enable(enable);
  m_mark_left_spin->Enable(enable);
  m_mark_bottom_spin->Enable(enable);
  m_mark_width_spin->Enable(enable);
  m_mark_height_spin->Enable(enable);
  m_mark_depth_spin->Enable(enable);
  m_mark_angle_spin->Enable(enable);
  m_mark_visible_box->Enable(enable);
  m_collision_function_button->Enable(enable);

  m_mark_x_easing->Enable(enable);
  m_mark_y_easing->Enable(enable);
  m_mark_width_easing->Enable(enable);
  m_mark_height_easing->Enable(enable);
  m_mark_angle_easing->Enable(enable);
} // snapshot_frame::fill_mark()


/*----------------------------------------------------------------------------*/
/**
 * \brief Fill controls of current action.
 */
void bf::snapshot_frame::fill_action()
{
  m_action_choice->Clear();

  if ( m_model_frame != NULL)
    {
      wxArrayString arr;

      const model::const_action_iterator end_it = 
        m_model_frame->get_model().action_end();
      model::const_action_iterator it;
      
      for ( it = m_model_frame->get_model().action_begin(); it != end_it; ++it )
        arr.Add( std_to_wx_string( it->get_name() ) );
          
      arr.Sort();
      m_action_choice->Append(arr);

      if ( m_model_frame->get_model().has_active_action() )
        m_action_choice->SetStringSelection
          ( std_to_wx_string
            ( m_model_frame->get_model().get_active_action().get_name() ) );
    }
} // snapshot_frame::fill_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief The alignment has changed.
 */
void bf::snapshot_frame::alignment_change()
{
  m_model_frame->do_action
    ( new action_modify_snapshot_alignment
      (m_snapshot,
       wx_to_std_string(m_x_alignment_choice->GetStringSelection()),
       m_x_alignment_value_spin->GetValue(),
       wx_to_std_string(m_y_alignment_choice->GetStringSelection()),
       m_y_alignment_value_spin->GetValue() ) );
} // snapshot_frame::alignment_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The size has changed.
 */
void bf::snapshot_frame::size_change()
{
  m_model_frame->do_action
    ( new action_modify_snapshot_size
      (m_snapshot, m_width_spin->GetValue(), m_height_spin->GetValue() ) );
} // snapshot_frame::size_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The sound has changed.
 */
void bf::snapshot_frame::sound_change()
{
  m_model_frame->do_action
    ( new action_modify_snapshot_sound
      ( m_snapshot, m_sound_frame->get_sound_description() ) );
} // snapshot_frame::sound_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enable or disable the controls.
 * \param enable Tell if the controls have to be enabled.
 */
void bf::snapshot_frame::enable_controls(bool enable)
{
  m_date_button->Enable(enable);
  m_mark_choice->Enable(enable);
  m_modify_whole_action->Enable(enable);
  m_function_button->Enable(enable);
  m_mark_visible_box->Enable(enable);
  m_sound_frame->Enable(enable);
  m_width_spin->Enable(enable);
  m_height_spin->Enable(enable);
  m_x_alignment_choice->Enable(enable);
  m_y_alignment_choice->Enable(enable);
  m_x_alignment_value_spin->Enable(enable);
  m_y_alignment_value_spin->Enable(enable);

  m_mark_x_easing->Enable(enable);
  m_mark_y_easing->Enable(enable);
  m_mark_width_easing->Enable(enable);
  m_mark_height_easing->Enable(enable);
  m_mark_angle_easing->Enable(enable);
} // snapshot_frame::enable_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::snapshot_frame::on_close(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
} // snapshot_frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief The date button is pressed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_date_change( wxCommandEvent& WXUNUSED(event) )
{
  action& act = m_model_frame->get_model().get_active_action();
  snapshot_date_frame dlg(this, &act);

  dlg.fill_from( m_snapshot );

  if ( dlg.ShowModal() == wxID_OK )
    {
      double new_date = dlg.get_date();
      double initial_date = m_snapshot->get_date();

      if ( new_date != initial_date )
        {
          action_group* group = new action_group();
          group->set_description(_("Move snapshot"));

          if ( act.has_snapshot_date( new_date ) )
            group->add_action
              ( new action_remove_snapshot
                ( act.get_name(),
                  act.get_snapshot_before_or_at_date(new_date)));

          group->add_action
            ( new action_move_snapshot(m_snapshot, new_date) );

          if ( initial_date == 0 )
            {
              snapshot* s =
                new snapshot
                ( *act.get_snapshot_before_or_at_date(initial_date));
              s->set_date(0);
              group->add_action
                ( new action_add_snapshot( act.get_name(), s));
            }

          m_model_frame->do_action(group);
          m_model_frame->set_date(new_date);
          fill();
        }
    }
} // snapshot_frame::on_date_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_change( wxCommandEvent& WXUNUSED(event) )
{
  m_model_frame->set_mark_selection
    (wx_to_std_string(m_mark_choice->GetStringSelection()));
} // snapshot_frame::on_mark_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected action has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_action_change( wxCommandEvent& WXUNUSED(event) )
{
  m_model_frame->set_active_action
    (wx_to_std_string(m_action_choice->GetStringSelection()));
} // snapshot_frame::on_action_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The alignment has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_alignment_choice_change
( wxCommandEvent& WXUNUSED(event) )
{
  alignment_change();
} // snapshot_frame::on_alignment_choice_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The x of the mark placement has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_x( spin_event<double>& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_left_spin->GetValue(),
          &make_action_modify_placement_x ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_x
        ( m_snapshot, m, m_mark_left_spin->GetValue() ) );
} // snapshot_frame::on_mark_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief The y of the mark placement has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_y( spin_event<double>& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_bottom_spin->GetValue(),
          &make_action_modify_placement_y ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_y
        ( m_snapshot, m, m_mark_bottom_spin->GetValue() ) );
} // snapshot_frame::on_mark_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief The angle of the mark placement has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_angle( spin_event<double>& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_angle_spin->GetValue(),
          &make_action_modify_placement_angle ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_angle
        ( m_snapshot, m, m_mark_angle_spin->GetValue() ) );
} // snapshot_frame::on_mark_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief The width of the mark placement has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_width( spin_event<double>& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_width_spin->GetValue(),
          &make_action_modify_placement_width ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_width
        ( m_snapshot, m, m_mark_width_spin->GetValue() ) );
} // snapshot_frame::on_mark_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief The height of the mark placement has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_height( spin_event<double>& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_height_spin->GetValue(),
          &make_action_modify_placement_height ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_height
        ( m_snapshot, m, m_mark_height_spin->GetValue() ) );
} // snapshot_frame::on_mark_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user asked to change the collision function of a mark.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_mark_function( wxCommandEvent& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );
  wxTextEntryDialog dlg
    (this, _("Function:"), _("Set mark's collision function"),
     std_to_wx_string(m_snapshot->get_placement(m).get_collision_function()) );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_model_frame->do_action
        ( new action_set_placement_function
          (m_snapshot, m, wx_to_std_string(dlg.GetValue())) );

      m_collision_function_button->SetLabel( dlg.GetValue() );
    }
} // snapshot_frame::on_mark_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief The alignment has been changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_alignment_change
( spin_event<double>& WXUNUSED(event) )
{
  alignment_change();
} // snapshot_frame::on_alignment_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The size has been changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_size_change( spin_event<double>& WXUNUSED(event) )
{
  size_change();
} // snapshot_frame::on_size_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The depth control has been changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_depth_change( wxSpinEvent& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_depth_spin->GetValue(),
          &make_action_modify_placement_depth ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_depth
        ( m_snapshot, m, m_mark_depth_spin->GetValue() ) );
} // snapshot_frame::on_depth_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The visibility statut of the mark has been modified.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_visibility_change( wxCommandEvent& WXUNUSED(event) )
{
  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  if ( m_modify_whole_action->GetValue() )
    m_model_frame->do_action
      ( make_action_modify_placement_globally
        ( m_model_frame->get_model().get_active_action(), m,
          m_mark_visible_box->GetValue(),
          &make_action_modify_placement_visibility ) );
  else
    m_model_frame->do_action
      ( make_action_modify_placement_visibility
        ( m_snapshot, m, m_mark_visible_box->GetValue() ) );
} // snapshot_frame::on_visibility_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The function button is pressed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_function_change( wxCommandEvent& WXUNUSED(event) )
{
  wxTextEntryDialog dlg
    (this, _("Function:"), _("Set snapshot function"),
     std_to_wx_string(m_snapshot->get_function()) );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_model_frame->do_action
              ( new action_modify_function
          (m_snapshot, wx_to_std_string(dlg.GetValue())) );

      fill();
    }
} // snapshot_frame::on_function_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Applies the new sound_description to the snapshot.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_sound_change
( sound_description_event& WXUNUSED(event) )
{
  sound_change();
} // snapshot_frame::on_sound_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The easing for the x-coordinate has changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_x_easing( wxCommandEvent& event )
{
  bear::easing e;
  e.from_string
    ( wx_to_std_string(m_mark_x_easing->GetPopupControl()->GetStringValue()) );

  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  m_model_frame->do_action( new action_set_x_easing( m_snapshot, m, e ) );
} // snapshot_frame::on_x_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief The easing for the y-coordinate has changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_y_easing( wxCommandEvent& WXUNUSED(event) )
{
  bear::easing e;
  e.from_string
    ( wx_to_std_string(m_mark_y_easing->GetPopupControl()->GetStringValue()) );

  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  m_model_frame->do_action( new action_set_y_easing( m_snapshot, m, e ) );
} // snapshot_frame::on_y_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief The easing for the width has changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_width_easing( wxCommandEvent& WXUNUSED(event) )
{
  bear::easing e;
  e.from_string
    ( wx_to_std_string
      (m_mark_width_easing->GetPopupControl()->GetStringValue()) );

  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  m_model_frame->do_action( new action_set_width_easing( m_snapshot, m, e ) );
} // snapshot_frame::on_width_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief The easing for the height has changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_height_easing( wxCommandEvent& WXUNUSED(event) )
{
  bear::easing e;
  e.from_string
    ( wx_to_std_string
      (m_mark_height_easing->GetPopupControl()->GetStringValue()) );

  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  m_model_frame->do_action( new action_set_height_easing( m_snapshot, m, e ) );
} // snapshot_frame::on_height_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief The easing for the angle has changed.
 * \param event The event that occured.
 */
void bf::snapshot_frame::on_angle_easing( wxCommandEvent& WXUNUSED(event) )
{
  bear::easing e;
  e.from_string
    ( wx_to_std_string
      (m_mark_angle_easing->GetPopupControl()->GetStringValue()) );

  mark* m( m_model_frame->get_model().get_main_mark_selection() );

  m_model_frame->do_action( new action_set_angle_easing( m_snapshot, m, e ) );
} // snapshot_frame::on_angle_easing()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::snapshot_frame, wxPanel)
  EVT_CLOSE( bf::snapshot_frame::on_close )
  EVT_BUTTON( bf::snapshot_frame::IDC_DATE_CTRL,
              bf::snapshot_frame::on_date_change )
  EVT_CHOICE(bf::snapshot_frame::IDC_MARK_CHOICE,
             bf::snapshot_frame::on_mark_change)
  EVT_CHOICE(bf::snapshot_frame::IDC_ACTION_CHOICE,
             bf::snapshot_frame::on_action_change)
  EVT_CHOICE(bf::snapshot_frame::IDC_ALIGNMENT,
             bf::snapshot_frame::on_alignment_choice_change)

  EVT_SPIN_VALUE_CHANGE(bf::snapshot_frame::IDC_MARK_X,
                        bf::snapshot_frame::on_mark_x,
                        double)
  EVT_SPIN_VALUE_CHANGE(bf::snapshot_frame::IDC_MARK_Y,
                        bf::snapshot_frame::on_mark_y,
                        double)
  EVT_SPIN_VALUE_CHANGE(bf::snapshot_frame::IDC_MARK_ANGLE,
                        bf::snapshot_frame::on_mark_angle,
                        double)
  EVT_SPIN_VALUE_CHANGE( bf::snapshot_frame::IDC_MARK_WIDTH,
                         bf::snapshot_frame::on_mark_width, double )
  EVT_SPIN_VALUE_CHANGE( bf::snapshot_frame::IDC_MARK_HEIGHT,
                         bf::snapshot_frame::on_mark_height, double )

  EVT_BUTTON( bf::snapshot_frame::IDC_MARK_FUNCTION,
              bf::snapshot_frame::on_mark_function )
  EVT_SPIN_VALUE_CHANGE(bf::snapshot_frame::IDC_ALIGNMENT,
                        bf::snapshot_frame::on_alignment_change,
                        double)
  EVT_SPIN_VALUE_CHANGE(bf::snapshot_frame::IDC_SIZE,
                        bf::snapshot_frame::on_size_change,
                        double)

  EVT_SPINCTRL( bf::snapshot_frame::IDC_MARK_DEPTH,
                bf::snapshot_frame::on_depth_change )
  EVT_CHECKBOX(bf::snapshot_frame::IDC_VISIBILITY,
               bf::snapshot_frame::on_visibility_change )

  EVT_BUTTON( bf::snapshot_frame::IDC_FUNCTION,
              bf::snapshot_frame::on_function_change )
  EVT_SOUND_DESCRIPTION_CHANGE( bf::snapshot_frame::IDC_SOUND,
                                bf::snapshot_frame::on_sound_change )
END_EVENT_TABLE()
