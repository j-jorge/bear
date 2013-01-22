/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A slider control.
 * \author Sébastien Angibaud
 */
#ifndef __BF_SLIDER_CTRL_HPP__
#define __BF_SLIDER_CTRL_HPP__

#include <wx/panel.h>

#include <set>

#define slider_event_handler(func)                                      \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(slider_event::function_type, &func)

#define EVT_SLIDER_VALUE_CHANGE(id, func)                              \
  wx__DECLARE_EVT1( bf::slider_event::value_change_event_type,         \
                    id, slider_event_handler(func) )

#define tick_event_handler(func)                                        \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(tick_event::function_type, &func)

#define EVT_TICK_MOVE(id, func)                              \
  wx__DECLARE_EVT1( bf::tick_event::move_event_type,         \
                    id, tick_event_handler(func) )

namespace bf
{
  /**
   * \brief Event sent when the value of a slider has changed.
   * \author Sébastien Angibaud
   */
  class slider_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(slider_event&);

  public:
    slider_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    slider_event
    ( double value, wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    slider_event( const slider_event& that );

    wxEvent* Clone() const;

    double get_value() const;

  public:
    static const wxEventType value_change_event_type;

  private:
    /** \brief The value of the slider. */
    double m_value;
  }; // class slider_event

  /**
   * \brief Event sent when a tick of the slider has moved.
   * \author Sébastien Angibaud
   */
  class tick_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(tick_event&);

  public:
    tick_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    tick_event
    ( double initial_value, double new_value, bool copy,
      wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    tick_event( const tick_event& that );

    wxEvent* Clone() const;

    double get_initial_value() const;
    double get_new_value() const;
    bool get_copy() const;

  public:
    static const wxEventType move_event_type;

  private:
    /** \brief The initial value of the tick. */
    double m_initial_value;

    /** \brief The new value of the tick. */
    double m_new_value;

    /** \brief The copy statut. */
    double m_copy;

  }; // class tick_event

  /**
   * \brief A slider control.
   * \author Sébastien Angibaud
   */
  class slider_ctrl:
    public wxPanel
  {
  public:
    typedef wxPanel super;

    typedef double tick;
    typedef std::set<tick> ticks;

    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_PREVIOUS,
        ID_NEXT
      }; // enum control_id

  private:
    /** \brief Informations on a drag local to the control. */
    class drag_info
    {
    public:
      enum drag_mode_type
        {
          drag_mode_none,
          drag_mode_tick_move,
          drag_mode_move
        }; // enum drag_mode

    public:
      drag_info();

      wxSize delta() const;

    public:
      /** \brief The current mode of the drag. */
      drag_mode_type drag_mode;

      /** \brief Initial value of the tick. */
      double initial_value;

      /** \brief Origin of the drag. */
      wxPoint mouse_origin;

      /** \brief Position of the mouse during the drag. */
      wxPoint mouse_position;
    }; // struct drag_info

  public:
    slider_ctrl
    (wxWindow* parent, wxWindowID id, tick value=0,
     tick min_value=0, tick max_value=100);

    void render();

    void set_ticks(const ticks* t);
    double get_value() const;
    void set_value(double value);
    void set_max_value(double max);

  private:
    bool has_tick( double value ) const;

    void send_event_change_value();
    void send_event_move_tick
    ( double initial_value, double new_value, bool copy );
    unsigned int get_slider_width() const;

    void render_slider( wxDC& dc ) const;
    void render_value( wxDC& dc ) const;
    void render_drag_info( wxDC& dc ) const;
    void render_ticks( wxDC& dc ) const;
    void render_tick( wxDC& dc, tick t ) const;
    void render_tick
    ( wxDC& dc, unsigned int position,
      const wxColour& colour, const wxBrush& brush ) const;

    double nearest_tick( double v) const;
    double get_value(int pos) const;
    unsigned int get_position(double value) const;
    void apply_drag_mode_move( bool shift );
    void apply_drag_mode_tick_move( bool control, bool alt );

    void on_size(wxSizeEvent& event);
    void on_paint(wxPaintEvent& event);
    void on_mouse_left_down(wxMouseEvent& event);
    void on_mouse_move(wxMouseEvent& event);
    void on_mouse_left_up(wxMouseEvent& event);
    void on_mouse_left_dclick(wxMouseEvent& event);

  private:
    /** \brief Info about the current dragging. */
    drag_info* m_drag_info;

    /** \brief The current value. */
    double m_value;

    /** \brief The minimum value. */
    double m_min_value;

    /** \brief The maximum value. */
    double m_max_value;

    /** \brief The list of ticks. */
    const ticks* m_ticks;

    /** \brief The border near the slider. */
    static const unsigned int s_slider_border;

    DECLARE_EVENT_TABLE()

      }; // class slider_ctrl
} // namespace bf

#endif // __BF_SLIDER_CTRL_HPP__
