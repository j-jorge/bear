/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::level_object class.
 * \author Julien Jorge.
 */
#include "engine/level_object.hpp"

#include "engine/layer/layer.hpp"
#include "engine/level.hpp"
#include "engine/level_globals.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::engine::level_object::level_object()
  : m_level(NULL)
{

} // level_object::level_object()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy contructor.
 * \param that The instance to copy from.
 */
bear::engine::level_object::level_object( const level_object& that )
  : m_level(NULL)
{
  // just ignore
} // level_object::level_object()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::level_object::~level_object()
{
  // do nothing
} // level_object::~level_object()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the data related to the level containing the object.
 * \post m_level == NULL
 */
void bear::engine::level_object::clear_level()
{
  m_level = NULL;
} // level_object::clear_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the level in which this object is.
 * \param the_level The level in which we are.
 * \pre m_level == NULL
 */
void bear::engine::level_object::set_level( level& the_level )
{
  CLAW_PRECOND( (m_level == NULL) || (m_level == &the_level) );

  m_level = &the_level;
} // level_object::set_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level in which this item is.
 * \pre m_level != NULL
 */
bear::engine::level& bear::engine::level_object::get_level() const
{
  CLAW_PRECOND( m_level != NULL );

  return *m_level;
} // level_object::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level_globals of the level in which this item is.
 * \pre m_level != NULL
 */
bear::engine::level_globals&
bear::engine::level_object::get_level_globals() const
{
  CLAW_PRECOND( m_level != NULL );

  return m_level->get_globals();
} // level_object::get_level_globals()
