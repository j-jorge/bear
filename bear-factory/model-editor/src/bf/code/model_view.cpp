/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_view class.
 * \author Julien Jorge
 */
#include "bf/model_view.hpp"

#include "bf/action.hpp"
#include "bf/gui_model.hpp"
#include "bf/mark.hpp"
#include "bf/model_snapshot_tweener.hpp"
#include "bf/snapshot.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/dcbuffer.h>
#include <wx/settings.h>

#include <vector>
#include <algorithm>

namespace bf
{
  /**
   * \brief Compare two mark_placement by increasing order of their depthe
   *        position.
   * \author Julien Jorge
   */
  struct compare_mark_placement_by_depth_lt
  {
    bool operator()( const mark_placement& a, const mark_placement& b ) const
    {
      return a.get_depth_position() < b.get_depth_position();
    }
  }; // struct::compare_mark_placement_by_depth_lt
}



const wxEventType bf::mark_event::selection_change_event_type =
  wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::mark_event::mark_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // mark_event::mark_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param value The label of the mark to select.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::mark_event::mark_event
( const std::string& value, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_value(value)
{

} // mark_event::mark_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The event to copy from.
 */
bf::mark_event::mark_event( const mark_event& that )
  : wxNotifyEvent(that), m_value(that.m_value)
{

} // mark_event::mark_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::mark_event::Clone() const
{
  return new mark_event(*this);
} // mark_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the label of the mark.
 */
const std::string& bf::mark_event::get_value() const
{
  return m_value;
} // mark_event::get_value()



const wxEventType bf::placement_event::move_event_type = wxNewEventType();
const wxEventType bf::placement_event::size_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::placement_event::placement_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // placement_event::placement_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param label The label of the mark to move.
 * \param x The new x position.
 * \param y The new y position.
 * \param width The new width of the mark.
 * \param height The new height of the mark.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::placement_event::placement_event
( const std::string& label, double x, double y, double width,
  double height, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_label(label), m_x(x), m_y(y),
    m_width(width), m_height(height)
{

} // placement_event::placement_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::placement_event::Clone() const
{
  return new placement_event(*this);
} // placement_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the label of the mark to move.
 */
const std::string& bf::placement_event::get_label() const
{
  return m_label;
} // placement_event::get_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new x position of the mark to move.
 */
double bf::placement_event::get_x() const
{
  return m_x;
} // placement_event::get_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new y position of the mark to move.
 */
double bf::placement_event::get_y() const
{
  return m_y;
} // placement_event::get_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new width of the mark to move.
 */
double bf::placement_event::get_width() const
{
  return m_width;
} // placement_event::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new height of the mark to move.
 */
double bf::placement_event::get_height() const
{
  return m_height;
} // placement_event::get_height()



const wxEventType bf::bounding_box_event::change_event_type = wxNewEventType();
const wxEventType bf::bounding_box_event::select_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::bounding_box_event::bounding_box_event( wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id)
{

} // bounding_box_event::bounding_box_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param left The new left position.
 * \param bottom The new bottom position.
 * \param width The new width.
 * \param height The new height.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::bounding_box_event::bounding_box_event
( double left, double bottom, double width, double height,
  wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_left(left), m_bottom(bottom),
    m_width(width), m_height(height)
{

} // bounding_box_event::bounding_box_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::bounding_box_event::Clone() const
{
  return new bounding_box_event(*this);
} // bounding_box_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new left position of the bounding_box.
 */
double bf::bounding_box_event::get_left() const
{
  return m_left;
} // bounding_box_event::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new bottom position of the bounding box.
 */
double bf::bounding_box_event::get_bottom() const
{
  return m_bottom;
} // bounding_box_event::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new width of the bounding_box.
 */
double bf::bounding_box_event::get_width() const
{
  return m_width;
} // bounding_box_event::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new height of the bounding box.
 */
double bf::bounding_box_event::get_height() const
{
  return m_height;
} // bounding_box_event::get_height()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::model_view::drag_info::drag_info()
  : drag_mode(drag_mode_none), picked_mark(NULL), x_active(true), y_active(true)
{

} // model_view::drag_info::drag_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the info to its initial state.
 */
void bf::model_view::drag_info::clear()
{
  drag_mode = drag_mode_none;
  picked_mark = NULL;
  x_active = true;
  y_active = true;
} // model_view::drag_info::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the difference between the initial mouse position and the current
 *        mouse position.
 */
wxSize bf::model_view::drag_info::delta() const
{
  return wxSize
    ( mouse_position.x - mouse_origin.x, mouse_position.y - mouse_origin.y );
} // model_view::drag_info::delta()




/*----------------------------------------------------------------------------*/
const unsigned int bf::model_view::s_mark_radius(5);
const wxCoord bf::model_view::s_grip_size = 10;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param id Identifier of the window.
 * \param m The model displayed.
 */
bf::model_view::model_view( wxWindow& parent, wxWindowID id, gui_model& m )
  : wxWindow(&parent, id), m_background_pattern( 20, 20 ),
    m_model(m), m_delta(0, 0), m_image_cache(new sprite_image_cache),
    m_zoom(100), m_wireframe_drawing(true), m_graphic_drawing(true),
    m_text_drawing(true), m_placements(NULL)
{
  const wxColour dark_gray(127, 127, 127);
  const wxColour light_gray(192, 192, 192);

  wxMemoryDC dc(m_background_pattern);
  const unsigned int w = m_background_pattern.GetWidth() / 2;
  const unsigned int h = m_background_pattern.GetHeight() / 2;

  dc.SetBrush( wxBrush(dark_gray) );
  dc.SetPen( wxPen(dark_gray) );
  dc.DrawRectangle( 0, 0, w, h );
  dc.DrawRectangle( w, h, w, h );

  dc.SetBrush( wxBrush(light_gray) );
  dc.SetPen( wxPen(light_gray) );
  dc.DrawRectangle( 0, h, w, h );
  dc.DrawRectangle( w, 0, w, h );

  reload_action();
} // model_view::model_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::model_view::~model_view()
{
  delete m_image_cache;
  delete m_placements;
} // model_view::model_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the action to display.
 * \param d The date at which the action is displayed.
 */
void bf::model_view::set_date( double d )
{
  m_player.set_date(d);
  update_placements();

  Refresh();
} // model_view::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame or snapshot.
 * \brief Return the new date.
 */
double bf::model_view::next()
{
  m_player.next();
  update_placements();

  return m_player.get_date();
} // model_view::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we are at the end of the action.
 */
bool bf::model_view::is_finished() const
{
  return m_player.is_finished();
} // model_view::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration until the next change.
 */
double bf::model_view::get_duration_until_next() const
{
  return m_player.get_duration_until_next();
} // model_view::get_duration_until_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the zoom ratio.
 * \param z The zoom ratio.
 */
void bf::model_view::set_zoom( unsigned int z )
{
  m_zoom = z;
  Refresh();
} // model_view::set_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the zoom ratio.
 */
unsigned int bf::model_view::get_zoom() const
{
  return m_zoom;
} // model_view::get_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the zoom ratio such that the image fits the control.
 */
void bf::model_view::auto_zoom()
{
  const unsigned int z = GetSize().x * 100 / m_view_size.y;
  m_zoom = GetSize().y * 100 / m_view_size.y;

  if (z < m_zoom)
    m_zoom = z;

  Refresh();
} // model_view::auto_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size needed to render the action.
 */
wxSize bf::model_view::get_view_size() const
{
  return wxSize
    ( to_local_size(m_view_size.x) + 1, to_local_size(m_view_size.y) + 1);
} // model_view::get_view_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the delta applied on the action when rendering.
 */
wxPoint bf::model_view::get_view_delta() const
{
  return m_delta;
} // model_view::get_view_delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the delta applied on the action when rendering.
 * \param x The delta on the X-axis.
 * \param y The delta on the Y-axis.
 */
void bf::model_view::set_view_delta( wxCoord x, wxCoord y )
{
  m_delta.x = x;
  m_delta.y = y;

  Refresh();
} // model_view::set_view_delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the view to reload the action.
 */
void bf::model_view::reload_action()
{
  if ( m_model.has_active_action() )
    m_player.set_action( &m_model.get_active_action() );
  else
    m_player.set_action( NULL );

  update_placements();
  compute_view_size();
  Refresh();
} // model_view::reload_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the wiriframe drawing.
 */
void bf::model_view::toggle_wireframe_mode()
{
  m_wireframe_drawing = !m_wireframe_drawing;
  Refresh();
} // model_view::toggle_wireframe_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bounding box and the marks are drawn.
 */
bool bf::model_view::get_wireframe_drawing() const
{
  return m_wireframe_drawing;
} // model_view::get_wireframe_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the graphic drawing.
 */
void bf::model_view::toggle_graphic_mode()
{
  m_graphic_drawing = !m_graphic_drawing;
  Refresh();
} // model_view_ctrl::toggle_graphic_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sprites of the marks are drawn.
 */
bool bf::model_view::get_graphic_drawing() const
{
  return m_graphic_drawing;
} // model_view::get_graphic_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of the text drawing.
 */
void bf::model_view::toggle_text_mode()
{
  m_text_drawing = !m_text_drawing;
  Refresh();
} // model_view_ctrl::toggle_text_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the name and the function of the marks are drawn.
 */
bool bf::model_view::get_text_drawing() const
{
  return m_text_drawing;
} // model_view::get_text_drawing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the mark placements to display in the view.
 */
void bf::model_view::update_placements()
{
  delete m_placements;
  m_placements = NULL;

  if ( !m_model.has_active_action() )
    return;

  const double current_date( m_player.get_date() );
  const action& active_action = m_model.get_active_action();

  const action* end_action( &active_action );
  const snapshot* init =
    active_action.get_snapshot_before_or_at_date( current_date );
  const snapshot* end = active_action.get_snapshot_after_date( current_date );
  double date(0);

  if ( end != NULL )
    date = end->get_date() - init->get_date();
  else
    {
      const std::string next = active_action.get_auto_next();

      if ( m_model.has_action( next ) )
        {
          end_action = &m_model.get_action( next );
          end = end_action->get_snapshot_before_or_at_date(0);
          date = active_action.get_duration() - current_date;
        }
    }

  if ( end == NULL )
    m_placements = new model_snapshot_tweener( *init );
  else
    m_placements =
      new model_snapshot_tweener
      ( *init, *end, active_action, *end_action, date );

  m_placements->update( m_player.get_date() - init->get_date() );
} // update_placements()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the sprite.
 */
void bf::model_view::render()
{
  compute_view_size();

  wxBufferedPaintDC dc( this );

  if( IsShown() )
    {
      fill_background(dc);

      if ( m_model.has_active_action() )
        {
          const snapshot* s =
            m_model.get_active_action().get_snapshot_before_or_at_date
            (m_player.get_date());

          if ( s != NULL )
            {
              draw_marks(dc);
              draw_bounding_box(dc, *s);
              draw_resized_bounding_box(dc, *s);
            }
          draw_view_box(dc);
        }
    }
} // model_view::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the background of the control.
 * \param dc The device context for the drawings.
 */
void bf::model_view::fill_background( wxDC& dc ) const
{
  for( int y=0; y<dc.GetSize().y; y+=m_background_pattern.GetHeight() )
    for( int x=0; x<dc.GetSize().x; x+=m_background_pattern.GetWidth() )
      dc.DrawBitmap(m_background_pattern, x, y, false);

  wxPoint p[6];

  p[0] = wxPoint( get_view_size().x, 0 );
  p[1] = wxPoint( GetSize().x, 0 );
  p[2] = wxPoint( GetSize().x, GetSize().y );
  p[3] = wxPoint( 0, GetSize().y );
  p[4] = wxPoint( 0, get_view_size().y );
  p[5] = wxPoint( get_view_size().x, get_view_size().y );

  dc.SetPen( wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND)) );
  dc.SetBrush( wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND)) );
  dc.DrawPolygon(6, p);
} // model_view::fill_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the marks in the action.
 * \param dc The device context for the drawings.
 */
void bf::model_view::draw_marks( wxDC& dc ) const
{
  std::vector<mark_placement> m
    ( m_placements->mark_placement_begin(),
      m_placements->mark_placement_end() );
  snapshot::const_mark_placement_iterator it;

  std::sort( m.begin(), m.end(), compare_mark_placement_by_depth_lt() );

  for ( std::size_t i=0; i!=m.size(); ++i )
    draw_mark_placement_sprite(dc, m[i]);

  for ( std::size_t i=0; i!=m.size(); ++i )
    draw_mark_placement_wireframe(dc, m[i]);

  if (  m_drag_info.drag_mode == drag_info::drag_mode_move )
    if ( m_drag_info.picked_mark != NULL )
      {
        mark_placement p
          ( m_model.get_selected_snapshot
            ()->get_placement(m_drag_info.picked_mark->get_label()) );

        p.set_x_position(to_model_x_coordinate(m_drag_info.mouse_position.x));
        p.set_y_position(to_model_y_coordinate(m_drag_info.mouse_position.y));

        draw_mark_placement( dc, p );
      }
} // model_view::draw_marks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a mark placement.
 * \param dc The device context for the drawings.
 * \param m The mark placement to draw.
 */
void bf::model_view::draw_mark_placement
( wxDC& dc, const mark_placement& m ) const
{
  draw_mark_placement_sprite(dc, m);
  draw_mark_placement_wireframe(dc, m);
} // model_view::draw_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a mark placement without the sprite.
 * \param dc The device context for the drawings.
 * \param m The mark placement to draw.
 */
void bf::model_view::draw_mark_placement_wireframe
( wxDC& dc, const mark_placement& m ) const
{
  draw_mark_placement_cross(dc, m);
  draw_mark_placement_box(dc, m);
  draw_mark_placement_text(dc, m);
} // model_view::draw_mark_placement_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the cross of a mark placement.
 * \param dc The device context for the drawings.
 * \param m The mark placement to draw.
 */
void bf::model_view::draw_mark_placement_cross
( wxDC& dc, const mark_placement& m ) const
{
  if ( !m_wireframe_drawing )
    return;

  const wxPoint center
    ( to_local_x_coordinate(m.get_x_position()),
      to_local_y_coordinate(m.get_y_position()) );
  wxPen pen;

  if ( m_model.get_main_mark_selection() == m.get_mark() )
    pen.SetColour(*wxRED);
  else
    pen.SetColour(*wxGREEN);

  if ( !m.is_visible() )
    pen.SetStyle( wxDOT );

  dc.SetPen(pen);
  dc.SetBrush( wxBrush(*wxBLACK, wxTRANSPARENT) );

  dc.DrawCircle( center.x, center.y, s_mark_radius );

  claw::math::coordinate_2d<double> c
    ( to_local_x_coordinate(m.get_x_position()),
      to_local_y_coordinate(m.get_y_position()) );

  claw::math::coordinate_2d<double> p =
    to_local_rotated_point
    ( m.get_x_position(), m.get_y_position() + 2 * s_mark_radius,
      m.get_x_position(), m.get_y_position(), m.get_angle() );

  dc.DrawLine( c.x, c.y, p.x, p.y );

  p = to_local_rotated_point
    ( m.get_x_position() + 2 * s_mark_radius, m.get_y_position(),
      m.get_x_position(), m.get_y_position(), m.get_angle() );

  dc.DrawLine( c.x, c.y, p.x, p.y );
} // model_view::draw_mark_placement_cross()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the box of a mark placement.
 * \param dc The device context for the drawings.
 * \param m The mark placement to draw.
 */
void bf::model_view::draw_mark_placement_box
( wxDC& dc, const mark_placement& m ) const
{
  if ( !m_wireframe_drawing )
    return;

  const wxRect r
    ( to_local_rect
      ( m.get_x_position() - m.get_width() / 2,
        m.get_y_position() - m.get_height() / 2,
        m.get_width(), m.get_height() ) );

  dc.SetBrush( wxBrush(*wxBLACK, wxTRANSPARENT) );

  if ( m_model.get_main_mark_selection() == m.get_mark() )
    {
      dc.SetPen( wxPen(*wxRED, 1, wxSOLID) );
      draw_grips(dc, r);
    }
  else
    dc.SetPen( wxPen(*wxGREEN, 1, wxSOLID) );

  dc.DrawRectangle(r);
} // model_view::draw_mark_placement_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the name of a mark placement.
 * \param dc The device context for the drawings.
 * \param m The mark placement to draw.
 */
void bf::model_view::draw_mark_placement_text
( wxDC& dc, const mark_placement& m ) const
{
  if ( !m_text_drawing )
    return;

  wxFont font(dc.GetFont());
  font.SetPointSize(8);
  dc.SetFont(font);
  dc.SetTextForeground(*wxWHITE);
  dc.SetTextBackground(*wxBLACK);
  dc.SetBackgroundMode(wxSOLID);

  const wxRect r( get_local_bounding_box(m) );
  wxString s(std_to_wx_string(m.get_mark()->get_label()));

  dc.SetBrush(*wxBLACK_BRUSH);
  dc.DrawText
    ( s, r.GetLeft() + r.GetWidth() / 2 - dc.GetTextExtent(s).x / 2,
      r.GetBottom() );

  if ( !m.get_collision_function().empty() )
    {
      font.SetStyle( wxFONTSTYLE_ITALIC );
      dc.SetFont(font);
      dc.SetTextForeground(*wxLIGHT_GREY);

      const wxCoord d = dc.GetTextExtent(s).y;
      s = std_to_wx_string(m.get_collision_function() + "()");

      dc.DrawText
        ( s, r.GetLeft() + r.GetWidth() / 2 - dc.GetTextExtent(s).x / 2,
          r.GetBottom() + d );
    }
} // model_view::draw_mark_placement_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the sprite of a mark placement.
 * \param dc The device context for the drawings.
 * \param m The mark placement to draw.
 */
void bf::model_view::draw_mark_placement_sprite
( wxDC& dc, const mark_placement& m ) const
{
  if ( m_graphic_drawing && m.get_mark()->has_animation() && m.is_visible() )
    {
      sprite s( m_player.get_current_sprite_for(m.get_mark()) );
      if ( m.get_mark()->apply_angle_to_animation() )
	s.set_angle(m.get_angle());
    
      const std::pair<wxBitmap, wxPoint> img =
        m_image_cache->get_image
        ( s, to_local_size(s.width()), to_local_size(s.height()) );
      const wxPoint p
        ( to_local_x_coordinate( m.get_x_position() )
          - img.first.GetWidth() / 2,
          to_local_y_coordinate( m.get_y_position() )
          - img.first.GetHeight() / 2 );

      dc.DrawBitmap( img.first, p );
    }
} // model_view::draw_mark_placement_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the bounding box of the action.
 * \param dc The device context for the drawings.
 * \param s The snapshot for which we draw the bounding box.
 */
void bf::model_view::draw_bounding_box( wxDC& dc, const snapshot& s ) const
{
  if ( !m_wireframe_drawing )
    return;

  const wxRect r( to_local_rect( 0, 0, s.get_width(), s.get_height() ) );

  dc.SetBrush( wxBrush(*wxBLACK, wxTRANSPARENT) );

  if ( m_model.bounding_box_is_selected() )
    dc.SetPen( wxPen(*wxRED, 1, wxSOLID) );
  else
    dc.SetPen( wxPen(*wxBLUE, 1, wxSOLID) );

  dc.DrawRectangle(r);

  if ( m_model.bounding_box_is_main_selection() )
    draw_grips(dc, r);
} // model_view::draw_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the bounding box of the action during a modification of size.
 * \param dc The device context for the drawings.
 * \param s The snapshot for which we draw the bounding box.
 */
void bf::model_view::draw_resized_bounding_box
( wxDC& dc, const snapshot& s ) const
{
  if ( m_drag_info.drag_mode == drag_info::drag_mode_size )
    {
      wxRect r = get_drag_mode_size_box();

      const wxRect re
        ( to_local_x_coordinate(r.x), to_local_y_coordinate(r.y),
          to_local_size(r.width), to_local_size(r.height) );

      dc.SetBrush( wxBrush(*wxBLACK, wxTRANSPARENT) );
      dc.SetPen( wxPen(*wxBLUE, 1, wxDOT) );

      dc.DrawRectangle(re);
    }
} // model_view::draw_resized_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the grip around a box.
 * \param dc The device context for the drawings.
 * \param r The box around which we draw the grips.
 */
void bf::model_view::draw_grips( wxDC& dc, const wxRect& r ) const
{
  const std::vector<wxRect> grips( compute_grips(r) );

  for ( std::size_t i=0; i!=grips.size(); ++i )
    dc.DrawRectangle( grips[i] );
} // model_view::draw_grips()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the visual bounding box of the action.
 * \param dc The device context for the drawings.
 */
void bf::model_view::draw_view_box( wxDC& dc ) const
{
  const wxRect r
    ( to_local_rect
      (m_view_origin.x, m_view_origin.y, m_view_size.x, m_view_size.y) );

  dc.SetBrush( wxBrush(*wxBLACK, wxTRANSPARENT) );

  dc.SetPen( wxPen(*wxBLACK, 1, wxSOLID) );
  dc.DrawRectangle
    ( r.GetX() - 1, r.GetY() - 1, r.GetWidth() + 2, r.GetHeight() + 2 );

  dc.SetPen( wxPen(*wxWHITE, 1, wxDOT) );
  dc.DrawRectangle
    ( r.GetX() - 1, r.GetY() - 1, r.GetWidth() + 2, r.GetHeight() + 2 );
} // model_view::draw_view_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the grips around a box.
 * \param r The box around which the grips are.
 * \return The rectangles representing the grips from the top left to the bottom
 *         right.
 */
std::vector<wxRect> bf::model_view::compute_grips( const wxRect& r ) const
{
  std::vector<wxRect> result;
  result.reserve(8);
  wxRect grip
    ( r.GetX() - s_grip_size, r.GetY() - s_grip_size, s_grip_size,
      s_grip_size );

  result.push_back( grip );

  grip.SetX( r.GetX() + r.GetWidth() / 2 - s_grip_size / 2 );
  result.push_back( grip );

  grip.SetX( r.GetX() + r.GetWidth() );
  result.push_back( grip );

  grip.SetY( r.GetY() + r.GetHeight() / 2 - s_grip_size / 2 );
  grip.SetX( r.GetX() - s_grip_size );
  result.push_back( grip );

  grip.SetX( r.GetX() + r.GetWidth() );
  result.push_back( grip );

  grip.SetY( r.GetY() + r.GetHeight() );
  grip.SetX( r.GetX() - s_grip_size );
  result.push_back( grip );

  grip.SetX( r.GetX() + r.GetWidth() / 2 - s_grip_size / 2 );
  result.push_back( grip );

  grip.SetX( r.GetX() + r.GetWidth() );
  result.push_back( grip );

  return result;
} // model_view::compute_grips()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the display bounding box of a mark placement.
 *
 * This is not the real bounding box of the placement but a box that would be
 * used to detect clics or to place the handles.
 *
 * \param m The placement for which we want the box.
 */
wxRect bf::model_view::get_local_bounding_box( const mark_placement& m ) const
{
  wxCoord w, h;

  if ( (m.get_width() != 0) && (m.get_height() != 0) )
    {
      w = m.get_width();
      h = m.get_height();
    }
  else
    w = h = s_mark_radius * 2;

  return
    to_local_rect
    ( m.get_x_position() - w / 2, m.get_y_position() - h / 2, w, h );
} // get_local_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the value of m_view_size;
 */
void bf::model_view::compute_view_size()
{
  double min_x=0;
  double min_y=0;
  double max_x=0;
  double max_y=0;

  if ( m_model.has_active_action() )
    m_model.get_active_action().get_bounds(min_x, min_y, max_x, max_y);

  m_view_origin.set( min_x-5, min_y-5 );
  m_view_size.set( max_x - min_x + 11, max_y - min_y + 11 );
} // model_view::compute_view_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the zoom from a given value. Use this function to convert a
 *        distance in the control into a distance in the model.
 * \param v The value to convert.
 */
double bf::model_view::unzoom( double v ) const
{
  return 100 * v / (double)m_zoom;
} // model_view::unzoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an X-coordinate of the model to the equivalent in the view.
 * \param x The coordinate to convert, in the model's point of view.
 */
double bf::model_view::to_local_x_coordinate( double x ) const
{
  return (x - m_view_origin.x) * (double)m_zoom / 100.0 - m_delta.x;
} // model_view::to_local_x_coordinate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a Y-coordinate of the model to the equivalent in the view.
 * \param y The coordinate to convert, in the model's point of view.
 */
double bf::model_view::to_local_y_coordinate( double y ) const
{
  return (m_view_size.y - (y - m_view_origin.y))
    * (double)m_zoom / 100.0 - m_delta.y;
} // model_view::to_local_y_coordinate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an X-coordinate of the view to the equivalent in the model.
 * \param x The coordinate to convert, in the control coordinates.
 */
double bf::model_view::to_model_x_coordinate( double x ) const
{
  return m_view_origin.x + 100 * ( x - m_delta.x ) / (double)m_zoom;
} // model_view::to_model_x_coordinate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a Y-coordinate of the view to the equivalent in the model.
 * \param y The coordinate to convert, in the control coordinates.
 */
double bf::model_view::to_model_y_coordinate( double y ) const
{
  return m_view_size.y + m_view_origin.y
    - 100 * ( y + m_delta.y ) / (double)m_zoom;
} // model_view::to_model_y_coordinate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a size of the model to the equivalent in the view.
 * \param s The size to convert.
 */
double bf::model_view::to_local_size( double s ) const
{
  return s * (double)m_zoom / 100.0;
} // model_view::to_local_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a size of the view to the equivalent in the model.
 * \param s The size to convert.
 */
double bf::model_view::to_model_size( double s ) const
{
  return 100 * s / (double)m_zoom;
} // model_view::to_local_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a rectangle in the model to the equivalent in the view.
 * \param x X-coordinate of the bottom left corner.
 * \param y Y-coordinate of the bottom left corner.
 * \param w The width of the rectangle.
 * \param h The height of the rectangle.
 */
wxRect
bf::model_view::to_local_rect( double x, double y, double w, double h ) const
{
  return wxRect
    ( to_local_x_coordinate(x) + 0.5,
      to_local_y_coordinate(y) - to_local_size(h) + 0.5,
      to_local_size(w) + 0.5, to_local_size(h) + 0.5 );
} // model_view::to_local_rect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the local coordinate of a point from the model, rotated around a
 *        given center.
 * \param x X-coordinate of the point to rotate.
 * \param y Y-coordinate of the point to rotate.
 * \param cx X-coordinate of the center of the rotation.
 * \param cy Y-coordinate of the center of the rotation.
 * \param a The angle.
 */
claw::math::coordinate_2d<double> bf::model_view::to_local_rotated_point
( double x, double y, double cx, double cy, double a ) const
{
  double lx = to_local_x_coordinate(x);
  double ly = to_local_y_coordinate(y);
  double lcx = to_local_x_coordinate(cx);
  double lcy = to_local_y_coordinate(cy);

  claw::math::coordinate_2d<double> result(lcx, lcy);

  result.x += (lx - lcx) * cos(a) + (ly - lcy) * sin(a);
  result.y += -(lx - lcx) * sin(a) + (ly - lcy) * cos(a);

  return result;
} // model_view::to_local_rotated_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a given position is in the bounding box.
 * \param point The considered position.
 */
bool bf::model_view::coordinate_in_bounding_box(const  wxPoint point) const
{
  const snapshot* s =
    m_model.get_active_action().get_snapshot_before_or_at_date
    (m_player.get_date());

  return to_local_rect(0, 0, s->get_width(), s->get_height()).Contains(point);
} // model_view::coordinate_in_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a given position is in a anchor.
 * \param point The considered position.
 */
bool bf::model_view::coordinate_in_anchors(const wxPoint& point)
{
  const snapshot* snap =
    m_model.get_active_action().get_snapshot_before_or_at_date
    (m_player.get_date());

  if ( snap == NULL )
    return false;

  if ( m_model.bounding_box_is_selected() )
    return coordinate_in_anchors
      ( point, to_local_rect( 0, 0, snap->get_width(), snap->get_height() ) );
  else if ( m_model.has_mark_selection() )
    return coordinate_in_anchors
      ( point,
        get_local_bounding_box
        ( snap->get_placement(m_model.get_main_mark_selection()) ) );
  else
    return false;
} // model_view::coordinate_in_anchors()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a given position is in a anchor.
 * \param point The considered position.
 * \param box The rectangle for which we check the anchors.
 */
bool bf::model_view::coordinate_in_anchors
( const wxPoint& point, const wxRect& box )
{
  std::vector<wxRect> anchors = compute_grips(box);
  CLAW_ASSERT( anchors.size() == 8,
               "compute_grips() did not return the correct count of anchors." );
  wxPoint mouse_pos;
  bool result(true);

  const wxRect top_left( anchors[0] );
  const wxRect middle_top( anchors[1] );
  const wxRect top_right( anchors[2] );
  const wxRect middle_left( anchors[3] );
  const wxRect middle_right( anchors[4] );
  const wxRect bottom_left( anchors[5] );
  const wxRect middle_bottom( anchors[6] );
  const wxRect bottom_right( anchors[7] );

  if ( top_left.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetBottomRight();
      mouse_pos = box.GetTopLeft();
    }
  else if ( top_right.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetBottomLeft();
      mouse_pos = box.GetTopRight();
    }
  else if ( bottom_left.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetTopRight();
      mouse_pos = box.GetBottomLeft();
    }
  else if ( bottom_right.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetTopLeft();
      mouse_pos = box.GetBottomRight();
    }
  else if ( middle_left.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetTopRight();
      mouse_pos = box.GetTopLeft();
      m_drag_info.y_active = false;
    }
  else if ( middle_right.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetTopLeft();
      mouse_pos = box.GetTopRight();
      m_drag_info.y_active = false;
    }
  else if ( middle_bottom.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetTopLeft();
      mouse_pos = box.GetBottomLeft();
      m_drag_info.x_active = false;
    }
  else if ( middle_top.Contains(point) )
    {
      m_drag_info.mouse_origin = box.GetBottomLeft();
      mouse_pos = box.GetTopLeft();
      m_drag_info.x_active = false;
    }
  else
    result = false;

  if ( result )
    {
      m_drag_info.drag_mode = drag_info::drag_mode_size;
      m_drag_info.mouse_position = mouse_pos;
    }

  return result;
} // model_view::coordinate_in_anchors()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the position of the mouse according to the constraints.
 * \param position The position of the mouse in the model.
 */
void bf::model_view::update_mouse_position( const wxPoint& position )
{
  if ( m_drag_info.x_active )
    m_drag_info.mouse_position.x = position.x;

  if ( m_drag_info.y_active )
    m_drag_info.mouse_position.y = position.y;
} // model_view::update_mouse_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search a mark at a given position.
 * \param point The considered position.
 */
const bf::mark* bf::model_view::find_mark_at(const wxPoint& point) const
{
  const mark* result = NULL;

  if ( m_placements != NULL )
    {
      model_snapshot_tweener::const_mark_placement_iterator it;

      for ( it=m_placements->mark_placement_begin();
            (it!=m_placements->mark_placement_end()) && (result == NULL);
            ++it )
        {
          const wxRect box = get_local_bounding_box(*it);

          if ( box.Contains(point) )
            result = it->get_mark();
        }
    }

  return result;
} // model_view::find_mark_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the selected mark.
 * \param point The new position of the mark.
 */
void bf::model_view::move_mark(const wxPoint& point)
{
  if ( m_model.has_active_action() && m_drag_info.picked_mark )
    {
      placement_event event
        ( m_drag_info.picked_mark->get_label(),
          to_model_x_coordinate(point.x),
          to_model_y_coordinate(point.y),
          0, 0,
          placement_event::move_event_type, GetId() );
      event.SetEventObject(this);
      ProcessEvent(event);
    }
} // model_view::move_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the bounding box.
 * \param point The new position of the box.
 */
void bf::model_view::move_box(const wxPoint& point)
{
  if ( m_model.has_active_action() )
    {
      const snapshot* snap =
        m_model.get_active_action().get_snapshot_before_or_at_date
        (m_player.get_date());

      bounding_box_event event
        ( unzoom(m_drag_info.delta().x), -unzoom(m_drag_info.delta().y),
          snap->get_width(), snap->get_height(),
          bounding_box_event::change_event_type, GetId() );
      event.SetEventObject(this);
      ProcessEvent(event);
    }
} // model_view::move_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the box when resizing an item.
 */
wxRect bf::model_view::get_drag_mode_size_box() const
{
  CLAW_PRECOND( m_drag_info.drag_mode == drag_info::drag_mode_size );
  CLAW_PRECOND( m_model.bounding_box_is_selected()
                || m_model.has_mark_selection() );

  const snapshot* snap =
    m_model.get_active_action().get_snapshot_before_or_at_date
    (m_player.get_date());

  if ( m_model.bounding_box_is_selected() )
    return get_drag_mode_size_box( snap->get_width(), snap->get_height() );
  else
    {
      const mark_placement& p =
        snap->get_placement( m_model.get_main_mark_selection() );
      return get_drag_mode_size_box( p.get_width(), p.get_height() );
    }
} // model_view::get_drag_mode_size_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the box when resizing an item.
 * \param w The width to return if resizing on the x axis is disabled.
 * \param h The width to return if resizing on the y axis is disabled.
 */
wxRect bf::model_view::get_drag_mode_size_box( double w, double h ) const
{
  wxRect result;

  if ( m_drag_info.x_active )
    {
      result.x =
        to_model_x_coordinate
        ( std::min( m_drag_info.mouse_position.x, m_drag_info.mouse_origin.x ));
      result.width =
        to_model_size
        ( std::abs
          ( m_drag_info.mouse_position.x - m_drag_info.mouse_origin.x ) + 1 );
    }
  else
    {
      result.x = to_model_x_coordinate(m_drag_info.mouse_origin.x);
      result.width = w;
    }

  if ( m_drag_info.y_active )
    {
      result.y =
        to_model_y_coordinate
        ( std::min
          ( m_drag_info.mouse_position.y, m_drag_info.mouse_origin.y ) );
      result.height =
        to_model_size
        ( std::abs
          ( m_drag_info.mouse_position.y - m_drag_info.mouse_origin.y ) + 1);
    }
  else
    {
      result.y = to_model_y_coordinate(m_drag_info.mouse_origin.y);
      result.height = h;
    }

  return result;
} // model_view::get_drag_mode_size_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in size mode on the bounding box of the
 *        model.
 */
void bf::model_view::apply_drag_mode_size_box()
{
  CLAW_PRECOND( m_drag_info.drag_mode == drag_info::drag_mode_size );

  wxRect box = get_drag_mode_size_box();

  const snapshot* snap =
    m_model.get_active_action().get_snapshot_before_or_at_date
    (m_player.get_date());

  if ( snap )
    {
      if ( (box.x != 0)
           || (box.y != 0)
           || (box.width != snap->get_width())
           || (box.height != snap->get_height()) )
        {
          bounding_box_event event
            ( box.x, box.y - box.height, box.width, box.height,
              bounding_box_event::change_event_type, GetId() );
          event.SetEventObject(this);
          ProcessEvent(event);
        }
    }
} // model_view::apply_drag_mode_size_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the result of a drag in size mode on the bounding box of a mark.
 */
void bf::model_view::apply_drag_mode_size_mark()
{
  CLAW_PRECOND( m_drag_info.drag_mode == drag_info::drag_mode_size );

  wxRect box = get_drag_mode_size_box();
  if ( m_model.has_mark_selection() )
    {
      placement_event event
        ( m_model.get_main_mark_selection()->get_label(),
          box.x + (double)box.width / 2,
          box.y - (double)box.height / 2, box.width, box.height,
          placement_event::size_event_type, GetId() );
      event.SetEventObject(this);
      ProcessEvent(event);
    }
} // model_view::apply_drag_mode_size_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event "selection_change".
 * \param label The name of the new selected mark.
 */
void bf::model_view::send_event_selection_change(const std::string& label)
{
  mark_event event
    ( label, mark_event::selection_change_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // model_view::send_event_selection_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send an event to inform that the bounding box has been selected.
 */
void bf::model_view::send_event_select_box()
{
  bounding_box_event event( bounding_box_event::select_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // model_view::send_event_select_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the event to inform that the selection has been cleared..
 */
void bf::model_view::send_event_clear_selection()
{
  mark_event event( mark_event::selection_change_event_type, GetId() );
  event.SetEventObject(this);
  ProcessEvent(event);
} // model_view::send_event_clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::model_view::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  render();
} // model_view::on_paint()

/*----------------------------------------------------------------------------*/
/**
 * \brief The mouse moves over the window.
 * \param event This event occured.
 */
void bf::model_view::on_mouse_move( wxMouseEvent& event )
{
  if ( m_drag_info.drag_mode != drag_info::drag_mode_none )
    {
      if ( event.LeftIsDown() )
        {
          update_mouse_position(event.GetPosition());

          if ( (m_drag_info.drag_mode == drag_info::drag_mode_pick) &&
               ( ( std::abs(event.GetPosition().x
                            - m_drag_info.mouse_origin.x) >= 10 )
                 || ( std::abs(event.GetPosition().y
                               - m_drag_info.mouse_origin.y) >= 10 ) ) )
               m_drag_info.drag_mode = drag_info::drag_mode_move;
        }
      else
        m_drag_info.clear();

      Refresh();
    }
} // model_view::on_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief The left button is down.
 * \param event This event occured.
 */
void bf::model_view::on_left_down( wxMouseEvent& event )
{
  if ( !m_model.has_active_action() )
    return;
  if ( !m_model.has_selected_snapshot() )
    return;

  const mark* m = find_mark_at( event.GetPosition() );
  m_drag_info.clear();
  m_drag_info.mouse_origin = event.GetPosition();

  if ( m != NULL )
    {
      m_drag_info.drag_mode = drag_info::drag_mode_pick;
      m_drag_info.picked_mark = m;
    }
  else if ( coordinate_in_bounding_box( event.GetPosition() ) )
    m_drag_info.drag_mode = drag_info::drag_mode_pick;
  else if ( coordinate_in_anchors( event.GetPosition() ) )
    m_drag_info.drag_mode = drag_info::drag_mode_size;
  else
    m_drag_info.drag_mode = drag_info::drag_mode_selection;
} // model_view::on_left_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief The left button is up.
 * \param event This event occured.
 */
void bf::model_view::on_left_up( wxMouseEvent& event )
{
  switch ( m_drag_info.drag_mode )
    {
    case drag_info::drag_mode_pick:
      if ( m_drag_info.picked_mark != NULL )
        send_event_selection_change(m_drag_info.picked_mark->get_label());
      else
        send_event_select_box();
      break;
    case drag_info::drag_mode_move:
      if ( m_drag_info.picked_mark != NULL )
        move_mark(event.GetPosition());
      else
        move_box(event.GetPosition());
      break;
    case drag_info::drag_mode_size:
      if ( m_model.has_mark_selection() )
        apply_drag_mode_size_mark();
      else
        apply_drag_mode_size_box();
      break;
    case drag_info::drag_mode_selection:
      send_event_clear_selection();
      break;
    default:
      break;
    }
  m_drag_info.clear();
} // model_view::on_left_up()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::model_view, wxWindow)
  EVT_PAINT( bf::model_view::on_paint )

  EVT_MOTION( bf::model_view::on_mouse_move )
  EVT_LEFT_DOWN( bf::model_view::on_left_down )
  EVT_LEFT_UP( bf::model_view::on_left_up )
END_EVENT_TABLE()
