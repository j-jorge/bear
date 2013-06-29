/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::base_link_visual class.
 * \author Sebastien Angibaud
 */
#include "generic_items/link/base_link_visual.hpp"

#include "universe/link/base_link.hpp"

BASE_ITEM_IMPLEMENT( base_link_visual, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::base_link_visual::base_link_visual()
  : m_start(NULL), m_end(NULL),
    m_start_x_position(&universe::physical_item_state::get_horizontal_middle),
    m_start_y_position(&universe::physical_item_state::get_vertical_middle),
    m_end_x_position(&universe::physical_item_state::get_horizontal_middle),
    m_end_y_position(&universe::physical_item_state::get_vertical_middle),
    m_start_delta(0, 0), m_end_delta(0, 0),
    m_link_id(universe::base_link::not_an_id)
{
  set_global(true);
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // base_link_visual::base_link_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the linked items.
 * \param s The item where the link starts.
 * \param e The item where the link ends.
 */
void bear::base_link_visual::set_items
( engine::base_item* s, engine::base_item* e )
{
  m_start = s;
  m_end = e;
  m_link_id = universe::base_link::not_an_id;
} // base_link_visual::set_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the linked items.
 * \param s The item where the link starts.
 * \param e The item where the link ends.
 * \param id The identifier of the link between the items. If the link is
 *        removed, the visual dies.
 */
void bear::base_link_visual::set_items
( engine::base_item* s, engine::base_item* e, std::size_t link_id )
{
  m_start = s;
  m_end = e;
  m_link_id = link_id;
} // base_link_visual::set_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::base_link_visual::set_real_field( const std::string& name, double value )
{
  bool ok = true;

  if (name == "base_link_visual.start_delta.x")
    m_start_delta.x = value;
  else if (name == "base_link_visual.start_delta.y")
    m_start_delta.y = value;
  else if (name == "base_link_visual.end_delta.x")
    m_end_delta.x = value;
  else if (name == "base_link_visual.end_delta.y")
    m_end_delta.y = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // base_link_visual::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::base_link_visual::set_string_field
( const std::string& name, const std::string& value )
{
  bool ok = true;

  if (name == "base_link_visual.start_origin.x")
    {
      if (value == "left")
        m_start_x_position =
          std::mem_fun(&universe::physical_item_state::get_left);
      else if (value == "right")
        m_start_x_position =
          std::mem_fun(&universe::physical_item_state::get_right);
      else if (value == "middle")
        m_start_x_position =
          std::mem_fun(&universe::physical_item_state::get_horizontal_middle);
      else
        ok = false;
    }
  else if (name == "base_link_visual.start_origin.y")
    {
      if (value == "top")
        m_start_y_position =
          std::mem_fun(&universe::physical_item_state::get_top);
      else if (value == "bottom")
        m_start_y_position =
          std::mem_fun(&universe::physical_item_state::get_bottom);
      else if (value == "middle")
        m_start_y_position =
          std::mem_fun(&universe::physical_item_state::get_vertical_middle);
      else
        ok = false;
    }
  else if (name == "base_link_visual.end_origin.x")
    {
      if (value == "left")
        m_end_x_position =
          std::mem_fun(&universe::physical_item_state::get_left);
      else if (value == "right")
        m_end_x_position =
          std::mem_fun(&universe::physical_item_state::get_right);
      else if (value == "middle")
        m_end_x_position =
          std::mem_fun(&universe::physical_item_state::get_horizontal_middle);
      else
        ok = false;
    }
  else if (name == "base_link_visual.end_origin.y")
    {
      if (value == "top")
        m_end_y_position =
          std::mem_fun(&universe::physical_item_state::get_top);
      else if (value == "bottom")
        m_end_y_position =
          std::mem_fun(&universe::physical_item_state::get_bottom);
      else if (value == "middle")
        m_end_y_position =
          std::mem_fun(&universe::physical_item_state::get_vertical_middle);
      else
        ok = false;
    }
  else
    ok = super::set_string_field(name, value);

  return ok;
} // base_link_visual::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::base_link_visual::set_item_field
( const std::string& name, base_item* value )
{
  bool ok = true;

  if (name == "base_link_visual.start_item")
    m_start = value;
  else if (name == "base_link_visual.end_item")
    m_end = value;
  else
    ok = super::set_item_field(name, value);

  return ok;
} // base_link_visual::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::base_link_visual::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( m_link_id != universe::base_link::not_an_id )
    if ( (m_start!=NULL) && (m_end!=NULL) )
      if ( !m_start->is_linked_to(*m_end, m_link_id) )
        kill();

  update_size(elapsed_time);
} // base_link_visual::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the size of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::base_link_visual::update_size( universe::time_type elapsed_time )
{
  if ( (m_start == NULL) || (m_end == NULL) )
    kill();
  else
    {
      universe::rectangle_type r( get_start_position(), get_end_position() );

      set_bottom_left( r.bottom_left() );
      set_size( r.size() );
    }
} // base_link_visual::update_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the start extremity.
 */
bear::universe::position_type bear::base_link_visual::get_start_position() const
{
  if ( m_start == NULL )
    return universe::position_type(0, 0);
  else
    return universe::position_type
      ( m_start_x_position(m_start.get()), m_start_y_position(m_start.get()) )
      + m_start_delta;
} // base_link_visual::get_start_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the end extremity.
 */
bear::universe::position_type bear::base_link_visual::get_end_position() const
{
  if ( m_end == NULL )
    return universe::position_type(0, 0);
  else
    return universe::position_type
      ( m_end_x_position(m_end.get()), m_end_y_position(m_end.get()) )
      + m_end_delta;
} // base_link_visual::get_end_position()
