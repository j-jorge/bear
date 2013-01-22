/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::audio::sound class.
 * \author Julien Jorge
 */
#include "audio/sound.hpp"
#include "audio/sound_manager.hpp"
#include "audio/sample.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the sound resource.
 * \param owner The instance of sound_manager who stores me.
 */
bear::audio::sound::sound( const std::string& name, sound_manager& owner )
  : m_owner(owner), m_name(name)
{

} // sound::sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sound::~sound()
{

} // sound::~sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new sample of this sound.
 */
bear::audio::sample* bear::audio::sound::new_sample()
{
  return new sample( get_sound_name(), get_manager() );
} // sound::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound_manager who owns this sound.
 */
bear::audio::sound_manager& bear::audio::sound::get_manager()
{
  return m_owner;
} // sound::get_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound_manager who owns this sound.
 */
const bear::audio::sound_manager& bear::audio::sound::get_manager() const
{
  return m_owner;
} // sound::get_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the sound resource.
 */
const std::string& bear::audio::sound::get_sound_name() const
{
  return m_name;
} // sound::get_sound_name()
