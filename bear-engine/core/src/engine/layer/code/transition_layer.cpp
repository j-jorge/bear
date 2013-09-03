/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::transition_layer class.
 * \author Julien Jorge
 */
#include "engine/layer/transition_layer.hpp"

#include "engine/level.hpp"
#include "engine/level_globals.hpp"
#include "engine/transition_effect/transition_effect.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param e The effect.
 * \param i The identifier given to the effect.
 */
bear::engine::transition_layer::effect_entry::effect_entry
( transition_effect* e, std::size_t i )
  : effect(e), id(i)
{

} // transition_effect::effect_entry::effect_entry()




/*----------------------------------------------------------------------------*/
const std::size_t bear::engine::transition_layer::not_an_id(0);
std::size_t bear::engine::transition_layer::s_next_id(1);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the of the layer in the post office.
 */
bear::engine::transition_layer::transition_layer( const std::string& name )
  : communication::messageable(name)
{

} // transition_layer::transition_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::transition_layer::~transition_layer()
{
  clear();
} // transition_layer::~transition_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the layer.
 */
void bear::engine::transition_layer::build()
{
  get_level_globals().register_item(*this);
} // transition_layer::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the components of the layer.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::transition_layer::progress
( universe::time_type elapsed_time )
{
  effect_map_type::iterator it;

  for ( it=m_effect.begin(); it!=m_effect.end();  )
    if ( (it->second.effect == NULL) || it->second.effect->is_finished() )
      {
        effect_map_type::iterator tmp = it;
        ++it;
        m_effect.erase(tmp);
      }
    else
      {
        it->second.effect->progress( elapsed_time );
        ++it;
      }
} // transition_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the components of the layer.
 * \param e (out) The scene elements.
 */
void bear::engine::transition_layer::render( scene_element_list& e ) const
{
  effect_map_type::const_iterator it;

  for ( it=m_effect.begin(); it!=m_effect.end(); ++it )
    it->second.effect->render( e );
} // transition_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key had been pressed.
 * \param key The value of the pressed key.
 */
bool bear::engine::transition_layer::key_pressed( const input::key_info& key )
{
  return diffuse_call
    ( boost::bind( &transition_effect::key_pressed, _1, key) );
} // transition_layer::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key is maintained.
 * \param key The value of the pressed key.
 */
bool bear::engine::transition_layer::key_maintained
( const input::key_info& key )
{
  return diffuse_call
    ( boost::bind( &transition_effect::key_maintained, _1, key) );
} // transition_layer::key_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key had been released.
 * \param key The value of the pressed key.
 */
bool bear::engine::transition_layer::key_released( const input::key_info& key )
{
  return diffuse_call
    ( boost::bind( &transition_effect::key_released, _1, key) );
} // transition_layer::key_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a character has been entered.
 * \param key The value of the pressed key.
 */
bool bear::engine::transition_layer::char_pressed( const input::key_info& key )
{
  return diffuse_call
    ( boost::bind( &transition_effect::char_pressed, _1, key) );
} // transition_layer::char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button had been pressed.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 */
bool bear::engine::transition_layer::button_pressed
( input::joystick::joy_code button, unsigned int joy_index )
{
  return diffuse_call
    ( boost::bind( &transition_effect::button_pressed, _1, button, joy_index) );
} // transition_layer::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button is maintained.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 */
bool bear::engine::transition_layer::button_maintained
( input::joystick::joy_code button, unsigned int joy_index )
{
  return diffuse_call
    ( boost::bind
      ( &transition_effect::button_maintained, _1, button, joy_index) );
} // transition_layer::button_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button had been released.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 */
bool bear::engine::transition_layer::button_released
( input::joystick::joy_code button, unsigned int joy_index )
{
  return diffuse_call
    ( boost::bind
      ( &transition_effect::button_released, _1, button, joy_index) );
} // transition_layer::button_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been pressed.
 * \param pos The current position of the cursor.
 * \param key The value of the pressed button.
 */
bool bear::engine::transition_layer::mouse_pressed
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return diffuse_call
    ( boost::bind( &transition_effect::mouse_pressed, _1, key, pos) );
} // transition_layer::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button is maintained.
 * \param pos The current position of the cursor.
 * \param key The value of the maintained button.
 */
bool bear::engine::transition_layer::mouse_maintained
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return diffuse_call
    ( boost::bind( &transition_effect::mouse_maintained, _1, key, pos) );
} // transition_layer::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been released.
 * \param pos The current position of the cursor.
 * \param key The value of the released button.
 */
bool bear::engine::transition_layer::mouse_released
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return diffuse_call
    ( boost::bind( &transition_effect::mouse_released, _1, key, pos) );
} // transition_layer::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that the mouse has been moved.
 * \param pos The new position of the mouse.
 */
bool bear::engine::transition_layer::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  return diffuse_call( boost::bind( &transition_effect::mouse_move, _1, pos) );
} // transition_layer::mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process an event triggered by a finger.
 * \param event The event to process.
 */
bool bear::engine::transition_layer::finger_action
( const bear::input::finger_event& event )
{
  return diffuse_call
    ( boost::bind( &transition_effect::finger_action, _1, event) );
} // transition_layer::finger_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Erase an effect.
 * \param id The identifier of the effect to erase.
 */
void bear::engine::transition_layer::erase_effect( std::size_t id )
{
  effect_map_type::iterator it;

  for ( it=m_effect.begin(); it!=m_effect.end(); ++it )
    if ( id == it->second.id )
      {
        delete it->second.effect;
        it->second.effect = NULL;
        it->second.id = not_an_id;
      }
} // transition_layer::erase_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Push an effect in the layer.
 * \param e The effect. It will be deleted by the layer.
 * \param p The prefered depth position of the effect if several effects are
 *        displayed simultaneously.
 * \remark This method call transition_layer::build().
 */
std::size_t
bear::engine::transition_layer::push_effect( transition_effect* e, int p )
{
  const std::size_t id(s_next_id);
  ++s_next_id;

  m_effect.insert( effect_map_type::value_type( p, effect_entry(e, id) ) );

  e->set_layer(*this);
  e->build();

  return id;
} // transition_layer::push_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replace all effects with a new effect.
 * \param e The effect. It will be deleted by the layer.
 * \param p The prefered depth position of the effect if several effects are
 *        displayed simultaneously.
 * \remark This method call transition_layer::build().
 */
std::size_t
bear::engine::transition_layer::set_effect( transition_effect* e, int p )
{
  clear();
  return push_effect(e, p);
} // transition_layer::set_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all effects.
 */
void bear::engine::transition_layer::clear()
{
  for ( ; !m_effect.empty(); m_effect.erase(m_effect.begin()) )
    delete m_effect.begin()->second.effect;
} // transition_layer::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Diffuse a call to the effects.
 */
template<typename F>
bool bear::engine::transition_layer::diffuse_call( F f ) const
{
  bool result(false);

  effect_map_type::const_iterator it;

  for ( it=m_effect.begin(); !result && (it!=m_effect.end()); ++it )
    if ( it->second.effect != NULL )
      result = f(*it->second.effect);

  return result;
} // transition_layer::diffuse_call()
