/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::explosion_effect_item class.
 * \author Sebastie Angibaud
 */
#include "bear/generic_items/explosion_effect_item.hpp"
#include "bear/generic_items/decorative_item.hpp"
#include "bear/engine/export.hpp"

BASE_ITEM_EXPORT( explosion_effect_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::explosion_effect_item::explosion_effect_item()
  : m_force(0), m_dust_count(100), m_splinter_count(10)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // explosion_effect_item::explosion_effect_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::explosion_effect_item::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( !m_dust_animations.empty() )
    create_dust_items();

  if ( !m_splinter_animations.empty() )
    create_splinter_items();

  kill();
} // explosion_effect_item::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::explosion_effect_item::set_animation_list_field
( const std::string& name, const std::vector<visual::animation>& value )
{
  bool result(true);

  if ( name == "explosion_effect_item.dust_animations" )
    m_dust_animations = value;
  else if ( name == "explosion_effect_item.splinter_animations" )
    m_splinter_animations = value;
  else
    result = super::set_animation_list_field(name, value);

  return result;
} // explosion_effect_item::set_animation_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::explosion_effect_item::set_real_field
( const std::string& name, double value )
{
  bool result(true);

  if (name == "explosion_effect_item.force")
    m_force = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // explosion_effect_item::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned int.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::explosion_effect_item::set_u_integer_field
( const std::string& name, unsigned int value )
{
  bool result(true);

  if (name == "explosion_effect_item.dust_count")
    m_dust_count = value;
  else if (name == "explosion_effect_item.splinter_count")
    m_splinter_count = value;
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // explosion_effect_item::set_u_integer_field()

/*---------------------------------------------------------------------------*/
/**
 * \brief Add a splinter animation.
 * \param anim A new splinter animation to add.
 */
void bear::explosion_effect_item::add_splinter_animation
(const visual::animation& anim)
{
  m_splinter_animations.push_back(anim);
} // add_splinter_animation()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set Add a dust animation.
 * \param anim A new dust animation.
 */
void bear::explosion_effect_item::add_dust_animation
(const visual::animation& anim)
{
  m_dust_animations.push_back(anim);
} // explosion_effect_item::add_dust_animation()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the force of ejection.
 * \param force The ejected force.
 */
void bear::explosion_effect_item::set_force(double force)
{
  m_force = force;
} // explosion_effect_item::set_force()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the splinter count.
 * \param splinter_count The number of splinter to create.
 */
void bear::explosion_effect_item::set_splinter_count
( unsigned int splinter_count)
{
  m_splinter_count = splinter_count;
} // explosion_effect_item::set_splinter_count()

/*---------------------------------------------------------------------------*/
/**
 * \brief Set the dust animation.
 * \param dust_count The number of dust to create.
 */
void bear::explosion_effect_item::set_dust_count( unsigned int dust_count)
{
  m_dust_count = dust_count;
} // explosion_effect_item::set_dust_count()

/*---------------------------------------------------------------------------*/
/**
 * \brief Create all dust items.
 */
void bear::explosion_effect_item::create_dust_items( )
{
  unsigned int dust_index(0);

  for ( unsigned int i = 0; i != m_dust_count; i++)
    {
      bear::universe::position_type pos;
      pos.x = get_left() + (get_width() * rand() / RAND_MAX);
      pos.y = get_bottom() + (get_height() * rand() / RAND_MAX);

      create_dust_item(dust_index, pos);
      dust_index = (dust_index + 1) % m_dust_animations.size();
    }
} // explosion_effect_item::create_dust_items()

/*---------------------------------------------------------------------------*/
/**
 * \brief Create a dust item.
 * \param dust_index Index of animation in dust_animations list.
 * \param pos Position of item to create.
 */
void bear::explosion_effect_item::create_dust_item
(unsigned int dust_index, bear::universe::position_type pos)
{
  bear::decorative_item* item = new bear::decorative_item;

  item->set_animation(m_dust_animations[dust_index]);
  item->set_kill_when_finished(true);
  item->set_kill_when_leaving(true);
  item->set_z_position(get_z_position()-1);
  double angle = ( 6.283 * rand() ) / RAND_MAX;
  item->set_system_angle(angle);
  item->set_system_angle_as_visual_angle(true);

  new_item( *item );

  item->set_center_of_mass(pos);

  CLAW_ASSERT
    ( item->is_valid(),
      "A decorative item created by exposion_effect_item isn't correctly "
      "initialized" );
} // explosion_effect_item::create_dust_item()

/*---------------------------------------------------------------------------*/
/**
 * \brief Create all splinter items.
 */
void bear::explosion_effect_item::create_splinter_items( )
{
  unsigned int splinter_index(0);

  for ( unsigned int i = 0; i != m_splinter_count; i++)
    {
      bear::universe::position_type pos;
      pos.x = get_left() + (get_width() * rand() / RAND_MAX);
      pos.y = get_bottom() + (get_height() * rand() / RAND_MAX);

      create_splinter_item(splinter_index, pos);
      splinter_index = (splinter_index + 1) % m_splinter_animations.size();
    }
} // explosion_effect_item::create_splinter_items()

/*---------------------------------------------------------------------------*/
/**
 * \brief Create a splinter item.
 * \param splinter_index Index of animation in splinter_animations list.
 * \param pos Position of item to create.
 */
void bear::explosion_effect_item::create_splinter_item
(unsigned int splinter_index, bear::universe::position_type pos)
{
  bear::decorative_item* item = new bear::decorative_item;

  item->set_animation(m_splinter_animations[splinter_index]);
  item->set_kill_when_leaving(true);
  item->set_kill_when_finished(false);
  item->set_mass(1);
  item->set_z_position(get_z_position());
  item->set_density(0.002);

  universe::force_type force;
  double angle = ( 6.283 * rand() ) / RAND_MAX;
  double factor = (double)rand() / RAND_MAX / 2.0 + 0.5;
  force.x = cos(angle) * m_force * factor;
  force.y = sin(angle) * m_force * factor;
  item->set_external_force(force);

  item->set_system_angle(angle);
  item->set_system_angle_as_visual_angle(true);

  new_item( *item );

  item->set_center_of_mass(pos);

  CLAW_ASSERT
    ( item->is_valid(),
      "A decorative item created by exposion_effect_item isn't correctly "
      "initialized" );

} // explosion_effect_item::create_splinter_item()
