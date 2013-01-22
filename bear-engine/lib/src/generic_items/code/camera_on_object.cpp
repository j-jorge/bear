/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::camera_on_object class.
 * \author Sébastien Angibaud
 */
#include "generic_items/camera_on_object.hpp"

BASE_ITEM_EXPORT( camera_on_object, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::camera_on_object::camera_on_object()
  : m_fit_items(false)
{

} // camera_on_object::camera_on_object()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera_on_object::progress( bear::universe::time_type elapsed_time )
{
  if ( m_fit_items )
    progress_fit_items(elapsed_time);
  else
    progress_center(elapsed_time);

  progress_zoom(elapsed_time);
} // camera_on_object::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type item_list.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::camera_on_object::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool ok = true;

  if (name == "camera_on_object.items")
    for( std::size_t i=0; i!=value.size(); ++i )
      m_objects.push_back( handle_type((universe::physical_item*)value[i]) );
  else
    ok = super::set_item_list_field(name, value);

  return ok;
} // camera_on_object::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::camera_on_object::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "camera_on_object.fit_items")
    m_fit_items = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // camera_on_object::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a item.
 * \param item The item to add.
 */
void bear::camera_on_object::add_item( base_item* item )
{
  m_objects.push_back( handle_type(item) );
} // camera_on_object::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item.
 * \param item The item to remove.
 */
void bear::camera_on_object::remove_item( base_item* item )
{
  for ( handle_list::iterator it=m_objects.begin(); it!=m_objects.end(); ++it )
    if ( *it == item )
      {
        m_objects.erase(it);
        return;
      }
} // camera_on_object::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera_on_object::progress_center
( bear::universe::time_type elapsed_time )
{
  bear::universe::position_type center(0, 0);
  unsigned int nb_objects(0);

  handle_list::const_iterator it;
  handle_list remaining_objects;

  for ( it = m_objects.begin(); it != m_objects.end(); ++it )
    if ( (*it).get() != NULL )
      {
        center += (*it)->get_center_of_mass();
        ++nb_objects;
        remaining_objects.push_back(*it);
      }

  std::swap(m_objects, remaining_objects);

  if ( nb_objects != 0 )
    adjust_position( center / nb_objects , elapsed_time );
} // camera_on_object::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::camera_on_object::progress_fit_items
( bear::universe::time_type elapsed_time )
{
  unsigned int nb_objects(0);
  bear::universe::position_type top_left(get_center_of_mass());
  bear::universe::position_type bottom_right(get_center_of_mass());

  handle_list::const_iterator it;
  handle_list remaining_objects;

  for ( it = m_objects.begin(); it != m_objects.end(); ++it )
    if ( (*it).get() != NULL )
      {
        if ( (*it)->get_left() < top_left.x )
          top_left.x = (*it)->get_left();

        if ( (*it)->get_right() > bottom_right.x )
          bottom_right.x = (*it)->get_right();

        if ( (*it)->get_top() > top_left.y )
          top_left.y = (*it)->get_top();

        if ( (*it)->get_bottom() < bottom_right.y )
          bottom_right.y = (*it)->get_bottom();

        ++nb_objects;
        remaining_objects.push_back(*it);
      }

  std::swap(m_objects, remaining_objects);

  if ( nb_objects != 0 )
    {
      universe::position_type center((top_left + bottom_right) /2);
      adjust_position( center , elapsed_time );

      universe::size_type r_init = get_default_size().x / get_default_size().y;
      universe::coordinate_type w(bottom_right.x - top_left.x + 200);
      universe::coordinate_type h(top_left.y  - bottom_right.y + 200*r_init);

      if ( w > h*r_init )
        set_wanted_size(universe::size_box_type(w, w/r_init));
      else
        set_wanted_size(universe::size_box_type(h*r_init, h));
    }
} // camera_on_object::progress_fit_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
void bear::camera_on_object::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_METHOD_1( camera_on_object, add_item, void,
                                   base_item* );
  TEXT_INTERFACE_CONNECT_METHOD_1( camera_on_object, remove_item, void,
                                   base_item* );
} // camera_on_object::init_exported_methods()

/*----------------------------------------------------------------------------*/
TEXT_INTERFACE_IMPLEMENT_METHOD_LIST( bear::camera_on_object )
