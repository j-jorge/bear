/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::line class.
 * \author Julien Jorge
 */
#include "generic_items/line.hpp"

#include "visual/scene_line.hpp"

#include <limits>

BASE_ITEM_EXPORT(line, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::line::line()
: m_width(1)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // line::line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param that The instance of the parent class from which we initialise.
 */
bear::line::line( const super& that )
  : super(that), m_width(1)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // line::line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::line::build()
{
  super::build();

  adjust_position_and_size();
} // line::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::line::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if ( name == "line.width" )
    m_width = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // line::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "list of items".
 * \param name The name of the field to set.
 * \param value The value of the field.
 */
bool bear::line::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result(true);

  if ( name == "line.ends" )
    m_points = point_list_type(value.begin(), value.end());
  else
    result = super::set_item_list_field(name, value);

  return result;
} // line::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::line::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  adjust_position_and_size();
} // line::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual of the item.
 * \param visual (out) The visual representation of the item.
 */
void bear::line::get_visual( std::list<engine::scene_visual>& visuals ) const
{
  super::get_visual(visuals);

  if ( m_points.size() <= 1 )
    return;

  std::vector<visual::position_type> p;
  p.reserve(m_points.size());

  for ( point_list_type::const_iterator it=m_points.begin();
        it!=m_points.end(); ++it)
    p.push_back( (*it)->get_center_of_mass() );

  engine::scene_visual v
    ( get_scene_visual
      ( visual::scene_line
        ( 0, 0, claw::graphic::white_pixel, p, m_width) ) );

  v.scene_element.set_position( get_gap() );
  visuals.push_front(v);
} // line::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new reference point at the end of the line.
 * \param item The item to use as the new reference point.
 */
void bear::line::push_back( engine::base_item* item )
{
  m_points.push_back(item);
} // line::push_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width of the line.
 * \param w The new width of the line.
 */
void bear::line::set_line_width( visual::size_type w )
{
  m_width = w;
} // line::set_line_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the position and the size of the item to fit the points.
 */
void bear::line::adjust_position_and_size()
{
  universe::coordinate_type left =
    std::numeric_limits<universe::coordinate_type>::max();
  universe::coordinate_type right =
    std::numeric_limits<universe::coordinate_type>::min();
  universe::coordinate_type bottom = left;
  universe::coordinate_type top = right;

  for ( point_list_type::iterator it=m_points.begin(); it!=m_points.end(); )
    if ( *it == NULL )
      {
        point_list_type::iterator tmp(it);
        ++it;
        m_points.erase(tmp);
      }
    else
      {
        left = std::min(left, (*it)->get_left());
        bottom = std::min(bottom, (*it)->get_bottom());
        right = std::max(right, (*it)->get_right());
        top = std::max(top, (*it)->get_top());

        ++it;
      }

  if ( !m_points.empty() )
    {
      set_bottom(bottom);
      set_left(left);
      set_size(right - left, top - bottom);
    }
} // line::adjust_position_and_size()
