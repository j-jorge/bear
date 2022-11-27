/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::item_information_layer class.
 * \author Julien Jorge
 */
#include "bear/generic_items/layer/item_information_layer.hpp"

#include "bear/engine/level.hpp"
#include "bear/engine/level_globals.hpp"
#include "bear/engine/world.hpp"
#include "bear/engine/layer/layer.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item from which we want informations.
 * \param font The font used to display the text.
 */
bear::item_information_layer::info_box::info_box
( bear::universe::physical_item& item, const bear::visual::font& font )
  : m_item(&item), m_delta( 0, 0 ), m_text( font )
{
  m_text.set_auto_size(true);
  m_text.set_background_color( bear::gui::color_type( "#000000" ) );

  get_informations();
} // item_information_layer::info_box::info_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::item_information_layer::info_box::~info_box()
{
  // nothing to do
} // item_information_layer::info_box::~info_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the box includes a given point.
 * \param pos The point to check.
 */
bool bear::item_information_layer::info_box::includes
( const claw::math::coordinate_2d<unsigned int>& pos ) const
{
  claw::math::rectangle<unsigned int> box
    ( m_text.get_position().x, m_text.get_position().y,
      m_text.width(), m_text.height() );

  return box.includes(pos);
} // item_information_layer::info_box::includes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this info_box links to a given item.
 * \param item The item to check
 */
bool bear::item_information_layer::info_box::has_item
( const bear::universe::physical_item* item ) const
{
  return m_item == item;
} // item_information_layer::info_box::has_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the informations.
 * \param delta Vector to add to the current position.
 */
void bear::item_information_layer::info_box::add_delta
( const claw::math::coordinate_2d<int>& delta )
{
  m_delta += delta;
} // item_information_layer::info_box::add_delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of the box.
 * \param camera_box The box of the camera in the level.
 * \param layer_size The size of the layer.
 */
void bear::item_information_layer::info_box::update
( const bear::universe::rectangle_type& camera_box,
  const claw::math::coordinate_2d<unsigned int>& layer_size)
{
  claw::math::coordinate_2d<double> ratio
    ( camera_box.size().x / layer_size.x,
      camera_box.size().y / layer_size.y );

  if ( m_item != NULL )
    {
      bear::universe::position_type pos;
      pos.x = (m_item->get_right() - camera_box.left())/ratio.x + m_delta.x;
      pos.y = (m_item->get_top() - camera_box.bottom())/ratio.y + m_delta.y;
      m_text.set_position( pos );
    }
  else
    m_text.set_text("Invalid handle.");
} // item_information_layer::info_box::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the text box.
 * \param e (out) The scene elements.
 */
void bear::item_information_layer::info_box::render
( scene_element_list& e ) const
{
  m_text.render( e );
} // item_information_layer::info_box::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the informations of the item.
 */
void bear::item_information_layer::info_box::get_informations()
{
  if ( m_item != NULL )
    {
      std::ostringstream oss;

      oss << *m_item << std::endl;
      m_text.set_text( oss.str() );
    }
} // item_information_layer::info_box::get_informations()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item from which we want informations.
 * \param font The font used to display the text.
 */
bear::item_information_layer::linked_info_box::linked_info_box
( bear::universe::physical_item& item, const bear::visual::font& font )
  : info_box(item, font)
{

} // item_information_layer::linked_info_box::linked_info_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of the box.
 * \param layer_size The size of the layer.
 * \param camera_box The box of the camera in the level.
 */
void bear::item_information_layer::linked_info_box::update
( const bear::universe::rectangle_type& camera_box,
  const claw::math::coordinate_2d<unsigned int>& layer_size )
{
  get_informations();
  info_box::update(camera_box, layer_size);
} // item_information_layer::linked_info_box::update()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::item_information_layer::item_information_layer()
  : m_dragged_item(NULL)
{

} // item_information_layer::item_information_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::item_information_layer::~item_information_layer()
{
  clear();
} // item_information_layer::~item_information_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the layer.
 */
void bear::item_information_layer::clear()
{
  info_box_list::iterator it;

  for (it=m_info_box.begin(); it!=m_info_box.end(); ++it)
    delete *it;

  m_info_box.clear();
} // item_information_layer::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param elapsed_time Elapsed time since the last call.
 */
void
bear::item_information_layer::progress( bear::universe::time_type elapsed_time )
{
  info_box_list::iterator it;

  for (it=m_info_box.begin(); it!=m_info_box.end(); ++it)
    (*it)->update( get_level().get_camera_focus(), get_size() );
} // item_information_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the layer on a screen.
 * \param e (out) The scene elements.
 */
void bear::item_information_layer::render( scene_element_list& e ) const
{
  info_box_list::const_iterator it;

  for (it=m_info_box.begin(); it!=m_info_box.end(); ++it)
    (*it)->render(e);
} // item_information_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been pressed.
 * \param key The value of the maintained button.
 * \param pos The current position of the cursor.
 */
bool bear::item_information_layer::mouse_pressed
( bear::input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;

  switch(key)
    {
    case bear::input::mouse::mc_left_button:
      result = grab_info_box(pos);
      break;
    case bear::input::mouse::mc_right_button:
      result = put_in_background(pos);
      break;
    case bear::input::mouse::mc_middle_button:
      result = close_info_box(pos);
      break;
    }

  if (!result)
    switch(key)
      {
      case bear::input::mouse::mc_left_button:
        result = show_item( pos, get_level().get_camera_focus() );
        break;
      case bear::input::mouse::mc_right_button:
        result = follow_item( pos, get_level().get_camera_focus() );
        break;
      case bear::input::mouse::mc_middle_button:
        if ( !m_info_box.empty() )
          {
            result = true;
            clear();
          }
        break;
      }

  return result;
} // item_information_layer::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been released.
 * \param key The value of the released button.
 * \param pos The current position of the cursor.
 */
bool bear::item_information_layer::mouse_released
( bear::input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;

  if (key == bear::input::mouse::mc_left_button)
    if (m_dragged_item != NULL)
      {
        m_dragged_item->add_delta( pos - m_drag_reference );
        m_dragged_item = NULL;
        result = true;
      }

  return result;
} // item_information_layer::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button is maintained.
 * \param key The value of the pressed button.
 * \param pos The current position of the cursor.
 */
bool bear::item_information_layer::mouse_maintained
( bear::input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;

  if (key == bear::input::mouse::mc_left_button)
    if (m_dragged_item != NULL)
      {
        m_dragged_item->add_delta( pos - m_drag_reference );
        m_drag_reference = pos;
        result = true;
      }

  return result;
} // item_information_layer::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the informations of an item, given a position.
 * \param pos The position where the item is searched.
 * \param camera_box The box of the camera in the level.
 */
bool bear::item_information_layer::show_item
( const bear::universe::position_type& pos,
  const bear::universe::rectangle_type& camera_box )
{
  claw::math::coordinate_2d<double> ratio
    ( camera_box.size().x / get_size().x,
      camera_box.size().y / get_size().y );

  bear::universe::position_type r_pos;
  r_pos.x = pos.x*ratio.x + camera_box.left();
  r_pos.y = pos.y*ratio.y + camera_box.bottom();

  bear::universe::physical_item* item = find_item(r_pos);

  if (item != NULL)
    {
      m_info_box.push_front
      ( new info_box
        (*item, get_level_globals().get_font("font/fixed_white-7x12.fnt",12)) );
    }
  return item != NULL;
} // item_information_layer::show_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Follow an item and show its informations, given a position.
 * \param pos The position where the item is searched.
 * \param camera_box The box of the camera in the level.
 */
bool bear::item_information_layer::follow_item
( const bear::universe::position_type& pos,
  const bear::universe::rectangle_type& camera_box )
{
  claw::math::coordinate_2d<double> ratio
    ( camera_box.size().x / get_size().x,
      camera_box.size().y / get_size().y );

  bear::universe::position_type r_pos;
  r_pos.x = pos.x*ratio.x + camera_box.left();
  r_pos.y = pos.y*ratio.y + camera_box.bottom();

  bear::universe::physical_item* item = find_item(r_pos);

  if (item != NULL)
    {
      m_info_box.push_front
      ( new linked_info_box
        ( *item, get_level_globals().get_font
          ("font/fixed_white-7x12.fnt",12) ) );
    }
  return item != NULL;
} // item_information_layer::follow_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close an information box, given a position.
 * \param pos The position where the box is searched.
 */
bool bear::item_information_layer::close_info_box
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  info_box_list::iterator it = find_info_box(pos);

  if (it!=m_info_box.end())
    {
      delete *it;
      m_info_box.erase(it);
      return true;
    }
  else
    return false;
} // item_information_layer::close_info_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Grab an information box, given a position.
 * \param pos The position where the box is searched.
 */
bool bear::item_information_layer::grab_info_box
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  info_box_list::iterator it = find_info_box(pos);

  if ( it!=m_info_box.end() )
    {
      m_dragged_item = *it;
      m_drag_reference = pos;
      return true;
    }
  else
    return false;
} // item_information_layer:grab_info_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Put an information box in the background, given a position.
 * \param pos The position where the box is searched.
 */
bool bear::item_information_layer::put_in_background
( const claw::math::coordinate_2d<unsigned int>& pos )
{
 info_box_list::iterator it = find_info_box(pos);

  if ( it!=m_info_box.end() )
    {
      info_box* item = *it;
      m_info_box.erase(it);
      m_info_box.push_front(item);
      return true;
    }
  else
    return false;
} // item_information_layer:put_in_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find an information box, given a position.
 * \param pos The position where the box is searched.
 */
bear::item_information_layer::info_box_list::iterator
bear::item_information_layer::find_info_box
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool found = false;

  info_box_list::iterator it = m_info_box.end();

  while ( !found && (it!=m_info_box.begin()) )
    {
      --it;

      if ( (*it)->includes(pos) )
        found = true;
    }

  if (found)
    return it;
  else
    return m_info_box.end();
} // item_information_layer::find_info_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find an item, given a position.
 * \param pos The position where the item is searched.
 */
bear::universe::physical_item* bear::item_information_layer::find_item
( const bear::universe::position_type& pos ) const
{
  universe::physical_item* result = NULL;

  for ( engine::level::const_layer_iterator it=get_level().layer_begin();
        (result == NULL) && (it!=get_level().layer_end()); ++it )
    if ( it->has_world() )
      {
        universe::world::item_list items;

        it->get_world().pick_items_by_position( items, pos );

        universe::world::item_list::const_iterator it;

        for ( it=items.begin(); (it!=items.end()) && (result==NULL); ++it )
          if ( !is_handled(*it) )
            result = *it;
      }

  return result;
} // item_information_layer::find_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have an info_box on a given item.
 * \param item The item to search.
 */
bool bear::item_information_layer::is_handled
( const bear::universe::physical_item* item ) const
{
  bool result = false;

  info_box_list::const_iterator it;

  for (it=m_info_box.begin(); !result && (it!=m_info_box.end()); ++it )
    result = (*it)->has_item(item);

  return result;
} // item_information_layer::is_handled()
