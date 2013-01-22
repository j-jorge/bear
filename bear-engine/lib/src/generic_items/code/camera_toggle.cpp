/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::camera_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/camera_toggle.hpp"

#include "engine/level.hpp"
#include "generic_items/camera.hpp"

BASE_ITEM_EXPORT( camera_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::camera_toggle::camera_toggle()
  : m_starting_smooth_delay(0), m_ending_smooth_delay(0), m_camera(NULL)
{
  set_can_move_items(false);
  set_phantom(true);
  set_artificial(true);
} // camera_toggle::camera_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <item>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::camera_toggle::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool ok = true;

  if (name == "camera_toggle.camera")
    m_camera = value;
  else
    ok = super::set_item_field(name, value);

  return ok;
} // camera_toggle::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <real>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::camera_toggle::set_real_field
( const std::string& name, double value )
{
  bool ok = true;

  if (name == "camera_toggle.starting_transition_duration")
    m_starting_smooth_delay = value;
  else  if (name == "camera_toggle.ending_transition_duration")
    m_ending_smooth_delay = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // camera_toggle::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialized.
 */
bool bear::camera_toggle::is_valid() const
{
  // We allow the camera to be NULL only if the toggle has been toggled on, in
  // which case he can have taken the NULL camera of the level.
  return ((m_camera != (camera*)NULL) || is_on() )
    && (m_starting_smooth_delay >= 0)
    && (m_ending_smooth_delay >= 0) && super::is_valid();
} // camera_toggle::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item in its off state.
 */
void bear::camera_toggle::build_off()
{
  // nothing to do
} // camera_toggle::build_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Activate the camera.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::camera_toggle::on_toggle_on( engine::base_item* activator )
{
  switch_camera();
} // camera_toggle::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the old camera.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::camera_toggle::on_toggle_off( engine::base_item* activator )
{
  switch_camera();
} // camera_toggle::on_toggle_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Switch the camera of the level.
 */
void bear::camera_toggle::switch_camera()
{
  handle_type old_cam = get_level().get_camera();

  if ( m_camera != (camera*)NULL )
    {
      if ( is_on() )
        {
          if (m_starting_smooth_delay == 0)
            m_camera->activate();
          else
            m_camera->smooth_activate(m_starting_smooth_delay);
        }
      else
        {
          if (m_ending_smooth_delay == 0)
            m_camera->activate();
          else
            m_camera->smooth_activate(m_ending_smooth_delay);
        }
    }

  m_camera = old_cam;
} // camera_toggle::switch_camera()
