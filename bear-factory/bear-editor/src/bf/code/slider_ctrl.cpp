/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::slider_ctrl class.
 * \author Sébastien Angibaud
 */
#include "bf/slider_ctrl.hpp"

#include <wx/dcbuffer.h>
#include <wx/settings.h>

const unsigned int bf::slider_ctrl::s_slider_border = 8;

const wxEventType bf::slider_event::value_change_event_type =
  wxNewEventType();

const wxEventType bf::tick_event::move_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::slider_event::slider_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // slider_event::slider_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param value The value of the slider.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::slider_event::slider_event
( double value, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_value(value)
{

} // slider_event::slider_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The event to copy from.
 */
bf::slider_event::slider_event( const slider_event& that )
  : wxNotifyEvent(that), m_value(that.m_value)
{

} // slider_event::slider_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::slider_event::Clone() const
{
  return new slider_event(*this);
} // slider_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of the slider.
 */
double bf::slider_event::get_value() const
{
  return m_value;
} // slider_event::get_value()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::tick_event::tick_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // tick_event::tick_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param initial_value The initial value of the tick.
 * \param new_value The new value of the tick.
 * \param copy Tell if the user asks to copy the data associated with the
 *        previous position (?).
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::tick_event::tick_event
( double initial_value, double new_value, bool copy,
  wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id),
    m_initial_value(initial_value), m_new_value(new_value), m_copy(copy)
{

} // tick_event::tick_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The event to copy from.
 */
bf::tick_event::tick_event( const tick_event& that )
  : wxNotifyEvent(that), m_initial_value(that.m_initial_value),
    m_new_value(that.m_new_value), m_copy(that.m_copy)
{
} // tick_event::tick_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::tick_event::Clone() const
{
  return new tick_event(*this);
} // tick_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the initial value of the tick.
 */
double bf::tick_event::get_initial_value() const
{
  return m_initial_value;
} // tick_event::get_initial_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new value of the tick.
 */
double bf::tick_event::get_new_value() const
{
  return m_new_value;
} // tick_event::get_new_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the copy statut.
 */
bool bf::tick_event::get_copy() const
{
  return m_copy;
} // tick_event::get_copy()



/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::slider_ctrl::drag_info::drag_info()
  : drag_mode(drag_mode_none)
{

} // slider_ctrl::drag_info::drag_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the difference between the initial mouse position and the current
 *        mouse position.
 */
wxSize bf::slider_ctrl::drag_info::delta() const
{
  return wxSize
    ( mouse_position.x - mouse_origin.x, mouse_position.y - mouse_origin.y );
} // slider_ctrl::drag_info::delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window owning this one.
 * \param id The identifier of the control.
 * \param value The value of the slider.
 * \param min_value The mininum value of the slider.
 * \param max_value The maximum value of the slider.
 */
bf::slider_ctrl::slider_ctrl
( wxWindow* parent, wxWindowID id, double value, double min_value,
  double max_value )
  : super( parent, id ), m_drag_info(NULL),
    m_value(value), m_min_value(min_value), m_max_value(max_value),
    m_ticks(NULL)
{

} // slider_ctrl::slider_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the control.
 */
void bf::slider_ctrl::render()
{
  wxBufferedPaintDC dc( this );

  if( IsShown() )
    {
      wxFont font(dc.GetFont());
      font.SetPointSize(8);
      dc.SetFont(font);
      dc.SetBackgroundMode(wxSOLID);
      dc.SetBackground(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
      dc.Clear();
      render_slider(dc);
      render_ticks(dc);
      render_value(dc);
      render_drag_info(dc);
    }
} // slider_ctrl::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add aSet theticks.
 * \param t The ticks.
 */
void bf::slider_ctrl::set_ticks(const ticks* t)
{
  m_ticks = t;
} // slider_ctrl::add_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value.
 */
double bf::slider_ctrl::get_value() const
{
  return m_value;;
} // slider_ctrl::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value.
 * \param value The new value.
 */
void bf::slider_ctrl::set_value( double value )
{
  if ( m_value != value)
    {
      m_value = value;
      render();
    }
} // slider_ctrl::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum value.
 * \param max The new maximum value.
 */
void bf::slider_ctrl::set_max_value( double max )
{
  m_max_value = max;

  if ( m_value > m_max_value )
    m_value = m_max_value;

  render();
} // slider_ctrl::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there exist a tick at a given position.
 * \param value The considered value.
 */
bool bf::slider_ctrl::has_tick( double value ) const
{
  bool result = false;

  ticks::const_iterator it;
  for ( it = m_ticks->begin(); ( it != m_ticks->end() ) && !result; ++it)
    if ( *it == value )
      result = true;

  return result;
} // slider_ctrl::has_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event "value_change".
 */
void bf::slider_ctrl::send_event_change_value()
{
  slider_event event
    ( m_value, slider_event::value_change_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // slider_ctrl::send_event_change_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event "tick_move".
 * \param initial_value The initial value of the tick
 * \param new_value The new value of the tick
 * \param copy Indicates if the move is a copy.
 */
void bf::slider_ctrl::send_event_move_tick
(double initial_value, double new_value, bool copy)
{
  tick_event event
    ( initial_value, new_value, copy, tick_event::move_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);

  if ( !event.IsAllowed() )
    {
      set_value(m_drag_info->initial_value);
      send_event_change_value();
    }
} // slider_ctrl::send_event_move_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the slider.
 */
unsigned int bf::slider_ctrl::get_slider_width() const
{
  return GetSize().x - 2 * s_slider_border;
} // slider_ctrl::get_slider_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the slider on the screen.
 * \param dc The device context for the drawings.
 */
void bf::slider_ctrl::render_slider( wxDC& dc ) const
{
  dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW));
  dc.DrawLine( s_slider_border, GetSize().y / 2 - 3,
               GetSize().x - s_slider_border, GetSize().y / 2 - 3 );

  dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT));
  dc.DrawLine( s_slider_border, GetSize().y / 2 - 2,
               GetSize().x - s_slider_border, GetSize().y / 2 - 2 );

  dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  dc.DrawRoundedRectangle( 1, 1, GetSize().x - 2, GetSize().y - 2, 3 );
} // slider_ctrl::render_slider()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the value on the screen.
 * \param dc The device context for the drawings.
 */
void bf::slider_ctrl::render_value( wxDC& dc ) const
{
  wxPoint pos(get_position(m_value), GetSize().y / 2);

  dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW));
  dc.SetBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
  wxPoint p3[5];
  p3[0] = wxPoint( pos.x, pos.y + 4 );
  p3[1] = wxPoint( pos.x + 4, pos.y );
  p3[2] = wxPoint( pos.x + 4, pos.y - 9 );
  p3[3] = wxPoint( pos.x - 4, pos.y - 9 );
  p3[4] = wxPoint( pos.x - 4, pos.y );
  dc.DrawPolygon(5, p3);

  dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT));
  wxPoint p[4];
  p[0] = wxPoint( pos.x, pos.y + 3 );
  p[1] = wxPoint( pos.x - 3, pos.y );
  p[2] = wxPoint( pos.x - 3, pos.y - 8 );
  p[3] = wxPoint( pos.x + 3, pos.y - 8 );
  dc.DrawLines(4, p);
} // slider_ctrl::render_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the drag info on the screen.
 * \param dc The device context for the drawings.
 */
void bf::slider_ctrl::render_drag_info( wxDC& dc ) const
{
  if ( m_drag_info != NULL )
    if ( m_drag_info->drag_mode == drag_info::drag_mode_tick_move )
      {
        double v = get_value(m_drag_info->mouse_position.x);

        if ( has_tick(v) )
          {
            render_tick( dc, get_position(m_drag_info->initial_value),
                         *wxRED, *wxTRANSPARENT_BRUSH);
            render_tick( dc, get_position(v), *wxRED, *wxRED_BRUSH);
          }
        else
          {
            render_tick( dc, get_position(m_drag_info->initial_value),
                         *wxGREEN, *wxTRANSPARENT_BRUSH);
            render_tick( dc, get_position(v), *wxGREEN, *wxGREEN_BRUSH);
          }
      }
} // slider_ctrl::render_drag_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render sticks on the screen.
 * \param dc The device context for the drawings.
 */
void bf::slider_ctrl::render_ticks( wxDC& dc ) const
{
  if ( m_ticks )
    {
      ticks::const_iterator it;

      for (it = m_ticks->begin(); it != m_ticks->end(); ++it )
        render_tick(dc, *it);
    }
} // slider_ctrl::render_ticks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a stick on the screen.
 * \param dc The device context for the drawings.
 * \param t The tick to render.
 */
void bf::slider_ctrl::render_tick( wxDC& dc, tick t ) const
{
  if ( t == m_value )
    {
      wxBrush brush(*wxBLACK);
      render_tick( dc, get_position(t), *wxBLACK, brush);
    }
  else
    render_tick( dc, get_position(t), *wxBLACK, *wxTRANSPARENT_BRUSH);
} // slider_ctrl::render_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a stick on the screen.
 * \param dc The device context for the drawings.
 * \param position The tick position tick.
 * \param colour The colour of rendering.
 * \param brush The brush.
 */
void bf::slider_ctrl::render_tick
( wxDC& dc, unsigned int position, const wxColour& colour,
  const wxBrush& brush ) const
{
  wxPen pen(colour);
  dc.SetPen(pen);
  dc.SetBrush(brush);

  wxPoint p[3];
  wxPoint pos(position, GetSize().y / 2);

  p[0] = wxPoint( pos.x, pos.y+5 );
  p[1] = wxPoint( pos.x+4, pos.y +9 );
  p[2] = wxPoint( pos.x-4, pos.y +9 );

  dc.DrawPolygon(3, p);
} // slider_ctrl::render_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value according a position.
 * \param pos The position.
 */
double bf::slider_ctrl::get_value( int pos ) const
{
  double result;

  if ( pos < (int)(s_slider_border))
    result = 0;
  else if ( pos > (int)(s_slider_border + get_slider_width()) )
    result = m_max_value;
  else
    {
      pos -= s_slider_border;

      result =
        ( (double)pos * ( m_max_value - m_min_value ) ) /
        (double)get_slider_width() + m_min_value;

      if ( result < m_min_value )
        result = m_min_value;

      if ( result > m_max_value )
        result = m_max_value;
    }

  result = (double)((int)((result+0.0005) * 1000)) / 1000;

  return result;
} // slider_ctrl::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position according a value.
 * \param value The value.
 */
unsigned int bf::slider_ctrl::get_position( double value ) const
{
  return  s_slider_border + (unsigned int)
    ( ( ( value - m_min_value ) * (double)(get_slider_width() ) ) /
      ( m_max_value - m_min_value ) );
} // slider_ctrl::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the nearest tick of a given value.
 * \param v The considered value.
 */
double bf::slider_ctrl::nearest_tick( double v ) const
{
  double result = v;
  double min_dist = m_max_value + 1;

  if ( m_ticks )
    {
      slider_ctrl::ticks::const_iterator it;

      for (it = m_ticks->begin(); it != m_ticks->end(); ++it )
        {
          double dist;
          if ( *it > v )
            dist = *it - v;
          else
            dist = v - *it;

          if ( dist < min_dist )
            {
              result = *it;
              min_dist = dist;
            }
        }
    }

  return result;
} // slider_ctrl::nearest_tick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in move mode.
 * \param shift Shift key is pressed.
 */
void bf::slider_ctrl::apply_drag_mode_move( bool shift )
{
  double v = get_value(m_drag_info->mouse_position.x);

  if ( shift )
    v = nearest_tick(v);

  if ( v != m_value )
    {
      set_value(v);
      send_event_change_value();
    }
} // slider_ctrl::apply_drag_mode_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in tick move mode.
 * \param control Indicates if control key is pressed.
 * \param alt Indicates if alt key is pressed.
 */
void bf::slider_ctrl::apply_drag_mode_tick_move( bool control, bool alt )
{
  if ( control )
    {
      double v = get_value(m_drag_info->mouse_position.x);
      send_event_move_tick(m_drag_info->initial_value, v, alt);
    }
  else
    {
      set_value(m_drag_info->initial_value);
      send_event_change_value();
    }
} // slider_ctrl::apply_drag_mode_tick_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::slider_ctrl::on_size( wxSizeEvent& WXUNUSED(event) )
{
  Refresh();
} // slider_ctrl::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::slider_ctrl::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  render();
} // slider_ctrl::on_paint()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user start a click in the frame.
 * \param event The mouse event that occured.
 */
void bf::slider_ctrl::on_mouse_left_down( wxMouseEvent& event )
{
  wxPoint point( event.GetX(), event.GetY() );

  if ( event.ControlDown() )
    {
      double v = get_value(point.x);
      v = nearest_tick(v);

      if ( has_tick(v) )
        {
          m_drag_info = new drag_info();
          m_drag_info->drag_mode = drag_info::drag_mode_tick_move;
          m_drag_info->initial_value = v;
          m_drag_info->mouse_origin = point;
          m_drag_info->mouse_position = m_drag_info->mouse_origin;
        }
    }
  else
    {
      m_drag_info = new drag_info();
      m_drag_info->drag_mode = drag_info::drag_mode_move;
      m_drag_info->mouse_origin = point;
      m_drag_info->mouse_position = m_drag_info->mouse_origin;
    }
} // slider_ctrl::on_mouse_left_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked in the frame.
 * \param event The mouse event that occured.
 */
void bf::slider_ctrl::on_mouse_move( wxMouseEvent& event )
{
  const wxPoint point( event.GetX(), event.GetY() );

  if ( event.LeftIsDown() )
    if ( m_drag_info != NULL )
      {
        m_drag_info->mouse_position = point;
        double v = get_value(point.x);

        if ( m_drag_info->drag_mode == drag_info::drag_mode_move )
          {
            if ( event.ShiftDown() )
              v = nearest_tick(v);
          }

        if ( v != m_value )
          {
            set_value(v);
            send_event_change_value();
          }
      }
} // slider_ctrl::on_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user click in the frame.
 * \param event The mouse event that occured.
 */
void bf::slider_ctrl::on_mouse_left_up( wxMouseEvent& event )
{
  const wxPoint point( event.GetX(), event.GetY() );

  if ( m_drag_info == NULL )
    event.Skip();
  else
    {
      m_drag_info->mouse_position = point;
      if ( m_drag_info->drag_mode == drag_info::drag_mode_tick_move )
        {
          apply_drag_mode_tick_move
            ( event.ControlDown(), event.AltDown() );
          delete m_drag_info;
          m_drag_info = NULL;
          render();
        }
      else if ( m_drag_info->drag_mode == drag_info::drag_mode_move )
        apply_drag_mode_move( event.ShiftDown() );

      if ( m_drag_info != NULL )
        {
          m_drag_info = NULL;
          render();
        }
    }
} // slider_ctrl::on_mouse_left_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user double click in the frame.
 * \param event The mouse event that occured.
 */
void bf::slider_ctrl::on_mouse_left_dclick( wxMouseEvent& event )
{
  double v = get_value(event.GetX());

  v = nearest_tick(v);

  if ( v != m_value )
    {
      set_value(v);
      send_event_change_value();
    }
} // slider_ctrl::on_mouse_left_dclick()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::slider_ctrl, wxPanel)
  EVT_SIZE( bf::slider_ctrl::on_size )
  EVT_PAINT( bf::slider_ctrl::on_paint )
  EVT_LEFT_UP( bf::slider_ctrl::on_mouse_left_up )
  EVT_LEFT_DOWN( bf::slider_ctrl::on_mouse_left_down )
  EVT_MOTION( bf::slider_ctrl::on_mouse_move )
  EVT_LEFT_DCLICK( bf::slider_ctrl::on_mouse_left_dclick )
END_EVENT_TABLE()
