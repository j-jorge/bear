/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implmentation of the bear::engine::transition_effect class.
 * \author Julien Jorge
 */
#include "engine/transition_effect/transition_effect.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::transition_effect::transition_effect()
  : m_layer(NULL)
{

} // transition_effect::transition_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::transition_effect::~transition_effect()
{
  // nothing to do
} // transition_effect::~transition_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the layer displaying the effect.
 * \param lay The layer.
 */
void bear::engine::transition_effect::set_layer( gui_layer& lay )
{
  m_layer = &lay;
} // transition_effect::set_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the effect is finished.
 */
bool bear::engine::transition_effect::is_finished() const
{
  return true;
} // transition_effect::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the effect.
 */
void bear::engine::transition_effect::build()
{
  // nothing to do
} // transition_effect::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress the effect.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time on \a elapsed_time at the end of the progress.
 */
bear::universe::time_type
bear::engine::transition_effect::progress( universe::time_type elapsed_time )
{
  return elapsed_time;
} // transition_effect::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the components of the effect.
 * \param e (out) The scene elements.
 */
void bear::engine::transition_effect::render( scene_element_list& e ) const
{
  // nothing to do
} // transition_effect::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level globals of the level in which the layer owning the
 *        effect is.
 */
bear::engine::level_globals&
bear::engine::transition_effect::get_level_globals() const
{
  return get_layer().get_level_globals();
} // transition_effect::get_level_globals()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer in which the effect is.
 */
const bear::engine::gui_layer&
bear::engine::transition_effect::get_layer() const
{
  CLAW_PRECOND( m_layer != NULL );
  return *m_layer;
} // transition_effect::get_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level in which the layer owning the effect is.
 */
const bear::engine::level& bear::engine::transition_effect::get_level() const
{
  CLAW_PRECOND( m_layer != NULL );
  return m_layer->get_level();
} // transition_effect::get_level()
