/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_snapshot class.
 * \author Julien Jorge
 */
#include "bear/engine/model/model_snapshot.hpp"

#include <stdexcept>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the alignment from its string representation.
 */
bear::engine::model_snapshot::horizontal_alignment::value
bear::engine::model_snapshot::horizontal_alignment::from_string
( const std::string& str )
{
  if ( str == "align_left" )
    return align_left;
  else if  ( str == "align_right" )
    return align_right;
  else if  ( str == "align_center" )
    return align_center;
  else
    throw std::invalid_argument("not a horizontal alignment '" + str + '\'');
} // model_snapshot::horizontal_alignment::from_string()




/*----------------------------------------------------------------------------*/
/**
 * \brief Get the alignment from its string representation.
 */
bear::engine::model_snapshot::vertical_alignment::value
bear::engine::model_snapshot::vertical_alignment::from_string
( const std::string& str )
{
  if ( str == "align_top" )
    return align_top;
  else if  ( str == "align_bottom" )
    return align_bottom;
  else if  ( str == "align_center" )
    return align_center;
  else
    throw std::invalid_argument("not a vertical alignment '" + str + '\'');
} // model_snapshot::vertical_alignment::from_string()



/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param d The date at which the snapshot is in the action.
 * \param n The count of marks in the model.
 * \param func The function to call when passing on the snapshot.
 * \param sounds The name of the sounds among which the one to play is randomly
 *        picked.
 * \param glob Tell if the sound is played globally.
 */
bear::engine::model_snapshot::model_snapshot
( universe::time_type d, std::size_t n, std::string func,
  std::vector<std::string> sounds, bool glob )
  : m_date(d), m_placement(n), m_function(func), m_sound_name(sounds),
    m_sound_is_global(glob)
{

} // model_snapshot::model_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the date at which the snapshot is in the action.
 */
bear::universe::time_type bear::engine::model_snapshot::get_date() const
{
  return m_date;
} // model_snapshot::get_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a mark placement.
 * \param m The mark placement to add.
 */
void bear::engine::model_snapshot::set_mark_placement
( const model_mark_placement& m )
{
  m_placement[m.get_mark_id()] = m;
} // model_snapshot::set_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark placement.
 * \param i The mark number for which we want the placement.
 */
const bear::engine::model_mark_placement&
bear::engine::model_snapshot::get_mark_placement( std::size_t i ) const
{
  return m_placement[i];
} // model_snapshot::get_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of mark placements.
 */
std::size_t bear::engine::model_snapshot::get_mark_placements_count() const
{
  return m_placement.size();
} // model_snapshot::get_mark_placements_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the function to call when passing on the snapshot.
 */
std::string bear::engine::model_snapshot::get_function() const
{
  return m_function;
} // model_snapshot::get_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the sound to play when passing on this snapshot.
 */
std::string bear::engine::model_snapshot::get_random_sound_name() const
{
  if ( m_sound_name.empty() )
    return std::string();
  else
    {
      std::vector<std::string>::const_iterator it=m_sound_name.begin();
      std::advance( it, rand() % m_sound_name.size() );

      return *it;
    }
} // model_snapshot::get_random_sound_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sound is played globally.
 */
bool bear::engine::model_snapshot::sound_is_global() const
{
  return m_sound_is_global;
} // model_snapshot::sound_is_global()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the mark_placements.
 */
bear::engine::model_snapshot::const_mark_placement_iterator
bear::engine::model_snapshot::mark_placement_begin() const
{
  return m_placement.begin();
} // model_snapshot::mark_placement_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the mark_placements.
 */
bear::engine::model_snapshot::const_mark_placement_iterator
bear::engine::model_snapshot::mark_placement_end() const
{
  return m_placement.end();
} // model_snapshot::mark_placement_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of this model_snapshot.
 * \param width The width of the box.
 * \param height The height of the box.
 */
void bear::engine::model_snapshot::set_size( double width, double height )
{
  m_width = width;
  m_height = height;
} // model_snapshot::set_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of this snapsot.
 */
double bear::engine::model_snapshot::get_width() const
{
  return m_width;
} // model_snapshot::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of this model_snapshot.
 */
double bear::engine::model_snapshot::get_height() const
{
  return m_height;
} // model_snapshot::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the alignment type on x-coordinate.
 */
void bear::engine::model_snapshot::set_x_alignment
(const horizontal_alignment::value a)
{
  m_x_alignment = a;
} // model_snapshot::set_x_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the alignment type on x-coordinate.
 */
bear::engine::model_snapshot::horizontal_alignment::value
bear::engine::model_snapshot::get_x_alignment() const
{
  return m_x_alignment;
} // model_snapshot::get_x_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value alignment on x-coordinate.
 */
void bear::engine::model_snapshot::set_x_alignment_value
(universe::coordinate_type value)
{
  m_x_alignment_value = value;
} // model_snapshot::set_x_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get value of the alignment on x-coordinate.
 */
bear::universe::coordinate_type
bear::engine::model_snapshot::get_x_alignment_value() const
{
  return m_x_alignment_value;
} // model_snapshot::get_x_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the alignment type on y-coordinate.
 */
void bear::engine::model_snapshot::set_y_alignment
(const vertical_alignment::value a)
{
  m_y_alignment = a;
} // model_snapshot::set_y_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the alignment type on y-coordinate.
 */
bear::engine::model_snapshot::vertical_alignment::value
bear::engine::model_snapshot::get_y_alignment() const
{
  return m_y_alignment;
} // model_snapshot::get_y_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value alignment on y-coordinate.
 */
void bear::engine::model_snapshot::set_y_alignment_value
(universe::coordinate_type value)
{
  m_y_alignment_value = value;
} // model_snapshot::set_y_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get value of the alignment on y-coordinate.
 */
bear::universe::coordinate_type
bear::engine::model_snapshot::get_y_alignment_value() const
{
  return m_y_alignment_value;
} // model_snapshot::get_y_alignment_value()
