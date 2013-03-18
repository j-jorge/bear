/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::speaker_item class.
 * \author Sébastien Angibaud
 */
#include "engine/comic/item/speaker_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::speaker_item::speaker_item()
  : m_persistent_balloon(false)
{
  // nothing to do
} // speaker_item::speaker_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::speaker_item::speaker_item( const speaker_item& that )
  : m_speeches(that.m_speeches), m_persistent_balloon(that.m_persistent_balloon)
{

} // speaker_item::speaker_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::speaker_item::~speaker_item()
{
  // must exist and be virtual for dynamic_casts
} // speaker_item::~speaker_item()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::speaker_item::progress( universe::time_type elapsed_time )
{
  m_balloon.progress(elapsed_time);

  if ( has_finished_to_speak() && has_more_things_to_say() )
    {
      m_balloon.set_speeches(m_speeches.front());
      m_speeches.pop_front();
    }
} // speaker_item::create_balloon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the speaker has finished to speak.
 */
bool bear::engine::speaker_item::has_finished_to_speak() const
{
  return m_balloon.is_finished();
} // speaker_item::has_finished_to_speak()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the speaker has more things to say.
 */
bool bear::engine::speaker_item::has_more_things_to_say() const
{
  return !m_speeches.empty();
} // speaker_item::has_more_things_to_say()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop speaking.
 */
void bear::engine::speaker_item::stop_speaking()
{
  m_balloon.close();
} // speaker_item::stop_speaking()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start a speech.
 * \param speech The new speech.
 */
void bear::engine::speaker_item::speak(const std::vector<std::string>& speech)
{
  const std::list<std::string> s( speech.begin(), speech.end() );

  m_speeches.push_back(s);
} // speaker_item::speak()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start a speech.
 * \param s The sentence to say.
 *
 * \a s is added at the end of the pending speeches.
 */
void bear::engine::speaker_item::speak(const std::string& s)
{
  std::list<std::string> t;
  t.push_back( s );
  m_speeches.push_back(t);
} // speaker_item::speak()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the balloon.
 */
bear::engine::balloon& bear::engine::speaker_item::get_balloon()
{
  return m_balloon;
} // speaker_item::get_balloon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the persistency of the balloon.
 * \param b Persistent or not.
 */
void bear::engine::speaker_item::set_persistent_balloon( bool b )
{
  m_persistent_balloon = b;
} // speaker_item::set_persistent_balloon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the balloon is persistent.
 */
bool bear::engine::speaker_item::get_persistent_balloon() const
{
  return m_persistent_balloon;
} // speaker_item::get_persistent_balloon()
