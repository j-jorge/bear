/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This control displays the items of the level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_RENDERER_HPP__
#define __BF_LEVEL_RENDERER_HPP__

#include "bf/grid.hpp"
#include "bf/level_check_result.hpp"
#include "bf/sprite_with_position.hpp"

#include <wx/dcgraph.h>

#include <list>
#include <map>

namespace bf
{
  class drag_info;
  class gui_level;
  class item_instance;
  class layer;
  class sprite_image_cache;

  /**
   * \brief This control display the items of the level.
   * \author Julien Jorge
   */
  class level_renderer
  {
  public:
    explicit level_renderer( const gui_level& lvl );
    ~level_renderer();

    wxCoord get_grip_size() const;

    wxPoint get_position_in_layer
    ( wxPoint position, unsigned int index, wxCoord height ) const;

    wxPoint compute_local_view_position() const;
    wxPoint compute_local_view_position( unsigned int index ) const;

    wxPoint compute_global_view_position( wxPoint view_position ) const;
    wxPoint compute_global_view_position
    ( unsigned int index, wxPoint view_position ) const;

    wxSize get_view_size() const;

    unsigned int get_zoom() const;
    void set_zoom(unsigned int z);

    void show_grid( bool v );
    bool get_grid_visibility() const;

    void set_bright_background( bool b );
    bool get_bright_background() const;

    void set_id_visibility( bool v );
    bool get_id_visibility() const;

    void toggle_relationship_drawing();
    bool get_relationship_drawing() const;

    void toggle_graphic_drawing();
    bool get_graphic_drawing() const;

    void toggle_wireframe_drawing();
    bool get_wireframe_drawing() const;

    const grid& get_grid() const;
    void set_grid( const grid& g );

    void get_structure_sprites( std::list<sprite_with_position>& s ) const;
    void render
    ( wxGCDC& dc, wxRect part, level_check_result check_result,
      drag_info const* d );

    template<typename T>
    T zoom( T v ) const
    {
      return v * T(m_zoom) / T(100);
    } // zoom()

    wxPoint zoom( wxPoint v ) const;
    wxSize zoom( wxSize v ) const;
    wxCoord unzoom( wxCoord v ) const;
    wxPoint unzoom( wxPoint v ) const;
    wxSize unzoom( wxSize v ) const;

  private:
    // not implemented
    level_renderer( const level_renderer& that );

    const layer& get_active_layer() const;
    const gui_level& get_level() const;

    void render_layers( wxGCDC& dc ) const;

    void render_items
    ( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order,
      unsigned int i ) const;

    void render_relationship
    ( wxDC& dc, const item_instance& item,
      const std::multimap<int, item_instance*>& z_order,
      unsigned int index) const;
    void render_relationship
    ( wxDC& dc, const item_instance& ref, const item_instance& rel,
      unsigned int index) const;

    void render_item
    ( wxGCDC& dc, const item_instance& item, const wxPoint& pos,
      unsigned int index,
      const std::multimap<int, item_instance*>& z_order ) const;

    void
    render_item_sprite
    ( wxDC& dc, const item_instance& item,
      const std::multimap<int, item_instance*>& z_order,
      unsigned int index) const;
    void render_item_filled
    ( wxGCDC& gc, const item_instance& item,
      unsigned int index ) const;
    void render_item_wireframe
    ( wxGCDC& dc, const item_instance& item, unsigned int index ) const;
    void render_item_id
    ( wxDC& dc, const item_instance& item, unsigned int layer ) const;

    void render_item_as_sprite
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      const std::multimap<int, item_instance*>& z_order) const;
    void render_item_as_wireframe
    ( wxGCDC& dc, const item_instance& item, const wxPoint& pos,
      unsigned int index ) const;
    void render_item_id
    ( wxDC& dc, const item_instance& item, const wxPoint& pos ) const;

    void render_item_as_point
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      unsigned int index ) const;
    bool render_sprite
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      const std::multimap<int, item_instance*>& z_order ) const;
    void render_item_bounding_box
    ( wxDC& dc, const item_instance& item, const wxPoint& pos,
      const wxSize& size, unsigned int index ) const;
    void render_slope_steepness
    ( wxGCDC& dc, const item_instance& item, 
      const wxPoint& pos, const wxSize& size, unsigned int index ) const;
    void render_slope_curve_grip
    ( wxDC& dc, const item_instance& item, 
      const wxPoint& pos, const wxSize& size, unsigned int index ) const;
    void render_non_valid_item
    ( wxDC& dc, const item_instance& item, unsigned int index ) const;
    void render_non_valid_item_as_point
    ( wxDC& dc, const item_instance& item, const wxPoint& pos ) const;
    void render_non_valid_item_box
    ( wxDC& dc, const item_instance& item,
      const wxPoint& pos, const wxSize& size ) const;

    void render_grip( wxDC& dc, unsigned int index ) const;
    void render_grip_box( wxDC& dc, wxCoord left, wxCoord top ) const;

    void render_drag
    ( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order, 
      unsigned int index ) const;
    void render_drag_mode_selection( wxDC& dc, unsigned int index ) const;
    void render_drag_mode_move
    ( wxGCDC& dc, const std::multimap<int, item_instance*>& z_order, 
      unsigned int index ) const;
    void render_drag_mode_size( wxDC& dc, unsigned int index ) const;
    void render_drag_mode_slope( wxGCDC& dc, unsigned int index ) const;
    void render_grid( wxDC& dc ) const;
    void render_grid_vertical_lines( wxDC& dc ) const;
    void render_grid_horizontal_lines( wxDC& dc ) const;

    std::pair<wxBitmap, wxPoint>
    get_item_visual( const item_instance& item ) const;
    std::pair<wxBitmap, wxPoint> 
    get_item_visual
    ( const item_instance& item, 
      const bitmap_rendering_attributes& rendering_attributes ) const;
    bool has_visual
    ( const item_instance& item,
      const std::multimap<int, item_instance*>& z_order ) const;
    std::pair<wxBitmap, wxPoint>
    get_item_or_mimic_visual
    ( const item_instance& item,
      const std::multimap<int, item_instance*>& z_order ) const;
    wxPen
    get_display_pen( const item_instance& item, unsigned int index ) const;

    wxRect get_presence_box( const item_instance& item ) const;

  private:
    /** \brief The level to render. */
    const gui_level& m_level;

    /** \brief Position and the size of the view in the (zoomed) level. */
    wxRect m_view;

    /** \brief Info about the current dragging. */
    drag_info const * m_drag_info;

    /** \brief The grid. */
    grid m_grid;

    /** \brief Tell if we draw the bounding box of the items. */
    bool m_wireframe_drawing;

    /** \brief Tell if we display the pictures of the items. */
    bool m_graphic_drawing;

    /** \brief Tell if we display the grid. */
    bool m_display_grid;

    /** \brief Tell if we display the id. */
    bool m_display_id;

    /** \brief Tell if we display the relationship among the items. */
    bool m_display_relationship;

    /** \brief Tell if we use a bright background. */
    bool m_bright_background;

    /** \brief A cache of the sprites of the level. */
    sprite_image_cache* m_image_cache;

    /** \brief The current level of zoom. */
    unsigned int m_zoom;

    /** \brief The result of the last check on the level. */
    level_check_result m_check_result;

    /** \brief The size of grip in the corner of the selected item. */
    static const wxCoord s_grip_size;

  }; // class level_renderer
} // namespace bf

#endif // __BF_LEVEL_RENDERER_HPP__
