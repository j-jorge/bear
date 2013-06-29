/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A simple camera.
 * \author Julien Jorge
 */
#ifndef __BEAR_CAMERA_HPP__
#define __BEAR_CAMERA_HPP__

#include "engine/base_item.hpp"
#include "engine/export.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A simple camera.
   *
   * The valid fields for this item are
   *  - \a valid_min.x: (real) the minimum valid x-position of the camera
   *    (default = 0),
   *  - \a valid_max.x: (real) the maximum valid x-position of the camera
   *    (default = the width of the level,
   *  - \a valid_min.y: (real) the minimum valid y-position of the camera
   *    (default = 0),
   *  - \a valid_max.y: (real) the maximum valid y-position of the camera
   *    (default = the height of the level,
   *  - \a max_move_length: (real) the maximum movement of the camera
   *    (default = infinity),
   *  - \a size.min_width: (real) the minimum width of the camera (default = 0),
   *  - \a size.min_height: (real) the minimum height of the camera
   *    (default = 0),
   *  - \a size.max_width: (real) the maximum width of the camera
   *    (default = infinity),
   *  - \a size.min_width: (real) the minimum width of the camera
   *    (default = infinity),
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT camera:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(camera);

  public:
    typedef engine::base_item super;

    TEXT_INTERFACE_DECLARE_METHOD_LIST(super, init_exported_methods)

  public:
    camera();

    bool is_valid() const;

    void build();
    void progress( universe::time_type elapsed_time );

    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    void set_valid_area( const universe::rectangle_type& area );
    void set_min_size( const universe::size_box_type& min_size );
    void set_max_size( const universe::size_box_type& max_size );
    void set_max_move_length( universe::coordinate_type max_zoom_length );
    void set_max_zoom_length( universe::coordinate_type max_zoom_length );

    void set_wanted_size( const universe::size_box_type& s );
    void set_shaker_force( double agitation_force );
    const universe::size_box_type& get_default_size() const;

    void set_active_on_build();
    void activate();
    void smooth_activate( universe::time_type d );

    void focus_on( const engine::base_item& item );

  protected:
    void progress_zoom( universe::time_type elapsed_time );

    void adjust_position( const universe::position_type& center_position,
                          universe::time_type elapsed_time );

    void teleport( const universe::position_type& center_position );

  private:
    void adjust_position_x( universe::coordinate_type center_position,
                            universe::coordinate_type max_move );
    void adjust_position_y( universe::coordinate_type center_position,
                            universe::coordinate_type max_move );

    void stay_valid();

    void progress_zoom_with_ratio( universe::time_type elapsed_time );

    static void init_exported_methods();

  private:
    /** \brief The area in which the camera can be set. */
    universe::rectangle_type m_valid_area;

    /** \brief Maximum length of a camera movement. */
    universe::coordinate_type m_max_move_length;

    /** \brief Maximum length of a change of size. */
    universe::coordinate_type m_max_zoom_length;

    /** \brief The default size of the camera. */
    universe::size_box_type m_default_size;

    /** \brief The size to attain. */
    universe::size_box_type m_wanted_size;

    /** \brief The minimum size of the camera. */
    universe::size_box_type m_min_size;

    /** \brief The maximum size of the camera. */
    universe::size_box_type m_max_size;

    /** \brief The force of the shaker. */
    double m_shaker_force;

    /** \brief Indicates if the camera start in active mode/ */
    bool m_active;

  }; // class camera
} // namespace bear

#endif // __BEAR_CAMERA_HPP__
