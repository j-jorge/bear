/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A slider with tick support.
 * \author Sébastien Angibaud
 */
#ifndef __BF_SLIDER_WITH_TICKS_HPP__
#define __BF_SLIDER_WITH_TICKS_HPP__

#include <wx/panel.h>

#include "bf/spin_ctrl.hpp"
#include "bf/slider_ctrl.hpp"

#include <set>

namespace bf
{
  /**
   * \brief A slider with tick support.
   * \author Sébastien Angibaud
   */
  class slider_with_ticks:
    public wxPanel
  {
  public:
    typedef wxPanel super;

    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_PREVIOUS,
        ID_NEXT,
        ID_DATE_SPIN,
        ID_SLIDER
      }; // enum control_id

  public:
    slider_with_ticks
    (wxWindow* parent, wxWindowID id, slider_ctrl::tick value=0,
     slider_ctrl::tick min_value=0, slider_ctrl::tick max_value=100);

    void add_tick(double pos);
    void remove_tick(double pos);
    void clear_ticks();

    double get_value() const;
    void set_value( double value );
    void set_max_value( double max );

    void previous_tick();
    void next_tick();

  private:
    void create_controls();
    void set_controls_position();
    void fill_controls();
    void send_event_change_value();
    void send_event_tick_move( tick_event& event );

    void on_previous( wxCommandEvent& event );
    void on_next( wxCommandEvent& event );
    void on_date_change(spin_event<double>& event);
    void on_slider_change(slider_event& event);
    void on_tick_move(tick_event& event);

  private:
    /** \brief The current value. */
    double m_value;

    /** \brief The minimum value. */
    double m_min_value;

    /** \brief The maximum value. */
    double m_max_value;

    /** \brief The list of ticks. */
    slider_ctrl::ticks m_ticks;

    /** \brief The button to go at the previous tick. */
    wxBitmapButton* m_previous;

    /** \brief The button to go at the next tick. */
    wxBitmapButton* m_next;

    /** \brief The spin to edit the value. */
    spin_ctrl<double>* m_spin;

    /** \brief The slider control. */
    slider_ctrl* m_slider;

    DECLARE_EVENT_TABLE()

  }; // class slider_with_ticks
} // namespace bf

#endif // __BF_SLIDER_WITH_TICKS_HPP__
