/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the sound_manager class.
 * \author Julien Jorge
 */
#include "audio/sound_manager.hpp"

#include "audio/sdl_sound.hpp"
#include "audio/sample.hpp"

#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <fstream>
#include <vector>

/*----------------------------------------------------------------------------*/
bool bear::audio::sound_manager::s_initialized = false;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::audio::sound_manager::sound_manager()
  : m_ears_position(0, 0), m_current_music(NULL), m_sound_volume(1),
    m_music_volume(1), m_silence_distance(1200), m_full_volume_distance(200),
    m_distance_unit(1)
{

} // sound_manager::sound_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sound_manager::~sound_manager()
{
  clear();
} // sound_manager::~sound_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete all sounds and musics. If a music is playing, she will be
 *        stopped
 * \post m_sounds.empty == true.
 */
void bear::audio::sound_manager::clear()
{
  stop_all();

  std::map<std::string, sound*>::iterator it;

  for (it=m_sounds.begin(); it!=m_sounds.end(); ++it)
    delete it->second;

  m_samples.clear();
  m_sounds.clear();
  m_muted_musics.clear();
} // sound_manager::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a sound to the cache.
 * \param name The name of the sound.
 * \param file A stream containing the sound.
 * \pre name is not used by another sound.
 */
void bear::audio::sound_manager::load_sound
( const std::string& name, std::istream& file )
{
  CLAW_PRECOND( !sound_exists(name) );

  if (s_initialized)
    m_sounds[name] = new sdl_sound(file, name, *this);
  else
    m_sounds[name] = new sound(name, *this);
} // sound_manager::load_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a copy of a sound from another cache into this one.
 * \param name The name of the sound.
 * \param source The cache from which the sound is copied.
 * \pre name is not used by another sound and source.sound_exists(name) is true.
 */
void bear::audio::sound_manager::copy_sound
( const std::string& name, const sound_manager& source )
{
  CLAW_PRECOND( !sound_exists(name) );
  CLAW_PRECOND( source.sound_exists(name) );

  if (s_initialized)
    {
      sound* const ref( source.m_sounds.find(name)->second );
      m_sounds[name] = new sdl_sound( *dynamic_cast<sdl_sound*>(ref), *this);
    }
  else
    m_sounds[name] = new sound(name, *this);
} // sound_manager::copy_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play a sound.
 * \param name The name of the sound to play.
 */
void bear::audio::sound_manager::play_sound( const std::string& name )
{
  CLAW_PRECOND( sound_exists(name) );

  sample* s = m_sounds[name]->new_sample();
  m_samples[s] = true;

  s->play();
} // sound_manager::play_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound, with an effect.
 * \param name The name of the sound to play.
 * \param effect The effect applied to the sound.
 */
void bear::audio::sound_manager::play_sound
( const std::string& name, const sound_effect& effect )
{
  CLAW_PRECOND( sound_exists(name) );

  sample* s = m_sounds[name]->new_sample();
  m_samples[s] = true;

  s->play( effect );
} // sound_manager::play_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sound sample.
 * \param name The name of the sound to get.
 * \pre There is an sound called "name".
 * \remark You are responsible of deleting the sample.
 */
bear::audio::sample*
bear::audio::sound_manager::new_sample( const std::string& name )
{
  CLAW_PRECOND( sound_exists(name) );

  sample* result = m_sounds[name]->new_sample();
  m_samples[result] = false;

  return result;
} // sound_manager::new_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a copy of a sound sample.
 * \param s The sample to copy.
 * \remark You are responsible of deleting the sample.
 */
bear::audio::sample* bear::audio::sound_manager::new_sample( const sample& s )
{
  return new_sample(s.get_sound_name());
} // sound_manager::new_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play a music.
 * \param name The name of the music to play.
 * \param loops How many times the music loops (zero means infinite).
 * \pre There is a sound called "name".
 * \return The identifier of the music.
 */
std::size_t bear::audio::sound_manager::play_music
(const std::string& name, unsigned int loops)
{
  CLAW_PRECOND( sound_exists(name) );

  if (m_current_music != NULL)
    {
      sound_effect e(m_current_music->get_effect());
      m_muted_musics.push_front( muted_music_data(m_current_music, e) );
      e.set_volume(0);
      m_current_music->set_effect(e);
    }

  m_current_music = m_sounds[name]->new_sample();

  // Calling m_current_music->play() may stop immediately if there is no sound
  // card or if the sound is empty. Consequently, m_current_music will be erased
  // and made NULL. So we must store the id now to return something to the
  // caller.
  std::size_t result = m_current_music->get_id();
  m_samples[m_current_music] = true;

  sound_effect e(loops);
  m_current_music->play(e);

  return result;
} // sound_manager::play_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop a music.
 * \param id The identifier of the music.
 * \param fadeout The duration of the fadeout, if any.
 */
void bear::audio::sound_manager::stop_music( std::size_t id, double fadeout )
{
  sample* m(NULL);

  if ( m_current_music != NULL )
    if ( m_current_music->get_id() == id )
      m = m_current_music;

  muted_music_list::const_iterator it;

  for (it=m_muted_musics.begin(); (m==NULL) && (it!=m_muted_musics.end());
       ++it)
    if (it->first->get_id() == id )
      m = it->first;

  if ( m != NULL )
    // calls sample_finished()
    m->stop(fadeout);
} // sound_manager::stop_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop all musics, including muted ones.
 * \param fadeout The duration of the fadeout, if any.
 */
void bear::audio::sound_manager::stop_all_musics( double fadeout )
{
  muted_music_list::iterator it = m_muted_musics.begin();

  while ( it != m_muted_musics.end() )
    {
      muted_music_list::iterator tmp(it);

      // calls sample_finished()
      it->first->stop( fadeout );
      // Reset the iterator since stop indirectly calls
      // remove_muted_music, invalidating the iterator.
      it = m_muted_musics.begin();
    }

  if ( m_current_music != NULL )
    m_current_music->stop( fadeout );
} // sound_manager::stop_all_musics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the ears.
 * \param position The new position.
 */
void bear::audio::sound_manager::set_ears_position
( const claw::math::coordinate_2d<double>& position )
{
  m_ears_position = position;
} // sound_manager::set_ears_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the ears.
 */
const claw::math::coordinate_2d<double>&
bear::audio::sound_manager::get_ears_position() const
{
  return m_ears_position;
} // sound_manager::get_ears_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sound.
 * \param v The new volume.
 * \pre 0 <= v <= 1
 */
void bear::audio::sound_manager::set_sound_volume( double v )
{
  CLAW_PRECOND( v >= 0 );
  CLAW_PRECOND( v <= 1 );

  m_sound_volume = v;

  std::map<sample*, bool>::iterator it;

  for ( it=m_samples.begin(); it!=m_samples.end(); ++it )
    if ( !is_music(it->first) )
      it->first->set_volume(m_sound_volume);
} // sound_manager::set_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the music.
 * \param v The new volume.
 * \pre 0 <= v <= 1
 */
void bear::audio::sound_manager::set_music_volume( double v )
{
  CLAW_PRECOND( v >= 0 );
  CLAW_PRECOND( v <= 1 );

  m_music_volume = v;

  if (m_current_music != NULL)
    m_current_music->set_volume(m_music_volume);
} // sound_manager::set_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the sound.
 */
double bear::audio::sound_manager::get_sound_volume() const
{
  return m_sound_volume;
} // sound_manager::get_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the music.
 */
double bear::audio::sound_manager::get_music_volume() const
{
  return m_music_volume;
} // sound_manager::get_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the adequate volume for a sample.
 * \param s The sample.
 * \return The music volume or the sound volume, according to the type of \a s.
 */
double bear::audio::sound_manager::get_volume( const sample* s ) const
{
  if ( s == m_current_music )
    return get_music_volume();
  else
    return get_sound_volume();
} // sound_manager::get_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a sound is in the cache.
 */
bool bear::audio::sound_manager::sound_exists(const std::string& name) const
{
  return m_sounds.find(name) != m_sounds.end();
} // sound_manager::sound_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the manager that a sample is finished. If the sample is managed
 *        by the manager, it will be deleted.
 */
void bear::audio::sound_manager::sample_finished( sample* s )
{
  std::map<sample*, bool>::iterator it;
  bool do_delete(false);

  it = m_samples.find(s);
  if ( it==m_samples.end() )
    do_delete = it->second;

  if ( do_delete )
    delete s; // will call sample_deleted()

  if ( s == m_current_music )
    {
      if ( m_muted_musics.empty() )
        m_current_music = NULL;
      else
        {
          m_current_music = m_muted_musics.front().first;
          m_current_music->set_effect( m_muted_musics.front().second );
          m_muted_musics.pop_front();
        }
    }
  else
    remove_muted_music(s);
} // sound_manager::sample_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the manager that a sample is deleted. The sample is removed
 *        from the set of samples.
 */
void bear::audio::sound_manager::sample_deleted( sample* s )
{
  m_samples.erase(s);
} // sound_manager::sample_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop all musics and sounds.
 */
void bear::audio::sound_manager::stop_all()
{
  // some samples can be removed from m_samples when calling sample::stop().
  // avoir looping on a map that changes.
  std::map<sample*, bool>::iterator it_s;
  std::vector<sample*> s;

  s.reserve( m_samples.size() );

  for ( it_s=m_samples.begin(); it_s!=m_samples.end(); ++it_s )
    s.push_back(it_s->first);

  for (unsigned int i=0; i!=s.size(); ++i)
    s[i]->stop();

  CLAW_POSTCOND(m_current_music == NULL);
} // sound_manager::stop_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pause all musics and sounds.
 */
void bear::audio::sound_manager::pause_all()
{
  std::map<sample*, bool>::iterator it;

  for ( it=m_samples.begin(); it!=m_samples.end(); ++it )
    it->first->pause();
} // sound_manager::pause_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resume all musics and sounds.
 */
void bear::audio::sound_manager::resume_all()
{
  std::map<sample*, bool>::iterator it;

  for ( it=m_samples.begin(); it!=m_samples.end(); ++it )
    it->first->resume();
} // sound_manager::resume_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the sound system.
 */
void bear::audio::sound_manager::initialize()
{
  s_initialized = sdl_sound::initialize();
} // sound_manager::initialize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close the sound system.
 */
void bear::audio::sound_manager::release()
{
  sdl_sound::release();
  s_initialized = false;
} // sound_manager::release()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a music from m_muted_musics.
 * \param m The music to remove.
 */
void bear::audio::sound_manager::remove_muted_music( sample* m )
{
  muted_music_list::iterator it(m_muted_musics.begin());
  bool found(false);

  while ( !found && (it!=m_muted_musics.end()) )
    if ( it->first == m )
      found = true;
    else
      ++it;

  if (found)
    m_muted_musics.erase(it);
} // sound_manager::remove_muted_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a sample is used as a music.
 */
bool bear::audio::sound_manager::is_music( const sample* m ) const
{
  bool result( m == m_current_music );
  muted_music_list::const_iterator it;

  for ( it=m_muted_musics.begin(); !result && (it!=m_muted_musics.end()); ++it )
    result = (it->first == m);

  return result;
} // sound_manager::is_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the distance under which sounds are at maximum volume.
 * \param d The distance.
 */
void bear::audio::sound_manager::set_full_volume_distance( double d )
{
  m_full_volume_distance = std::max( 0.0, d );
  
  if ( m_full_volume_distance > m_silence_distance )
    m_silence_distance = m_full_volume_distance;
} // sound_manager::set_full_volume_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the distance under which sounds are at maximum volume.
 */
double bear::audio::sound_manager::get_full_volume_distance() const
{
  return m_full_volume_distance;
} // sound_manager::get_full_volume_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the distance from which we can't hear a sound.
 * \param d The distance.
 */
void bear::audio::sound_manager::set_silence_distance( double d )
{
  m_silence_distance = std::max( 0.0, d );

  if ( m_silence_distance < m_full_volume_distance )
    m_full_volume_distance = m_silence_distance;
} // sound_manager::set_silence_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the distance from which we can't hear a sound.
 */
double bear::audio::sound_manager::get_silence_distance() const
{
  return m_silence_distance;
} // sound_manager::get_silence_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the unit of the measures applied in the computation of the
 *        distances for the sound effects.
 * \param d The unit.
 * \remark The function has no effect if d <= 0;
 */
void bear::audio::sound_manager::set_distance_unit( double d )
{
  if ( d > 0 )
    m_distance_unit = d;
} // sound_manager::set_distance_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the unit of the measures applied in the computation of the
 *        distances for the sound effects.
 */
double bear::audio::sound_manager::get_distance_unit() const
{
  return m_distance_unit;
} // sound_manager::get_distance_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the tone down at a given distance of the ears.
 * \param d The distance.
 * \returns A value between zero and one representing the percent of the volume.
 */
double bear::audio::sound_manager::get_volume_for_distance( double d ) const
{
  double result;
  const double distance( d / get_distance_unit() );

  if ( distance <= get_full_volume_distance() )
    result = 1;
  else if ( get_silence_distance() <= distance )
    result = 0;
  else
    result = 1.0 - ( distance - get_full_volume_distance() )
      / ( get_silence_distance() - get_full_volume_distance() );

  return result;
} // sound_manager::get_volume_for_distance()
