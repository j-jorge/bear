/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::hidden_block class.
 * \author Sebastien Angibaud
 */
#include "generic_items/hidden_block.hpp"
#include "generic_items/decorative_effect.hpp"
#include "universe/collision_info.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( hidden_block, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::hidden_block::hidden_block()
  : m_active_state(true), m_new_collision(false), m_transition_duration(0.25),
  m_last_modification(0), m_passive_opacity(0), m_active_opacity(1)
{

} // hidden_block::hidden_block()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elasped time since the last progress.
 */
void bear::hidden_block::progress( universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  if ( !m_new_collision && m_active_state )
    {
      m_last_modification = 0;
      m_active_state = false;
    }
  else
    m_last_modification += elapsed_time;

  if ( m_last_modification <= m_transition_duration )
    {
      if ( m_active_state )
        select_active_opacity();
      else
        select_passive_opacity();
    }

  m_new_collision = false;
} // bear::hidden_block::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::hidden_block::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if ( name == "hidden_block.transition_duration" )
    m_transition_duration = value;
  else if ( name == "hidden_block.opacity.passive" )
    m_passive_opacity = value;
  else if ( name == "hidden_block.opacity.active" )
    m_active_opacity = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // hidden_block::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_check_and_align().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::hidden_block::collision
( engine::base_item& that, universe::collision_info& info )
{
  if ( collision_check_and_align(that, info) )
    {
      if ( !m_active_state )
        m_last_modification = 0;

      m_new_collision = true;
      m_active_state = true;
    }
} // hidden_block::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select active opacity.
 */
void bear::hidden_block::select_active_opacity()
{
  double opacity =
    m_passive_opacity + (m_active_opacity - m_passive_opacity)
    * m_last_modification / m_transition_duration;

  if ( opacity <= 0 )
    opacity = 0;
  else if ( opacity >= 1 )
    opacity = 1;

  get_rendering_attributes().set_opacity(opacity);
} // hidden_block::select_active_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select passive opacity.
 */
void bear::hidden_block::select_passive_opacity()
{
  double opacity =
    m_active_opacity + (m_passive_opacity-m_active_opacity)
    * m_last_modification / m_transition_duration;

  if ( opacity <= 0 )
    opacity = 0;
  else if ( opacity >= 1 )
    opacity = 1;

  get_rendering_attributes().set_opacity(opacity);
} // hidden_block::select_passive_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
void bear::hidden_block::to_string( std::string& str ) const
{
  super::to_string(str);

  if ( m_active_state )
    str += "\nactive" ;
  else
    str += "\npassive" ;
} // hidden_block::to_string()
