/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::balloon_layer class.
 * \author Sebastien Angibaud
 */
#include "engine/comic/layer/balloon_layer.hpp"

#include "engine/comic/balloon.hpp"
#include "engine/comic/item/speaker_item.hpp"
#include "engine/comic/layer/balloon_placement/balloon_placement.hpp"

#include "engine/level.hpp"
#include "engine/level_globals.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the layer.
 */
bear::engine::balloon_layer::balloon_layer( const std::string& name )
  : communication::messageable(name), m_border(20)
{

} // balloon_layer::balloon_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the layer.
 */
void bear::engine::balloon_layer::build()
{
  get_level_globals().register_item(*this);
} // balloon_layer::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the postion of the balloons.
 * \param elapsed_time The time elapsed since the last call.
 */
void bear::engine::balloon_layer::progress( universe::time_type elapsed_time )
{
  if ( get_level().is_paused() )
    return;

  speaker_list::iterator it;
  balloon_placement placement(get_size().x, get_size().y);

  for ( it = m_speakers.begin(); it != m_speakers.end(); )
    if ( (*it) == (speaker_item*)(NULL) )
      it = m_speakers.erase(it);
    else
      {
        placement.add_speaker( **it, get_bounding_box_on_screen(*it) );
        ++it;
      }

  placement.place_balloons();
} // balloon_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the layer.
 * \param e (out) The scene elements.
 */
void bear::engine::balloon_layer::render( scene_element_list& e ) const
{
  speaker_list::const_iterator it;

  for ( it = m_speakers.begin(); it != m_speakers.end(); ++it)
    if ( !(*it)->has_finished_to_speak() )
      (*it)->get_balloon().render(e);
} // balloon_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a speaker.
 * \param speaker The speaker.
 * \pre The speaker is unknown by this layer.
 */
void bear::engine::balloon_layer::add_speaker( speaker_item* speaker )
{
#ifndef NDEBUG
  bool known = false;
  speaker_list::iterator it;
  for ( it=m_speakers.begin(); (it!=m_speakers.end()) && !known; ++it)
    if ( *it == speaker )
      known = true;

  if (known)
    CLAW_FAIL("The item is already in the balloon layer.");
#endif

  m_speakers.push_back( universe::item_handle_from(speaker) );
} // balloon_layer::add_speaker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bounding box of a speaker of the screen.
 * \param speaker The speaker.
 */
bear::universe::rectangle_type
bear::engine::balloon_layer::get_bounding_box_on_screen
( handle_type& speaker ) const
{
  const universe::rectangle_type cam( get_level().get_camera_focus() );
  const double x_ratio = get_size().x / cam.size().x;
  const double y_ratio = get_size().y / cam.size().y;

  const universe::coordinate_type left =
    x_ratio * (speaker.get_item()->get_left() - cam.left());
  const universe::coordinate_type right =
    x_ratio * (speaker.get_item()->get_right() - cam.left());
  const universe::coordinate_type top =
    y_ratio * (speaker.get_item()->get_top() - cam.bottom());
  const universe::coordinate_type bottom =
    y_ratio * (speaker.get_item()->get_bottom() - cam.bottom());

  return universe::rectangle_type( left, bottom, right, top );
} // balloon_layer::get_bounding_box_on_screen()
