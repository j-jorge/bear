/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::link_remover class.
 * \author Julien Jorge
 */
#include "generic_items/link/link_remover.hpp"

#include "engine/level_globals.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( link_remover, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::link_remover::set_string_field
( const std::string& name, const std::string& value )
{
  bool ok(true);

  if (name == "link_remover.sound")
    {
      m_sound_name = value;
      get_level_globals().load_sound(m_sound_name);
    }
  else
    ok = super::set_string_field(name, value);

  return ok;
} // link_remover::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the links of a colliding item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::link_remover::collision_remove_links
( engine::base_item& that, universe::collision_info& info )
{
  that.remove_all_links();

  if (!m_sound_name.empty())
    get_level_globals().play_sound(m_sound_name);
} // link_remover::collision_remove_links()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_remove_links().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::link_remover::collision
( engine::base_item& that, universe::collision_info& info )
{
  collision_remove_links(that, info);
} // link_remover::collision()
