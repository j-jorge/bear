/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays a animation and some buttons to play it.
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_VIEW_CTRL_HPP__
#define __BF_ANIMATION_VIEW_CTRL_HPP__

#include "bf/animation_player.hpp"
#include "bf/spin_ctrl.hpp"

#include "bf/libeditor_export.hpp"

namespace bf
{
  class sprite_view_ctrl;

  /**
   * \brief This control displays a animation and some buttons to play it.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation_view_ctrl:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_PLAYER_START,
        ID_PLAYER_REWIND,
        ID_PLAYER_PLAY,
        ID_PLAYER_STOP,
        ID_PLAYER_FORWARD,
        ID_PLAYER_END,
        ID_SLIDER,
        ID_TIMER
      }; // enum control_id

  public:
    animation_view_ctrl
    ( wxWindow& parent, const animation& anim = animation() );

    void set_animation( const animation& anim );

  private:
    void create_controls();
    void create_sizers();

    void display_current_sprite();
    void set_timer_duration();

    void next();

    void on_player_start( wxCommandEvent& event );
    void on_player_rewind( wxCommandEvent& event );
    void on_player_play( wxCommandEvent& event );
    void on_player_stop( wxCommandEvent& event );
    void on_player_forward( wxCommandEvent& event );
    void on_player_end( wxCommandEvent& event );
    void on_scroll( wxScrollEvent& event );
    void on_timer( wxTimerEvent& event );

  private:
    /** \brief The control that displays the current sprite of the animation. */
    sprite_view_ctrl* m_sprite_view;

    /** \brief The displayed animation. */
    animation m_animation;

    /** \brief The player of the animation. */
    animation_player m_player;

    /** \brief Slider displaying the position in the animation. */
    wxSlider* m_slider;

    /** \brief The timer informing that we must change the frame. */
    wxTimer m_timer;

    /** \brief A factor applied to the duration of the frames while playing. */
    spin_ctrl<double>* m_factor;

    DECLARE_EVENT_TABLE()

  }; // class animation_view_ctrl
} // namespace bf

#endif // __BF_ANIMATION_VIEW_CTRL_HPP__
