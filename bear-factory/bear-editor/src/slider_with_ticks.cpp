/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::slider_with_ticks class.
 * \author Sébastien Angibaud
 */
#include "bf/slider_with_ticks.hpp"

#include "bf/icons.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param id The identifier of the window.
 * \param value The initial value of the slider.
 * \param min_value The minimum allowed value.
 * \param max_value The maximum allowed value.
 */
bf::slider_with_ticks::slider_with_ticks
( wxWindow* parent, wxWindowID id, double value, double min_value,
  double max_value )
  : super( parent, id ),
    m_value(value), m_min_value(min_value), m_max_value(max_value),
    m_previous(NULL), m_next(NULL), m_spin(NULL), m_slider(NULL)
{
  create_controls();
} // slider_with_ticks::slider_with_ticks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a tick.
 * \param pos The position of the tick.
 */
void bf::slider_with_ticks::add_tick(double pos)
{
  if ( ( pos >= 0 ) && ( pos <= m_max_value ) )
    if ( m_ticks.find(pos) ==  m_ticks.end() )
      {
        m_ticks.insert(pos);
        m_slider->render();
      }
} // slider_with_ticks::add_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a tick.
 * \param pos The pos of the tick to remove.
 */
void bf::slider_with_ticks::remove_tick(double pos)
{
  if ( m_ticks.find(pos) !=  m_ticks.end() )
    {
      m_ticks.erase(pos);

      if ( m_value == pos )
        {
          slider_ctrl::ticks::const_iterator it;
          bool ok = false;
          bool stop = false;
          double new_value = 0;

          for (it = m_ticks.begin(); (it != m_ticks.end() ) && !stop; ++it )
            {
              if ( *it < m_value )
                {
                  ok = true;
                  new_value = *it;
                }
              else
                stop = true;
            };
          if ( ok )
            set_value(new_value);
        }

      m_slider->render();
    }
} // slider_with_ticks::remove_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all ticks.
 */
void bf::slider_with_ticks::clear_ticks()
{
  m_ticks.clear();
  m_slider->render();
} // slider_with_ticks::clear_ticks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value.
 */
double bf::slider_with_ticks::get_value( ) const
{
  return m_value;
} // slider_with_ticks::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value.
 * \param value The new value;
 */
void bf::slider_with_ticks::set_value( double value )
{
  m_value = (double)((int)((value+0.0005) * 1000)) / 1000;

  m_slider->set_value(m_value);
  m_spin->SetValue(m_value);

  send_event_change_value();
} // slider_with_ticks::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum value.
 * \param max The new maximum value;
 */
void bf::slider_with_ticks::set_max_value( double max )
{
  m_max_value = max;

  if ( m_value > m_max_value )
    set_value(m_max_value);

  m_slider->set_max_value(m_max_value);
  m_spin->SetRange(m_min_value, m_max_value);
} // slider_with_ticks::set_max_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the previous tick.
 */
void bf::slider_with_ticks::previous_tick()
{
  slider_ctrl::ticks::const_iterator it;
  bool ok = false;
  bool stop = false;
  double new_value = 0;

  for (it = m_ticks.begin(); (it != m_ticks.end() ) && !stop; ++it )
    {
      if ( *it < m_value )
        {
          ok = true;
          new_value = *it;
        }
      else
        stop = true;
    }

  if ( ok )
    set_value(new_value);
} // slider_with_ticks::previous_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next tick.
 */
void bf::slider_with_ticks::next_tick()
{
  slider_ctrl::ticks::const_iterator it;
  bool ok = false;
  double new_value = 0;

  for (it = m_ticks.begin(); (it != m_ticks.end() ) && !ok; ++it )
    if ( *it > m_value )
      {
        ok = true;
        new_value = *it;
      }

  if ( ok )
    set_value(new_value);
} // slider_with_ticks::next_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sub controls.
 */
void bf::slider_with_ticks::create_controls()
{
  m_next =
    new wxBitmapButton(this, ID_NEXT, wxBitmap(player_forward_xpm));
  m_previous =
    new wxBitmapButton(this, ID_PREVIOUS, wxBitmap(player_rewind_xpm));

  m_spin =
    new spin_ctrl<double>(this, ID_DATE_SPIN, wxDefaultPosition,
                          wxDefaultSize, 0, m_min_value,
                          m_max_value, m_value, 0.1);

  m_slider = new slider_ctrl
    (this, ID_SLIDER, m_value, m_min_value, m_max_value );
  m_slider->set_ticks(&m_ticks);

  set_controls_position();
} // slider_with_ticks::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set control position.
 */
void bf::slider_with_ticks::set_controls_position()
{
  wxBoxSizer* s = new wxBoxSizer(wxHORIZONTAL);

  s->Add( m_previous );
  s->Add( m_next );
  s->Add( m_slider, 1, wxEXPAND );
  s->Add( m_spin );

  SetSizer(s);
} // slider_with_ticks::set_controls_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill controls.
 */
void bf::slider_with_ticks::fill_controls()
{
  m_spin->SetValue(m_value);
  m_slider->set_value(m_value);

  Refresh();
} // slider_with_ticks::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event "value_change".
 */
void bf::slider_with_ticks::send_event_change_value()
{
  slider_event event
    ( m_value, slider_event::value_change_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // slider_with_ticks::send_event_change_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event "tick_move".
 * \param event
 */
void bf::slider_with_ticks::send_event_tick_move( tick_event& event )
{
  tick_event ev
    ( event.get_initial_value(), event.get_new_value(), event.get_copy(),
      tick_event::move_event_type, GetId() );
  ev.SetEventObject(this);
  ProcessEvent(ev);

  if ( !ev.IsAllowed() )
    event.Veto();
} // slider_with_ticks::send_event_tick_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the previous tick.
 * \param event This event occured.
 */
void bf::slider_with_ticks::on_previous( wxCommandEvent& WXUNUSED(event) )
{
  previous_tick();
} // slider_with_ticks::on_previous()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next tick.
 * \param event This event occured.
 */
void bf::slider_with_ticks::on_next( wxCommandEvent& WXUNUSED(event) )
{
  next_tick();
} // slider_with_ticks::on_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief The current date changed.
 * \param event This event occured.
 */
void bf::slider_with_ticks::on_date_change(spin_event<double>& event)
{
  if ( event.get_value() != m_value )
    set_value(event.get_value());
} // slider_with_ticks::on_date_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The date of the slider changed.
 * \param event This event occured.
 */
void bf::slider_with_ticks::on_slider_change(slider_event& event)
{
  if ( event.get_value() != m_value )
    set_value(event.get_value());
} // slider_with_ticks::on_slider_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The date of the slider changed.
 * \param event This event occured.
 */
void bf::slider_with_ticks::on_tick_move(tick_event& event)
{
  send_event_tick_move(event);
} // slider_with_ticks::on_tick_move()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::slider_with_ticks, wxPanel)
  EVT_BUTTON( bf::slider_with_ticks::ID_PREVIOUS,
              bf::slider_with_ticks::on_previous )
  EVT_BUTTON( bf::slider_with_ticks::ID_NEXT,
              bf::slider_with_ticks::on_next )
  EVT_SPIN_VALUE_CHANGE( bf::slider_with_ticks::ID_DATE_SPIN,
                         bf::slider_with_ticks::on_date_change,
                         double )
  EVT_SLIDER_VALUE_CHANGE( bf::slider_with_ticks::ID_SLIDER,
                           bf::slider_with_ticks::on_slider_change )
  EVT_TICK_MOVE( bf::slider_with_ticks::ID_SLIDER,
                 bf::slider_with_ticks::on_tick_move )
END_EVENT_TABLE()
