/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::crossfade_sequence class.
 * \author Sebastie Angibaud
 */
#include "generic_items/crossfade_sequence.hpp"
#include "engine/export.hpp"
#include <claw/logger.hpp>

BASE_ITEM_EXPORT( crossfade_sequence, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
  bear::crossfade_sequence::crossfade_sequence()
    : m_last_sprite(0), m_index(0), m_fadeout_duration(1),
      m_fadein_for_first_sprite(true), m_fadeout_for_last_sprite(true)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // crossfade_sequence::crossfade_sequence()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::crossfade_sequence::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  m_last_sprite += elapsed_time;

  if ( m_index >= m_duration_sequence.size() )
    {
      if ( ( m_last_sprite <= m_fadeout_duration ) &&
           m_fadeout_for_last_sprite && ( m_index > 0) )
        m_items_sequence[m_index-1]->get_rendering_attributes().set_opacity
          (1-m_last_sprite/m_fadeout_duration);
    }
  else
    {
      if ( m_last_sprite <= m_fadeout_duration )
        {
          m_items_sequence[m_index]->get_rendering_attributes().set_opacity
            (m_last_sprite/m_fadeout_duration);
          if ( m_index > 0 )
            m_items_sequence[m_index-1]->get_rendering_attributes().
              set_opacity(1- m_last_sprite/m_fadeout_duration);

          if ( ( m_index == 0 ) && !m_fadein_for_first_sprite )
            m_items_sequence[m_index]->get_rendering_attributes().
              set_opacity(1);
        }
      else
        {
          m_items_sequence[m_index]->get_rendering_attributes().
            set_opacity(1);
          if ( m_index > 0 )
            m_items_sequence[m_index-1]->get_rendering_attributes().
              set_opacity(0);
        }

      if ( m_last_sprite >= m_duration_sequence[m_index] )
        {
          ++m_index;
          m_last_sprite = 0;
        }
    }
} // crossfade_sequence::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::crossfade_sequence::build()
{
  super::build();

  item_list_type::iterator it;

  for ( it = m_items_sequence.begin(); it != m_items_sequence.end(); ++it )
    (*it)->get_rendering_attributes().set_opacity(0);
} // crossfade_sequence::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::crossfade_sequence::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "crossfade_sequence.fadein_for_first_item")
    m_fadein_for_first_sprite = value;
  else if (name == "crossfade_sequence.fadeout_for_last_item")
    m_fadeout_for_last_sprite = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // croosfade_sequence::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::crossfade_sequence::set_real_field
( const std::string& name, double value )
{
  bool ok = true;

  if (name == "crossfade_sequence.fadeout_duration")
    m_fadeout_duration = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // croosfade_sequence::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::crossfade_sequence::set_real_list_field
( const std::string& name, const std::vector<double>& value )
{
  bool ok = true;

  if (name == "crossfade_sequence.duration_sequence")
    m_duration_sequence = value;
  else
    ok = super::set_real_list_field(name, value);

  return ok;
} // croosfade_sequence::set_real_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of <*base_item>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::crossfade_sequence::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool result = false;

  if ( name == "crossfade_sequence.item_sequence" )
    {
      m_items_sequence.clear();

      for ( std::size_t index = 0; index != value.size(); ++index )
        m_items_sequence.push_back(value[index]);

      result = true;
    }
  else
    result = super::set_item_list_field( name, value );

  return result;
} // crossfade_sequence::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::crossfade_sequence::is_valid() const
{
  return super::is_valid() &&
    ( m_duration_sequence.size() == m_items_sequence.size()) &&
    ( !m_duration_sequence.empty());
} // crossfade_sequence::is_valid()
