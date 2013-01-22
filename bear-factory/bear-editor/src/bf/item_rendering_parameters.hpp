/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Rendering parameters of an instance of an item class.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_RENDERING_PARAMETERS_HPP__
#define __BF_ITEM_RENDERING_PARAMETERS_HPP__

#include "bf/animation.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  class item_instance;
  class item_class;

  /**
   * \brief An instance of an item class displayed in the level.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT item_rendering_parameters
  {
  public:
    item_rendering_parameters( item_instance& item );
    item_rendering_parameters( const item_rendering_parameters& that );
    ~item_rendering_parameters();

    void field_changed( const std::string& field_name );
    void reset();
    void init();

    double get_left() const;
    double get_right() const;
    double get_bottom() const;
    double get_top() const;
    double get_horizontal_middle() const;
    double get_vertical_middle() const;

    int get_pos_z() const;
    double get_width() const;
    double get_height() const;
    int get_gap_x() const;
    int get_gap_y() const;

    bool is_mirrored() const;
    bool is_flipped() const;

    bool has_user_defined_width() const;
    bool has_user_defined_height() const;

    bool has_sprite() const;
    const sprite& get_sprite() const;
    sprite& get_sprite();

    void set_position( double x, double y );
    void set_left( double p );
    void set_bottom( double p );
    void set_size( double x, double y );

  private:
    sprite get_sprite_from_item() const;
    sprite get_sprite_from_sprites() const;
    sprite search_sprite_in_class( const item_class& c ) const;
    sprite get_sprite_from_animations() const;
    animation search_animation_in_class( const item_class& c ) const;
    animation load_animation( const std::string& path ) const;

    double get_field_real( const std::string& field_name, double v = 0 ) const;
    color get_field_color( const std::string& field_name, color c ) const;
    int get_field_int( const std::string& field_name, int v = 0 ) const;
    bool get_field_bool( const std::string& field_name, bool v = false ) const;

    void set_field_real( const std::string& field_name, double v );

  public:
    /** \brief Default field name for the left position. */
    static const std::string s_field_name_left;

    /** \brief Default field name for the bottom positino. */
    static const std::string s_field_name_bottom;

    /** \brief Default field name for the width. */
    static const std::string s_field_name_width;

    /** \brief Default field name for the height. */
    static const std::string s_field_name_height;

    /** \brief Default field name for the extend on bounding box option. */
    static const std::string s_field_name_extend_on_bounding_box;

    /** \brief Default field name for the depth. */
    static const std::string s_field_name_depth;

    /** \brief Default field name for the gap on the x-axis. */
    static const std::string s_field_name_gap_x;

    /** \brief Default field name for the gap on the y-axis. */
    static const std::string s_field_name_gap_y;

    /** \brief Default field name for the mirror state. */
    static const std::string s_field_name_mirror;

    /** \brief Default field name for the flipped state. */
    static const std::string s_field_name_flip;

    /** \brief Default field name for the clor. */
    static const std::string s_field_name_color;

    /** \brief Default field name for the angle. */
    static const std::string s_field_name_angle;

  private:
    /** \brief The instance of the item. */
    item_instance& m_item;

    /**
     * \brief The sprite used to display the item.
     *
     * We use a pointer because the pointed value updated in const member
     * functions.
     */
    sprite* m_sprite;

    /** \brief Left position of the item. */
    double m_left;

    /** \brief Bottom position of the item. */
    double m_bottom;

    /** \brief Width of the item. */
    double m_width;

    /** \brief Height position of the item. */
    double m_height;

    /** \brief Indicates if the sprite must be extended on the bounding box. */
    bool m_extend_on_bounding_box;

    /** \brief Z-axis position of the item. */
    int m_pos_z;

    /** \brief Gap of the sprite on the X-axis. */
    int m_gap_x;

    /** \brief Gap of the sprite on the Y-axis. */
    int m_gap_y;

    /** \brief Indicates if the item is mirrorred. */
    bool m_mirror;

    /** \brief Indicates if the item is flipped. */
    bool m_flip;

  }; // class item_rendering_parameters
} // namespace bf

#endif // __BF_ITEM_RENDERING_PARAMETERS_HPP__
