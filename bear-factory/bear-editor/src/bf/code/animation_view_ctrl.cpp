/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_view_ctrl class.
 * \author Julien Jorge
 */
#include "bf/animation_view_ctrl.hpp"

#include "bf/sprite_view_ctrl.hpp"

#include "bf/icon/player_start.xpm"
#include "bf/icon/player_rewind.xpm"
#include "bf/icon/player_play.xpm"
#include "bf/icon/player_stop.xpm"
#include "bf/icon/player_forward.xpm"
#include "bf/icon/player_end.xpm"

#include <claw/assert.hpp>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param anim The animation displayed.
 */
bf::animation_view_ctrl::animation_view_ctrl
( wxWindow& parent, const animation& anim )
  : wxPanel(&parent, wxID_ANY), m_animation(anim), m_player(m_animation),
    m_timer(this, ID_TIMER)
{
  create_controls();
  create_sizers();

  Fit();

  set_animation(anim);
} // animation_view_ctrl::animation_view_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation.
 * \param anim The animation displayed.
 */
void bf::animation_view_ctrl::set_animation( const animation& anim )
{
  m_animation = anim;
  m_player.set_animation(m_animation);

  m_player.set_current_index(0);
  display_current_sprite();

  if ( m_animation.frames_count() <= 1 )
    m_slider->Disable();
  else
    {
      m_slider->Enable();
      m_slider->SetRange(0, m_animation.frames_count() - 1);
    }

  m_slider->SetValue(0);
} // animation_view_ctrl::set_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sub controls.
 */
void bf::animation_view_ctrl::create_controls()
{
  m_sprite_view = new sprite_view_ctrl(*this);
  m_slider = new wxSlider(this, ID_SLIDER, 0, 0, 1);
  m_factor = new spin_ctrl<double>(this, wxID_ANY);

  m_factor->SetRange(0, std::numeric_limits<double>::max());
  m_factor->SetValue(1);
  m_factor->SetStep(0.1);
} // animation_view_ctrl::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers and the non-member controls.
 */
void bf::animation_view_ctrl::create_sizers()
{
  wxBoxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );

  h_sizer->Add
    ( new wxBitmapButton(this, ID_PLAYER_START, wxBitmap(player_start_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_PLAYER_REWIND, wxBitmap(player_rewind_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_PLAYER_PLAY, wxBitmap(player_play_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_PLAYER_STOP, wxBitmap(player_stop_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton
      (this, ID_PLAYER_FORWARD, wxBitmap(player_forward_xpm)) );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_PLAYER_END, wxBitmap(player_end_xpm)) );

  h_sizer->Add( new wxStaticText(this, wxID_ANY, _("Time factor:")) );
  h_sizer->Add( m_factor, 1, wxEXPAND );

  v_sizer->Add( m_sprite_view, 1, wxEXPAND );
  v_sizer->Add( m_slider, 0, wxEXPAND );
  v_sizer->Add( h_sizer, 0, wxEXPAND );

  SetSizer(v_sizer);
} // animation_view_ctrl::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Display the current sprite of the animation.
 */
void bf::animation_view_ctrl::display_current_sprite()
{
  m_sprite_view->set_sprite( m_player.get_sprite() );
} // animation_view_ctrl::display_current_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the timer with the duration of the current frame.
 */
void bf::animation_view_ctrl::set_timer_duration()
{
  if ( !m_player.is_finished() )
    m_timer.Start
      ( (int)
        (m_factor->GetValue() * m_player.get_duration_until_next() * 1000) );
} // animation_view_ctrl::set_timer_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame.
 */
void bf::animation_view_ctrl::next()
{
  if ( m_player.is_finished() )
    m_timer.Stop();
  else
    {
      m_player.next();
      set_timer_duration();
      display_current_sprite();
      m_slider->SetValue(m_player.get_current_index());
   }
} // animation_view_ctrl::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the first frame of the animation.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_player_start( wxCommandEvent& WXUNUSED(event) )
{
  m_player.reset();
  display_current_sprite();
  m_slider->SetValue( m_player.get_current_index() );
} // animation_view_ctrl::on_player_start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the previous frame.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_player_rewind
( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_player.get_current_index() > 0 )
    {
      m_player.set_current_index( m_player.get_current_index() - 1 );
      display_current_sprite();
      m_slider->SetValue( m_player.get_current_index() );
    }
} // animation_view_ctrl::on_player_rewind()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the animation.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_player_play( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_player.is_finished() )
    m_player.reset();

  set_timer_duration();
} // animation_view_ctrl::on_player_play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop playing the animation.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_player_stop( wxCommandEvent& WXUNUSED(event) )
{
  m_timer.Stop();
} // animation_view_ctrl::on_player_stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_player_forward
( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_player.get_current_index() + 1 < m_animation.frames_count() )
    {
      m_player.set_current_index( m_player.get_current_index() + 1 );
      display_current_sprite();
      m_slider->SetValue( m_player.get_current_index() );
    }
} // animation_view_ctrl::on_player_forward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the last frame.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_player_end( wxCommandEvent& WXUNUSED(event) )
{
  if ( !m_animation.empty() )
    {
      m_player.set_current_index( m_animation.frames_count() - 1 );
      display_current_sprite();
      m_slider->SetValue( m_player.get_current_index() );
    }
} // animation_view_ctrl::on_player_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief The position in the slider has been changed.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_scroll( wxScrollEvent& WXUNUSED(event) )
{
  unsigned int i = m_slider->GetValue();

  if ( i >= m_animation.frames_count() )
    i = 0;

  m_player.set_current_index(i);
  display_current_sprite();
} // animation_view_ctrl::on_scroll()

/*----------------------------------------------------------------------------*/
/**
 * \brief Time to change the frame.
 * \param event This event occured.
 */
void bf::animation_view_ctrl::on_timer( wxTimerEvent& WXUNUSED(event) )
{
  next();
} // animation_view_ctrl::on_timer()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::animation_view_ctrl, wxPanel)
  EVT_BUTTON( bf::animation_view_ctrl::ID_PLAYER_START,
              bf::animation_view_ctrl::on_player_start )
  EVT_BUTTON( bf::animation_view_ctrl::ID_PLAYER_REWIND,
              bf::animation_view_ctrl::on_player_rewind )
  EVT_BUTTON( bf::animation_view_ctrl::ID_PLAYER_PLAY,
              bf::animation_view_ctrl::on_player_play )
  EVT_BUTTON( bf::animation_view_ctrl::ID_PLAYER_STOP,
              bf::animation_view_ctrl::on_player_stop )
  EVT_BUTTON( bf::animation_view_ctrl::ID_PLAYER_FORWARD,
              bf::animation_view_ctrl::on_player_forward )
  EVT_BUTTON( bf::animation_view_ctrl::ID_PLAYER_END,
              bf::animation_view_ctrl::on_player_end )
  EVT_COMMAND_SCROLL( bf::animation_view_ctrl::ID_SLIDER,
                      bf::animation_view_ctrl::on_scroll )
  EVT_TIMER( bf::animation_view_ctrl::ID_TIMER,
             bf::animation_view_ctrl::on_timer )
END_EVENT_TABLE()
