/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::spring class.
 * \author Julien Jorge
 */
#include "generic_items/spring.hpp"

#include "universe/collision_info.hpp"
#include "engine/layer/layer.hpp"
#include "engine/world.hpp"

#include "engine/item_brick/loader/activable_sides_loader.hpp"

BASE_ITEM_EXPORT( spring, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::spring::spring()
  : m_applied_force(0, 0)
{

} // spring::spring()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::spring::set_real_field( const std::string& name, double value )
{
  bool result = true;

  if ( name == "spring.applied_force.x" )
    m_applied_force.x = value;
  else if ( name == "spring.applied_force.y" )
    m_applied_force.y = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // spring::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::spring::build()
{
  if ( get_layer().has_world() )
    {
      m_applied_force.x =
        get_layer().get_world().to_world_unit( m_applied_force.x );
      m_applied_force.y =
        get_layer().get_world().to_world_unit( m_applied_force.y );
    }

  get_animation().set_current_index(get_animation().get_max_index());
} // forced_join_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::spring::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::activable_sides_loader(*this) );
} // spring::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the collision is on an active side and bounce the other item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::spring::collision_check_and_bounce
( engine::base_item& that, universe::collision_info& info )
{
  bool bounce(false);
  bool top_contact(false);

  switch( info.get_collision_side() )
    {
    case universe::zone::bottom_zone:
      bounce = bottom_side_is_active();
      break;
    case universe::zone::top_zone:
      bounce = top_side_is_active();
      top_contact = true;
      break;
    case universe::zone::middle_left_zone:
      bounce = left_side_is_active();
      break;
    case universe::zone::middle_right_zone:
      bounce = right_side_is_active();
      break;
    case universe::zone::middle_zone:
      break;
    default: { CLAW_ASSERT( false, "Invalid collision side." ); }
    }

  if ( bounce )
    {
      if ( m_applied_force.x > 0 )
        bounce = collision_align_right( info );
      else if ( m_applied_force.x < 0 )
        bounce = collision_align_left( info );

      if ( m_applied_force.y > 0 )
        bounce = collision_align_top( info );
      else if ( m_applied_force.y < 0 )
        bounce = collision_align_bottom( info );

      if (bounce)
        {
          that.add_external_force(m_applied_force);
          if ( top_contact )
            that.set_bottom_contact(false);
          get_animation().reset();
        }
    }
  else
    default_collision(info);
} // spring::collision_check_and_bounce()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_check_and_align().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::spring::collision
( engine::base_item& that, universe::collision_info& info )
{
  collision_check_and_bounce(that, info);
} // spring::collision()
