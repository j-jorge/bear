/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_with_toggle class.
 * \author Julien Jorge
 */

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::item_with_toggle<Base>::item_with_toggle()
  : m_is_on(false), m_elapsed_time(0),
    m_delay(std::numeric_limits<universe::time_type>::infinity()), m_fadeout(0),
    m_sample(NULL)
{
  this->set_phantom(true);
  this->set_can_move_items(false);
  this->set_artificial(true);
} // item_with_toggle::item_with_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
template<class Base>
bear::engine::item_with_toggle<Base>::item_with_toggle
( const item_with_toggle<Base>& that )
  : super(that), m_is_on(false), m_elapsed_time(0), m_delay(that.m_delay),
    m_fadeout(that.m_fadeout),
    m_sample( that.m_sample == NULL ? NULL : that.m_sample->clone() )
{

} // item_with_toggle::item_with_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::item_with_toggle<Base>::~item_with_toggle()
{
  delete m_sample;
} // item_with_toggle::~item_with_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_toggle<Base>::set_real_field
( const std::string& name, double value )
{
  bool ok = true;

  if (name == "item_with_toggle.delay")
    set_delay(value);
  else if (name == "item_with_toggle.fadeout")
    m_fadeout = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // item_with_toggle::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_toggle<Base>::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "item_with_toggle.initial_state")
    m_is_on = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // item_with_toggle::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <sample>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_toggle<Base>::set_sample_field
( const std::string& name, audio::sample* value )
{
  bool ok = true;

  if (name == "item_with_toggle.sample")
    m_sample = value;
  else
    ok = super::set_sample_field(name, value);

  return ok;
} // item_with_toggle::set_sample_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type item_list.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_toggle<Base>::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool ok = true;

  if (name == "item_with_toggle.linked_toggles")
    for( std::size_t i=0; i!=value.size(); ++i )
      m_linked.push_back( toggle_handle(value[i]) );
  else
    ok = super::set_item_list_field(name, value);

  return ok;
} // item_with_toggle::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::build()
{
  super::build();

  if (is_on())
    {
      play_sound();
      build_on();
    }
  else
    build_off();
} // item_with_toggle::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::progress
( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);
  progress_toggle(elapsed_time);
} // item_with_toggle::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a linked toggle.
 * \param t The toggle.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::add_linked_toggle( with_toggle* t )
{
  m_linked.push_back( toggle_handle::create_from_derived(t) );
} // item_with_toggle::add_linked_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the linked toggles.
 */
template<class Base>
std::vector<bear::engine::with_toggle*>
bear::engine::item_with_toggle<Base>::get_linked_toggles() const
{
  std::vector<with_toggle*> result;
  result.reserve( m_linked.size() );

  for ( handle_list::const_iterator it=m_linked.begin(); it!=m_linked.end();
        ++it )
    result.push_back( it->get() );

  return result;
} // item_with_toggle::get_linked_toggles()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the delay before turning the item off.
 */
template<class Base>
bear::universe::time_type
bear::engine::item_with_toggle<Base>::get_delay() const
{
  return m_delay;
} // item_with_toggle::get_delay()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delay before turning the item off
 * \param d The delay.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::set_delay( universe::time_type d )
{
  m_delay = d;
} // item_with_toggle::set_delay()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sample played when the toggle is on.
 * \param s The sample.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::set_sample( audio::sample* s )
{
  delete m_sample;
  m_sample = s;
} // item_with_toggle::set_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the fade out duration of the sample.
 * \param d The duration.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::set_fadeout( universe::time_type d )
{
  m_fadeout = d;
} // item_with_toggle::set_fadeout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the toggle is turned on.
 */
template<class Base>
bool bear::engine::item_with_toggle<Base>::is_on() const
{
  return m_is_on;
} // item_with_toggle::is_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn the toggle on.
 * \param activator The item that activates the toggle, if any.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::toggle_on( base_item* activator )
{
  if ( !is_on() && !this->is_dead() )
    {
      m_is_on = true;
      m_elapsed_time = 0;

      play_sound();
      on_toggle_on(activator);

      toggle_linked( activator );

      if ( m_delay == 0 )
        toggle_off(activator);
    }
} // item_with_toggle::toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn the toggle off.
 * \param activator The item that activates the toggle, if any.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::toggle_off( base_item* activator )
{
  if ( is_on() && !this->is_dead()  )
    {
      m_is_on = false;
      stop_sound();
      on_toggle_off(activator);

      toggle_linked( activator );
    }
} // item_with_toggle::toggle_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::get_dependent_items
( typename super::item_list& d ) const
{
  handle_list::const_iterator it;

  for (it=m_linked.begin(); it!=m_linked.end(); ++it)
    if ( *it != (with_toggle*)NULL )
      d.push_back(it->get_item());
} // item_with_toggle::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::progress_toggle
( universe::time_type elapsed_time )
{
  if ( is_on() )
    {
      if (m_elapsed_time + elapsed_time >= m_delay)
        {
          const universe::time_type new_duration
            (m_elapsed_time + elapsed_time - m_delay);
          const universe::time_type old_duration(m_delay - m_elapsed_time);

          if ( old_duration > 0 )
            progress_on( old_duration );

          toggle_off(this);

          if ( new_duration > 0 )
            progress_off( new_duration );
        }
      else
        {
          m_elapsed_time += elapsed_time;
          progress_on(elapsed_time);
        }
    }
  else
    progress_off(elapsed_time);
} // item_with_toggle::progress_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item in its activated state.
 *
 * You should overload this method when the initialisation of your item must do
 * different things than a call to on_toggle_on(NULL).
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::build_on()
{
  on_toggle_on(NULL);
} // item_with_toggle::build_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item in its deactivated state.
 *
 * You should overload this method when the initialisation of your item must do
 * different things than a call to on_toggle_off(NULL).
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::build_off()
{
  on_toggle_off(NULL);
} // item_with_toggle::build_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggles switches on.
 * \param activator The item that activates the toggle, if any.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::on_toggle_on( base_item* activator )
{
  // nothing to do
} // item_with_toggle::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggles switches off.
 * \param activator The item that deactivates the toggle, if any.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::on_toggle_off( base_item* activator )
{
  // nothing to do
} // item_with_toggle::on_toggle_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress the item as it is turned on.
 * \param elapsed_time The duration of the activity.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::progress_on
( universe::time_type elapsed_time )
{
  progress_sound();
} // item_with_toggle::progress_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress the item as it is turned off.
 * \param elapsed_time The duration of the activity.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::progress_off
( universe::time_type elapsed_time )
{
  // nothing to do
} // item_with_toggle::progress_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::play_sound() const
{
  if ( m_sample == NULL )
    return;

  audio::sound_effect effect(m_sample->get_effect());

  if (!this->is_global())
    effect.set_position( this->get_center_of_mass() );

  m_sample->play(effect);
} // item_with_toggle::play_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the sample.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::stop_sound() const
{
  if ( m_sample != NULL )
    m_sample->stop(m_fadeout);
} // item_with_toggle::stop_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the position of the sample.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::progress_sound() const
{
  if ( !this->is_global() && (m_sample != NULL) )
    {
      audio::sound_effect effect(m_sample->get_effect());
      effect.set_position( this->get_center_of_mass() );
      m_sample->set_effect(effect);
    }
} // item_with_toggle::progress_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the linked toggles.
 * \param activator The item that changed the state of the toggle, if any.
 */
template<class Base>
void bear::engine::item_with_toggle<Base>::toggle_linked
( base_item* activator )
{
  handle_list::iterator it;
  std::list<handle_list::iterator> dead;

  for (it=m_linked.begin(); it!=m_linked.end(); ++it)
    if ( *it == (with_toggle*)NULL )
      dead.push_back(it);
    else
      (*it)->toggle( m_is_on, activator );

  for ( ; !dead.empty(); dead.pop_front() )
    m_linked.erase(dead.front());
} // item_with_toggle::toggle_linked()
