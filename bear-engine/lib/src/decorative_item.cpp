/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::decorative_item class.
 * \author Sebastie Angibaud
 */
#include "bear/generic_items/decorative_item.hpp"

#include "bear/visual/scene_element_sequence.hpp"

#include "bear/engine/export.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The instance loaded by this loaded.
 */
bear::decorative_item::loader::loader( decorative_item& item )
  : super("decorative_item"), m_item(item)
{

} // decorative_item::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::decorative_item::loader* bear::decorative_item::loader::clone() const
{
  return new loader( *this );
} // decorative_item::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::decorative_item::loader::set_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "shadow.x" )
    m_item.set_shadow_x( value );
  else if ( name == "shadow.y" )
    m_item.set_shadow_y( value );
  else
    result = super::set_field(name, value);

  return result;
} // decorative_item::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c boolean.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::decorative_item::loader::set_field
( const std::string& name, bool value )
{
  bool result = true;

  if (name == "kill_when_finished")
    m_item.set_kill_when_finished( value );
  else if (name == "kill_on_contact")
    m_item.set_kill_on_contact( value );
  else if (name == "kill_when_leaving")
    m_item.set_kill_when_leaving( value );
  else
    result = super::set_field(name, value);

  return result;
} // decorative_item::loader::set_field()





BASE_ITEM_EXPORT( decorative_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::decorative_item::decorative_item()
  : m_kill_when_finished(false),  m_kill_on_contact(false),
  m_stop_on_bottom_contact(false), m_kill_when_leaving(false),
  m_shadow_x(0), m_shadow_y(0)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // decorative_item::decorative_item()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::decorative_item::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( m_kill_on_contact && has_contact() )
    kill();
  else if ( m_kill_when_finished && get_animation().is_finished() )
    kill();
} // decorative_item::progress()

/*---------------------------------------------------------------------------*/
/**
 * \brief Gets the scene elements to use to render this item.
 * \param visuals (out) The scene elements.
 */
void bear::decorative_item::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  if ( (m_shadow_x == 0) || (m_shadow_y == 0) )
    super::get_visual( visuals );
  else
    {
      typedef std::list<engine::scene_visual> visual_list;

      visual_list parent_visuals;

      super::get_visual( parent_visuals );
      parent_visuals.sort( engine::scene_visual::z_position_compare() );

      visual::scene_element_sequence result;
      result.set_shadow( m_shadow_x, m_shadow_y );

      for ( visual_list::iterator it=parent_visuals.begin();
            it!=parent_visuals.end(); ++it )
        result.push_back( it->scene_element );

      visuals.push_back( engine::scene_visual( result, get_z_position() ) );
    }
} // decorative_item::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if the item kills himself when the animation is finished.
 * \param value The new value.
 */
void bear::decorative_item::set_kill_when_finished(bool value)
{
  m_kill_when_finished = value;
} // decorative_item::set_kill_when_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if the item kills himself when he has a contact.
 * \param value The new value.
 */
void bear::decorative_item::set_kill_on_contact(bool value)
{
  m_kill_on_contact = value;
} // decorative_item::set_kill_on_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the item kills himself when he leaves the active region.
 */
bool bear::decorative_item::get_kill_when_leaving() const
{
  return m_kill_when_leaving;
} // decorative_item::get_kill_when_leaving()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if the item kills himself when he leaves the active region.
 * \param value The new value.
 */
void bear::decorative_item::set_kill_when_leaving(bool value)
{
  m_kill_when_leaving = value;
} // decorative_item::set_kill_when_leaving()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the offset of the shadow on the x-axis.
 * \param v The new offset.
 */
void bear::decorative_item::set_shadow_x( double v )
{
  m_shadow_x = v;
} // decorative_item::set_shadow_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the offset of the shadow on the y-axis.
 * \param v The new offset.
 */
void bear::decorative_item::set_shadow_y( double v )
{
  m_shadow_y = v;
} // decorative_item::set_shadow_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::decorative_item::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader( *this ) );
} // decorative_item::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief The item leaves the active region.
 */
void bear::decorative_item::leaves_active_region()
{
  super::leaves_active_region();

  if (m_kill_when_leaving)
    kill();
} // decorative_item::leaves_active_region()
