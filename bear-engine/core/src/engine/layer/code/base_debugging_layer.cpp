/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::base_debugging_layer class.
 * \author Julien Jorge
 */
#include "engine/layer/base_debugging_layer.hpp"

#include "engine/layer/layer.hpp"
#include "engine/level.hpp"
#include "engine/level_globals.hpp"
#include "engine/world.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param toggle_key The value of the key that changes the visibility of the
 *        layer.
 */
bear::engine::base_debugging_layer::base_debugging_layer
( bear::input::key_code toggle_key )
  : m_visible(false), m_toggle_key(toggle_key)
{

} // base_debugging_layer::base_debugging_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::base_debugging_layer::progress
( bear::universe::time_type elapsed_time )
{
  item_list items;

  find_items(items);

  if ( !items.empty() )
    progress(items, elapsed_time);
 } // base_debugging_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the layer on a screen.
 * \param e (out) The scene elements.
 */
void bear::engine::base_debugging_layer::render( scene_element_list& e ) const
{
  if (m_visible)
    render(e, get_level().get_camera_focus().bottom_left());
} // base_debugging_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a keyboard key has been pressed.
 * \param key The value of the pressed key.
 */
bool bear::engine::base_debugging_layer::key_pressed
( const bear::input::key_info& key )
{
  bool result = true;

  if ( key.get_code() == m_toggle_key )
    {
      m_visible = !m_visible;
      if ( m_visible )
        on_show();
    }
  else
    result = false;

  return result;
} // base_debugging_layer::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visible items.
 * \param items (out) The items found.
 */
void bear::engine::base_debugging_layer::find_items( item_list& items ) const
{
  for ( engine::level::const_layer_iterator it=get_level().layer_begin();
        it!=get_level().layer_end(); ++it )
    if ( it->has_world() )
      it->get_world().pick_items_in_rectangle
        ( items, get_level().get_camera_focus() );
} // base_debugging_layer::find_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the subclasses that the layer is displayed.
 */
void bear::engine::base_debugging_layer::on_show()
{
  // nothing to do
} // base_debugging_layer::on_show()
