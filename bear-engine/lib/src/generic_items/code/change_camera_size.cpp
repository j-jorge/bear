/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::change_camera_size class.
 * \author Julien Jorge
 */
#include "generic_items/change_camera_size.hpp"
#include "engine/level.hpp"
#include "generic_items/camera.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( change_camera_size, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::change_camera_size::change_camera_size()
  : m_wanted_size(320, 240)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // change_camera_size::change_camera_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param value The value of the field.
 */
bool bear::change_camera_size::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "change_camera_size.wanted_width" )
    m_wanted_size.x = value;
  else if ( name == "change_camera_size.wanted_height" )
    m_wanted_size.y = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // change_camera_size::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::change_camera_size::progress
( bear::universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( get_level().get_camera_focus().includes(get_center_of_mass()) )
    {
      camera* c = dynamic_cast<camera*>(get_level().get_camera().get());

      if (c != NULL)
        c->set_wanted_size(m_wanted_size);
    }
} // change_camera_size::progress()
