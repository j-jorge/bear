/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::camera class.
 * \author Julien Jorge
 */
#include "generic_items/camera.hpp"

#include "engine/level.hpp"
#include "universe/forced_movement/forced_goto.hpp"

#include "engine/export.hpp"

#include <limits>

BASE_ITEM_EXPORT( camera, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::camera::camera()
: m_valid_area
(0, 0,
 std::numeric_limits<bear::universe::coordinate_type>::infinity(),
 std::numeric_limits<bear::universe::coordinate_type>::infinity()),
  m_max_move_length
  ( std::numeric_limits<bear::universe::coordinate_type>::infinity() ),
  m_max_zoom_length
  ( std::numeric_limits<bear::universe::coordinate_type>::infinity() ),
  m_min_size(0, 0),
  m_max_size
  ( std::numeric_limits<bear::universe::coordinate_type>::infinity(),
    std::numeric_limits<bear::universe::coordinate_type>::infinity() ),
  m_shaker_force(0), m_active(false)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // camera::camera()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the camera is well initialized.
 */
bool bear::camera::is_valid() const
{
  return (m_max_size.x >= m_min_size.x) && (m_max_size.y >= m_min_size.y)
    && super::is_valid();
} // camera::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::camera::build()
{
  super::build();

  universe::coordinate_type left = m_valid_area.left();
  universe::coordinate_type right = m_valid_area.right();
  universe::coordinate_type top = m_valid_area.top();
  universe::coordinate_type bottom = m_valid_area.bottom();

  if (left < 0)
    left = 0;
  else if (left > get_level().get_size().x)
    left = get_level().get_size().x;

  if (bottom < 0)
    bottom = 0;
  else if (bottom > get_level().get_size().y)
    bottom = get_level().get_size().y;

  if (right > get_level().get_size().x)
    right = get_level().get_size().x;

  if (top > get_level().get_size().y)
    top = get_level().get_size().y;

  m_valid_area.set(left, bottom, right, top);

  m_default_size = m_wanted_size = get_size();

  if ( m_active )
    activate();
} // camera::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera::progress( bear::universe::time_type elapsed_time )
{
  progress_zoom(elapsed_time);

  if ( m_shaker_force != 0 )
    adjust_position(get_center_of_mass(), elapsed_time);
} // camera::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param value The value of the field.
 */
bool bear::camera::set_real_field( const std::string& name, double value )
{
  bool result = true;

  if ( name == "camera.valid_min.x" )
    m_valid_area.first_point.x = value;
  else if ( name == "camera.valid_min.y" )
    m_valid_area.first_point.y = value;
  else if ( name == "camera.valid_max.x" )
    m_valid_area.second_point.x = value;
  else if ( name == "camera.valid_max.y" )
    m_valid_area.second_point.y = value;
  else if ( name == "camera.max_move_length" )
    m_max_move_length = value;
  else if ( name == "camera.max_zoom_length" )
    m_max_zoom_length = value;
  else if ( name == "camera.size.max_width" )
    m_max_size.x = value;
  else if ( name == "camera.size.max_height" )
    m_max_size.y = value;
  else if ( name == "camera.size.min_width" )
    m_min_size.x = value;
  else if ( name == "camera.size.min_height" )
    m_min_size.y = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // camera::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param value The value of the field.
 */
bool bear::camera::set_bool_field( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "camera.active" )
    m_active = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // camera::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the valid area.
 * \param rect The valid area.
 */
void bear::camera::set_valid_area( const universe::rectangle_type& rect )
{
  m_valid_area = rect;
} // camera::set_valid_area()

/*----------------------------------------------------------------------------*/
/** \brief Set the minimum size of the camera. 
 * \param min_size The minimum size.
 */
void bear::camera::set_min_size( const universe::size_box_type& min_size )
{
  m_min_size = min_size;
} // camera::set_min_size()

/*----------------------------------------------------------------------------*/
/** \brief Set the maximum size of the camera. 
 * \param max_size The maximum size.
 */
void bear::camera::set_max_size( const universe::size_box_type& max_size )
{
  m_max_size = max_size;
} // camera::set_max_size()

/*----------------------------------------------------------------------------*/
/** \brief Set the maximum move length of the camera. 
 * \param max_move_length The maximum move length.
 */
void bear::camera::set_max_move_length
( universe::coordinate_type max_move_length )
{
  m_max_move_length = max_move_length;
} // camera::set_max_move_length()

/*----------------------------------------------------------------------------*/
/** \brief Set the maximum zoom length of the camera. 
 * \param max_zoom_length The maximum zoom length.
 */
void bear::camera::set_max_zoom_length
( universe::coordinate_type max_zoom_length )
{
  m_max_zoom_length = max_zoom_length;
} // camera::set_max_zoom_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progressively change the size of the camera.
 * \param s The size to attain.
 */
void bear::camera::set_wanted_size( const universe::size_box_type& s )
{
  m_wanted_size.x = std::min( std::max( s.x, m_min_size.x ), m_max_size.x );
  m_wanted_size.y = std::min( std::max( s.y, m_min_size.y ), m_max_size.y );

  const universe::coordinate_type w
    ( m_valid_area.second_point.x - m_valid_area.first_point.x );
  const universe::coordinate_type h
    ( m_valid_area.second_point.y - m_valid_area.first_point.y );

  if ( m_wanted_size.x > w )
    set_wanted_size
      ( universe::size_box_type(w, w/(m_wanted_size.x / m_wanted_size.y)) );

  if (  m_wanted_size.y > h )
    set_wanted_size
      ( universe::size_box_type(h*(m_wanted_size.x / m_wanted_size.y), h) );
} // camera::set_wanted_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the force of the shaker of the camera.
 * \param shaker_force The new shaker force.
 */
void bear::camera::set_shaker_force( double shaker_force )
{
  m_shaker_force = shaker_force;
} // camera::set_shaker_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Det the default size of the camera.
 */
const bear::universe::size_box_type& bear::camera::get_default_size() const
{
  return m_default_size;
} // camera::get_default_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the camera must be activated by default when built.
 */
void bear::camera::set_active_on_build()
{
  m_active = true;
} // camera::set_active_on_build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Activate the camera.
 */
void bear::camera::activate()
{
  get_level().set_camera( *this );
} // camera::activate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Activate the camera with a smooth movement from the previous camera's
 *        position.
 * \param d The duration of the movement.
 *
 * This method applies a forced movement to the camera, during \d seconds. You
 * may want not to use this method if the camera already has a forced movement.
 */
void bear::camera::smooth_activate( universe::time_type d )
{
  const universe::position_type target_pos( get_center_of_mass() );

  set_center_of_mass( get_level().get_camera_center() );
  stay_valid();

  universe::forced_goto mvt(d);
  mvt.set_auto_remove(true);
  mvt.set_acceleration_time(d/2);
  mvt.set_length( target_pos - get_center_of_mass() );

  set_forced_movement(mvt);
  get_level().set_camera( *this );
} // camera::smooth_activate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the center of mass of the camera on the center of mass of an item.
 * \param item The item to focus on.
 */
void bear::camera::focus_on( const engine::base_item& item )
{
  set_center_of_mass( item.get_center_of_mass() );
  stay_valid();
} // camera::focus_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress toward the wanted size.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera::progress_zoom( universe::time_type elapsed_time )
{
  if ( get_size() != m_wanted_size )
    {
      const universe::position_type c = get_center_of_mass();

      progress_zoom_with_ratio(elapsed_time);

      set_center_of_mass(c);
    }

  m_wanted_size = m_default_size;
} // camera::progress_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the position of the center of the camera.
 * \param center_position New position for the center of the camera.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera::adjust_position
( const bear::universe::position_type& center_position,
  bear::universe::time_type elapsed_time )
{
  double a_x = (m_shaker_force * rand() / RAND_MAX) - m_shaker_force/2;
  double a_y = (m_shaker_force * rand() / RAND_MAX) - m_shaker_force/2;

  adjust_position_x(a_x + center_position.x, elapsed_time * m_max_move_length);
  adjust_position_y(a_y + center_position.y, elapsed_time * m_max_move_length);

  m_shaker_force = 0;
} // camera::adjust_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a new position for the center of the camera, but keep it in the
 *        valid area.
 * \param center_position New position for the center of the camera.
 */
void bear::camera::teleport
( const bear::universe::position_type& center_position )
{
  set_center_of_mass(center_position);
  stay_valid();
} // camera::teleport()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the X position of the center of the camera.
 * \param center_position X-coordinate to set to the center of the camera.
 * \param max_move The longest feasible movement.
 */
void bear::camera::adjust_position_x
( bear::universe::coordinate_type center_position,
  bear::universe::coordinate_type max_move )
{
  const bear::universe::coordinate_type current_x = get_center_of_mass().x;
  bear::universe::coordinate_type distance;

  if ( center_position < current_x )
    {
      distance = std::min(current_x - center_position, max_move);

      if ( get_left() - m_valid_area.left() >= distance )
        set_left( get_left() - distance );
      else
        set_left( m_valid_area.left() );
    }
  else if ( center_position > current_x )
    {
      distance = std::min(center_position - current_x, max_move);

      if ( get_right() + distance <= m_valid_area.right() )
        set_left( get_left() + distance );
      else
        set_right( m_valid_area.right() );
    }
} // camera::adjust_position_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the Y position of the center of the camera.
 * \param center_position Y-coordinate to set to the center of the camera.
 * \param max_move The longest feasible movement.
 */
void bear::camera::adjust_position_y
( bear::universe::coordinate_type center_position,
  bear::universe::coordinate_type max_move )
{
  const bear::universe::coordinate_type current_y = get_center_of_mass().y;
  bear::universe::coordinate_type distance;

  if ( center_position < current_y )
    {
      distance = std::min(current_y - center_position, max_move);

      if ( get_bottom() - distance >= m_valid_area.bottom() )
        set_bottom( get_bottom() - distance );
      else
        set_bottom( m_valid_area.bottom() );
    }
  else if ( center_position > current_y )
    {
      distance = std::min(center_position - current_y, max_move);

      if ( get_top() + distance <= m_valid_area.top() )
        set_bottom( get_bottom() + distance );
      else
        set_top( m_valid_area.top() );
    }
} // camera::adjust_position_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the camera is in its valid area and adjust its position if
 *        needed.
 */
void bear::camera::stay_valid()
{
  if ( get_left() < m_valid_area.left() )
    set_left( m_valid_area.left() );

  if ( get_bottom() < m_valid_area.bottom() )
    set_bottom( m_valid_area.bottom() );

  if ( get_right() > m_valid_area.right() )
    {
      if ( m_valid_area.right() > get_width() )
        set_right( m_valid_area.right() );
      else
        set_left(0);
    }

  if ( get_top() > m_valid_area.top() )
    {
      if ( m_valid_area.top() > get_height() )
        set_top( m_valid_area.top() );
      else
        set_bottom(0);
    }
} // camera::stay_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress toward the wanted size and keep the ratio width/height.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera::progress_zoom_with_ratio( universe::time_type elapsed_time )
{
  const universe::coordinate_type r = m_wanted_size.x / m_wanted_size.y;

  const universe::coordinate_type d_x =
    std::min( std::abs(m_wanted_size.x - get_width()),
              m_max_zoom_length * elapsed_time );
  const universe::coordinate_type d_y =
    std::min( std::abs(m_wanted_size.y - get_height()),
              m_max_zoom_length * elapsed_time );

  if ( d_x > d_y )
    {
      if ( m_wanted_size.x > get_width() )
        set_width( get_width() + d_x );
      else
        set_width( get_width() - d_x );

      set_height( get_width() / r );
    }
  else
    {
      if ( m_wanted_size.y > get_height() )
        set_height( get_height() + d_y );
      else
        set_height( get_height() - d_y );

      set_width( get_height() * r );
    }
} // camera::progress_zoom_with_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
void bear::camera::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_METHOD_0( bear::camera, activate, void);
  TEXT_INTERFACE_CONNECT_METHOD_1
    ( bear::camera, smooth_activate, void, bear::universe::time_type);
  TEXT_INTERFACE_CONNECT_METHOD_1
    ( bear::camera, focus_on, void, const bear::engine::base_item&);
} // camera::init_exported_methods()

/*----------------------------------------------------------------------------*/
TEXT_INTERFACE_IMPLEMENT_METHOD_LIST( bear::camera )
