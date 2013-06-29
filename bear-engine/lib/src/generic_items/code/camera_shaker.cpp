/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::camera_shaker class.
 * \author Sebastien Angibaud
 */
#include "generic_items/camera_shaker.hpp"

#include "generic_items/camera.hpp"
#include "generic_items/delayed_kill_item.hpp"

#include "engine/level.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( camera_shaker, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::camera_shaker::camera_shaker()
: m_shaker_force(10), m_camera_intersection(true)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(false);
} // camera_shaker::camera_shaker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a camera_shaker near an item with a given force and during a
 *        given duration.
 * \param item The item around which the camera is shaked.
 * \param force The strength of the shake.
 * \param duration The duration.
 * \param s The sample played during the shake.
 */
void bear::camera_shaker::shake_around
( const engine::base_item& item, double force, universe::time_type duration,
  audio::sample* s )
{
  camera_shaker* new_camera_shaker = new camera_shaker;
  const universe::time_type fadeout(0.5);

  item.get_layer().add_item( *new_camera_shaker );
  new_camera_shaker->set_center_of_mass(item.get_center_of_mass());
  new_camera_shaker->check_camera_intersection(false);
  new_camera_shaker->set_shaker_force(force);
  new_camera_shaker->set_sample(s);

  if ( duration > fadeout )
    {
      new_camera_shaker->set_fadeout(fadeout);
      new_camera_shaker->set_delay(duration - 0.5);
    }

  new_camera_shaker->toggle_on(NULL);

  delayed_kill_item* killer = new delayed_kill_item;

  killer->add_item(new_camera_shaker);
  killer->set_duration(duration);

  CLAW_ASSERT(killer->is_valid(),
              "The camera shaker killer isn't correctly initialized" );
  item.get_layer().add_item( *killer );
  killer->set_center_of_mass(item.get_center_of_mass());
} // camera_shaker::shake_around()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param value The value of the field.
 */
bool bear::camera_shaker::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "camera_shaker.shaker_force" )
    m_shaker_force = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // camera_shaker::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param value The value of the field.
 */
bool bear::camera_shaker::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "camera_shaker.check_camera_intersection" )
    m_camera_intersection = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // camera_shaker::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the force of the shaker.
 * \param force The value of the force.
 */
void bear::camera_shaker::set_shaker_force( double value )
{
  m_shaker_force = value;
} // camera_shaker::set_shaker_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera_shaker::progress_on( bear::universe::time_type elapsed_time )
{
  super::progress_on(elapsed_time);

  if ( !m_camera_intersection
       || get_level().get_camera_focus().includes(get_center_of_mass()) )
    {
      camera* c = dynamic_cast<camera*>(get_level().get_camera().get());

      if (c != NULL)
        c->set_shaker_force(m_shaker_force);
    }
} // camera_shaker::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the shaker checks camera intersection.
 * \param value The new value.
 */
void bear::camera_shaker::check_camera_intersection(bool value)
{
  m_camera_intersection = value;
} // camera_shaker::check_camera_intersection()
