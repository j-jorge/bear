/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This control displays the active action of a model.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_VIEW_HPP__
#define __BF_MODEL_VIEW_HPP__

#include "bf/action_player.hpp"
#include "bf/sprite_image_cache.hpp"

#include <wx/window.h>
#include <vector>

#define mark_event_handler(func)                                       \
  (wxObjectEventFunction)(wxEventFunction)                             \
    wxStaticCastEvent(mark_event::function_type, &func)

#define EVT_MARK_SELECTION_CHANGE(id, func)                          \
  wx__DECLARE_EVT1( bf::mark_event::selection_change_event_type,     \
                    id, mark_event_handler(func) )

#define placement_event_handler(func)                                  \
  (wxObjectEventFunction)(wxEventFunction)                             \
    wxStaticCastEvent(placement_event::function_type, &func)

#define EVT_PLACEMENT_MOVE(id, func)                       \
  wx__DECLARE_EVT1( bf::placement_event::move_event_type,  \
                    id, placement_event_handler(func) )
#define EVT_PLACEMENT_SIZE(id, func)                           \
  wx__DECLARE_EVT1( bf::placement_event::size_event_type,      \
                    id, placement_event_handler(func) )

#define bounding_box_event_handler(func)                               \
  (wxObjectEventFunction)(wxEventFunction)                             \
    wxStaticCastEvent(bounding_box_event::function_type, &func)

#define EVT_BOUNDING_BOX_CHANGE(id, func)                        \
  wx__DECLARE_EVT1( bf::bounding_box_event::change_event_type,   \
                    id, bounding_box_event_handler(func) )

#define EVT_BOUNDING_BOX_SELECTED(id, func)                     \
  wx__DECLARE_EVT1( bf::bounding_box_event::select_event_type,  \
                    id, bounding_box_event_handler(func) )

namespace bf
{
  class gui_model;
  class snapshot;
  class mark_placement;
  class model_snapshot_tweener;
  class workspace_environment;

  /**
   * \brief Event sent when the value of a selected mark has changed.
   * \author Sebastien Angibaud
   */
  class mark_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(mark_event&);

  public:
    mark_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    mark_event
    ( const std::string& name, wxEventType t = wxEVT_NULL,
      wxWindowID id = wxID_ANY );
    mark_event( const mark_event& that );

    wxEvent* Clone() const;

    const std::string& get_value() const;

  public:
    static const wxEventType selection_change_event_type;

  private:
    /** \brief The name of the selected mark. */
    const std::string m_value;

  }; // class mark_event

  /**
   * \brief Event sent when the placement of a mark changes.
   * \author Sebastien Angibaud
   */
  class placement_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(placement_event&);

  public:
    placement_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    placement_event
    ( const std::string& name, double x, double y, double width,
      double height, wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

    const std::string& get_label() const;
    double get_x() const;
    double get_y() const;
    double get_width() const;
    double get_height() const;

  public:
    static const wxEventType move_event_type;
    static const wxEventType size_event_type;

  private:
    /** \brief The name of the mark to move. */
    const std::string m_label;

    /** \brief The new x position of the mark to move. */
    double m_x;

    /** \brief The new y position of the mark to move. */
    double m_y;

    /** \brief The new width of the mark. */
    double m_width;

    /** \brief The new height of the mark. */
    double m_height;

  }; // class placement_event

  /**
   * \brief Event sent when the bounding_box changes.
   * \author Sébastien Angibaud
   */
  class bounding_box_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(bounding_box_event&);

  public:
    bounding_box_event( wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );
    bounding_box_event
    ( double left, double bottom, double width, double height,
      wxEventType t = wxEVT_NULL, wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

    double get_left() const;
    double get_bottom() const;
    double get_width() const;
    double get_height() const;

  public:
    static const wxEventType change_event_type;
    static const wxEventType select_event_type;

  private:
    /** \brief The new left position of the bounding box. */
    double m_left;

    /** \brief The new bottom position of the bounding box. */
    double m_bottom;

    /** \brief The new width of the bounding box. */
    double m_width;

    /** \brief The new height of the bounding box. */
    double m_height;

    /** \brief Indicates if mark must be modified in x coordinate. */
    bool m_mark_move_x;

    /** \brief Indicates if mark must be modified in y coordinate. */
    bool m_mark_move_y;

  }; // class bounding_box_event

  /**
   * \brief This control displays a action.
   * \author Julien Jorge
   */
  class model_view:
    public wxWindow
  {
  private:
    /** \brief Informations on a drag local to the view. */
    class drag_info
    {
    public:
      enum drag_mode_type
        {
          drag_mode_none,

          /** \brief A rectangular selection. */
          drag_mode_selection,

          /** \brief Moving the selected marks or the bounding box. */
          drag_mode_move,

          /** \brief Resizing the bounding box. */
          drag_mode_size,

          /** \brief Picking a mark, or the bounding box. */
          drag_mode_pick

        }; // enum drag_mode

    public:
      drag_info();
      void clear();

      wxSize delta() const;

    public:
      /** \brief The current mode of the drag. */
      drag_mode_type drag_mode;

      /** \brief The mark picked at the begining of the drag. */
      const mark* picked_mark;

      /** \brief Origin of the drag. */
      wxPoint mouse_origin;

      /** \brief Position of the mouse during the drag. */
      wxPoint mouse_position;

      /** \brief Tell if the drag conterns the X-axis. */
      bool x_active;

      /** \brief Tell if the drag conterns the Y-axis. */
      bool y_active;

    }; // struct drag_info

  public:
    model_view
    ( wxWindow& parent, wxWindowID id, gui_model& m, 
      workspace_environment& env );
    ~model_view();

    void set_date( double d );
    double next();
    bool is_finished() const;
    double get_duration_until_next() const;

    void set_zoom( unsigned int z );
    unsigned int get_zoom() const;
    void auto_zoom();

    wxSize get_view_size() const;
    wxPoint get_view_delta() const;
    void set_view_delta( wxCoord x, wxCoord y );

    void reload_action();

    void toggle_wireframe_mode();
    bool get_wireframe_drawing() const;

    void toggle_graphic_mode();
    bool get_graphic_drawing() const;

    void toggle_text_mode();
    bool get_text_drawing() const;

  private:
    void update_placements();

    void render();

    void fill_background( wxDC& dc ) const;
    void draw_marks( wxDC& dc ) const;
    void draw_mark_placement( wxDC& dc, const mark_placement& m ) const;
    void draw_mark_placement_wireframe
    ( wxDC& dc, const mark_placement& m ) const;
    void draw_mark_placement_cross( wxDC& dc, const mark_placement& m ) const;
    void draw_mark_placement_box( wxDC& dc, const mark_placement& m ) const;
    void draw_mark_placement_text( wxDC& dc, const mark_placement& m ) const;
    void draw_mark_placement_sprite( wxDC& dc, const mark_placement& m ) const;
    void draw_bounding_box( wxDC& dc, const snapshot& s ) const;
    void draw_resized_bounding_box( wxDC& dc, const snapshot& s ) const;
    void draw_grips( wxDC& dc, const wxRect& r ) const;
    void draw_view_box( wxDC& dc ) const;

    std::vector<wxRect> compute_grips( const wxRect& r ) const;
    wxRect get_local_bounding_box( const mark_placement& m ) const;

    void compute_view_size();

    double unzoom( double x ) const;
    double to_local_x_coordinate( double x ) const;
    double to_local_y_coordinate( double y ) const;
    double to_model_x_coordinate( double x ) const;
    double to_model_y_coordinate( double y ) const;
    double to_local_size( double s ) const;
    double to_model_size( double s ) const;
    wxRect to_local_rect( double x, double y, double w, double h ) const;
    claw::math::coordinate_2d<double> to_local_rotated_point
    ( double x, double y, double cx, double cy, double a ) const;

    bool coordinate_in_bounding_box(const  wxPoint point) const;
    bool coordinate_in_anchors(const wxPoint& point);
    bool coordinate_in_anchors(const wxPoint& point, const wxRect& box);

    void update_mouse_position( const wxPoint& position );
    const mark* find_mark_at(const wxPoint& point) const;

    void move_mark(const wxPoint& point);
    void move_box(const wxPoint& point);

    wxRect get_drag_mode_size_box() const;
    wxRect get_drag_mode_size_box( double w, double h ) const;
    void apply_drag_mode_size_box();
    void apply_drag_mode_size_mark();

    void send_event_selection_change(const std::string& label);
    void send_event_select_box();
    void send_event_clear_selection();

    void on_paint( wxPaintEvent& event );
    void on_mouse_move( wxMouseEvent& event );
    void on_left_down( wxMouseEvent& event );
    void on_left_up( wxMouseEvent& event );

  private:
    /** \brief The pattern of the background. */
    wxBitmap m_background_pattern;

    /** \brief The model from which an action is displayed. */
    gui_model& m_model;

    /** \brief The player of the current action. */
    action_player m_player;

    /** \brief A delta to the snapshot when rendering. */
    wxPoint m_delta;

    /** \brief The minimum coordinates of anything in the action. */
    claw::math::coordinate_2d<double> m_view_origin;

    /** \brief The size needed to render the action. */
    claw::math::coordinate_2d<double> m_view_size;

    /** \brief A cache of the sprites. */
    sprite_image_cache* m_image_cache;

    /** \brief The zoom ratio. */
    unsigned int m_zoom;

    /** \brief Information about a drag in the window. */
    drag_info m_drag_info;

    /** \brief Tell if the bounding box and the marks are drawn. */
    bool m_wireframe_drawing;

    /** \brief Tell if the sprites of the marks are drawn. */
    bool m_graphic_drawing;

    /** \brief Tell if the texts of the marks are drawn. */
    bool m_text_drawing;

    /** \brief This object manages the placements of the marks between two
        snapshots. */
    model_snapshot_tweener* m_placements;

    /** \brief the radius of the marks when rendering. */
    static const unsigned int s_mark_radius;

    /** \brief The size of grip in the corner of the selected item. */
    static const wxCoord s_grip_size;

    DECLARE_EVENT_TABLE()

  }; // class model_view
} // namespace bf

#endif // __BF_MODEL_VIEW_HPP__
