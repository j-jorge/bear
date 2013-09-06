/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::mouse_over_manager class.
 * \author Sebastie Angibaud
 */
#include "generic_items/mouse_over_manager.hpp"

#include "engine/item_brick/with_rendering_attributes.hpp"
#include "visual/scene_sprite.hpp"
#include "engine/export.hpp"

BASE_ITEM_EXPORT( mouse_over_manager, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::mouse_over_manager::mouse_over_manager()
: m_sample(NULL)
{
  set_global(true);
  set_artificial(true);
  set_phantom(true); 
} // mouse_over_manager::mouse_over_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::mouse_over_manager::~mouse_over_manager()
{
  delete m_sample;
} // mouse_over_manager::mouse_over_manager()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::mouse_over_manager::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  progress_input_reader(elapsed_time);
  m_animation_on.next(elapsed_time);
  m_animation_off.next(elapsed_time);  
} // mouse_over_manager::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::mouse_over_manager::set_item_list_field
( const std::string& name, const std::vector<bear::engine::base_item*> & value)
{
  bool ok = true;

  if (name == "mouse_over_manager.items")
    for ( unsigned int i = 0; i != value.size(); ++i )
      m_items.push_back( handle_pair_type(value[i],false) );
  else
    ok = super::set_item_list_field(name, value);

  return ok;
} // mouse_over_manager::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::mouse_over_manager::set_animation_field
( const std::string& name, const bear::visual::animation& value )
{
  bool result(true);

  if ( name == "mouse_over_manager.animation_on" ) 
    m_animation_on = value;
  else if ( name == "mouse_over_manager.animation_off" ) 
    m_animation_off = value;
  else 
    result = super::set_animation_field(name,value);

  return result;
} // mouse_over_manager::::set_animation_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <sample>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::mouse_over_manager::set_sample_field
( const std::string& name, audio::sample* value )
{
  bool ok = true;

  if (name == "mouse_over_manager.sample")
    m_sample = value;
  else
    ok = super::set_sample_field(name, value);

  return ok;
} // mouse_over_manager::set_sample_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::mouse_over_manager::get_visual
( std::list<bear::engine::scene_visual>& visuals ) const
{
  super::get_visual(visuals); 
  
  handle_list::const_iterator it;
  
  for ( it = m_items.begin(); it != m_items.end(); ++it )
    if ( it->first != (base_item*)NULL )
      {
        double opacity(1);
  
        engine::with_rendering_attributes* b = 
          dynamic_cast<engine::with_rendering_attributes*>(it->first.get());
        if ( b )
          opacity = b->get_rendering_attributes().get_opacity();

        if ( it->second )
          {
            bear::visual::scene_sprite s
              ( it->first->get_horizontal_middle() - 
                m_animation_on.get_sprite().width() / 2, 
                it->first->get_vertical_middle() - 
                m_animation_on.get_sprite().height() / 2, 
                m_animation_on.get_sprite() );
            s.get_rendering_attributes().set_opacity( opacity );
            visuals.push_back( s );
          }
        else
          {
            bear::visual::scene_sprite s
              ( it->first->get_horizontal_middle() - 
                m_animation_off.get_sprite().width() / 2, 
                it->first->get_vertical_middle() - 
                m_animation_off.get_sprite().height() / 2, 
                m_animation_off.get_sprite() );
            s.get_rendering_attributes().set_opacity( opacity );
            visuals.push_back( s );
          }
      }
} // mouse_over_manager::get_visuals()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::mouse_over_manager::get_dependent_items
( universe::physical_item::item_list& d ) const
{
  super::get_dependent_items(d);

  handle_list::const_iterator it;
  for ( it = m_items.begin(); it != m_items.end(); ++it )
    if ( it->first != (base_item*)NULL )
      d.push_back( it->first.get() );
} // mouse_over_manager::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the position of the mouse changes.
 * \param pos The new position of the cursor.
 * \return true if the event has been processed.
 */
bool bear::mouse_over_manager::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  bear::universe::position_type mouse_position = 
    get_level().screen_to_level(pos);
  handle_list::iterator it;
  
  for ( it = m_items.begin(); it != m_items.end(); ++it )
    if ( it->first != (base_item*)NULL )
      {
        if ( it->first->get_bounding_box().includes( mouse_position ) )
          {
            if ( ! it->second )
              play_sound(it->first);
            
            it->second = true;
          }
        else
          it->second = false;
      }
  
  return false;
} // mouse_over_manager::mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sound when the mouse is on the item.
 */
void bear::mouse_over_manager::play_sound( handle_type item )
{
  if ( m_sample != NULL )
    {
      bool play(true);
      
      engine::with_rendering_attributes* a = 
        dynamic_cast<engine::with_rendering_attributes*>( item.get() );  
 
      if ( a != NULL )
        play = a->get_rendering_attributes().get_opacity() != 0; 
  
      if ( play )
        {
          audio::sound_effect effect(m_sample->get_effect());
          effect.set_position( this->get_center_of_mass() );
          m_sample->play(effect);
        }
    }
} // mouse_over_manager::play_sound()
