/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_snapshot_tweener class;
 * \author Julien Jorge
 */
#include "engine/model/model_snapshot_tweener.hpp"

#include "engine/model/model_action.hpp"
#include "engine/model/model_mark_placement.hpp"
#include "engine/model/model_snapshot.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param init The initial positions of the marks.
 * \remark This constructor can be use when there is no target snapshot.
 */
bear::engine::model_snapshot_tweener::model_snapshot_tweener
( const model_snapshot& init )
{
  m_placement.resize(init.get_mark_placements_count());

  for ( std::size_t i=0; i!=m_placement.size(); ++i )
    m_placement[i] = init.get_mark_placement(i);
} // model_snapshot_tweener::model_snapshot_tweener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param init The initial positions of the marks.
 * \param end The final positions of the marks.
 * \param start_action The action from which init was taken.
 * \param end_action The action from which end was taken.
 * \param d The duration of the movement.
 */
bear::engine::model_snapshot_tweener::model_snapshot_tweener
( const model_snapshot& init, const model_snapshot& end,
  const model_action& init_action, const model_action& end_action,
  universe::time_type d )
{
  m_placement.resize(init.get_mark_placements_count());

  for ( std::size_t i=0; i!=m_placement.size(); ++i )
    {
      m_placement[i] = init.get_mark_placement(i);

      const std::size_t end_id
      ( end_action.get_mark_id(init_action.get_mark(i).get_label()) );

      if ( end_id != model_action::not_an_id )
        {
          const model_mark_placement end_mark =
              get_mark_in_local_coordinates(init, end, end_id);
          insert_tweener( i, end_mark, d );
        }
    }
} // model_snapshot_tweener::model_snapshot_tweener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark placement.
 * \param i The mark number for which we want the placement.
 */
bear::engine::model_mark_placement&
bear::engine::model_snapshot_tweener::get_mark_placement( std::size_t i )
{
  return m_placement[i];
} // model_snapshot_tweener::get_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark placement.
 * \param i The mark number for which we want the placement.
 */
const bear::engine::model_mark_placement&
bear::engine::model_snapshot_tweener::get_mark_placement( std::size_t i ) const
{
  return m_placement[i];
} // model_snapshot_tweener::get_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the mark_placements.
 */
bear::engine::model_snapshot_tweener::const_mark_placement_iterator
bear::engine::model_snapshot_tweener::mark_placement_begin() const
{
  return m_placement.begin();
} // model_snapshot_tweener::mark_placement_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the mark_placements.
 */
bear::engine::model_snapshot_tweener::const_mark_placement_iterator
bear::engine::model_snapshot_tweener::mark_placement_end() const
{
  return m_placement.end();
} // model_snapshot_tweener::mark_placement_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the tweeners for a given amount of time.
 * \param elapsed_time The duration of the update.
 */
void bear::engine::model_snapshot_tweener::update
( universe::time_type elapsed_time )
{
  m_tweeners.update(elapsed_time);
} // model_snapshot_tweener::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark placement from the target snapshot in the coordinates of
 *        the current snapshot.
 * \param init The current snapshot.
 * \param end The target snapshot.
 * \param id The identifier of the mark.
 */
bear::engine::model_mark_placement
bear::engine::model_snapshot_tweener::get_mark_in_local_coordinates
( const model_snapshot& init, const model_snapshot& end,
    std::size_t id ) const
{
  bear::universe::coordinate_type dx = end.get_x_alignment_value();
  bear::universe::coordinate_type dy = end.get_y_alignment_value();

  switch ( end.get_x_alignment() )
  {
  case model_snapshot::horizontal_alignment::align_center:
    dx += (end.get_width() - init.get_width()) / 2;
    break;
  case model_snapshot::horizontal_alignment::align_right:
    dx += end.get_width() - init.get_width();
    break;
  case model_snapshot::horizontal_alignment::align_left:
    dx += 0;
    break;
  }

  switch ( end.get_y_alignment() )
  {
  case model_snapshot::vertical_alignment::align_center:
    dy += (end.get_height() - init.get_height()) / 2;
    break;
  case model_snapshot::vertical_alignment::align_top:
    dy += end.get_height() - init.get_height();
    break;
  case model_snapshot::vertical_alignment::align_bottom:
    dy += 0;
    break;
  }

  model_mark_placement result( end.get_mark_placement(id) );
  result.set_x_position( result.get_position().x - dx );
  result.set_y_position( result.get_position().y - dy );

  return result;
} // model_snapshot_tweener::get_mark_in_local_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a tweener to move a given mark placement to reach an other
 *        placement.
 * \param id The identifier of the mark to tween.
 * \param end The mark placement at the final position.
 * \param d The duration of the movement.
 */
void bear::engine::model_snapshot_tweener::insert_tweener
( std::size_t id, const model_mark_placement& end, universe::time_type d )
{
  if ( m_placement[id].get_angle() != end.get_angle() )
    m_tweeners.insert
      ( claw::tween::single_tweener
        (m_placement[id].get_angle(), end.get_angle(), d,
         boost::bind( &model_mark_placement::set_angle, &m_placement[id],
                      boost::placeholders::_1 ),
         m_placement[id].get_angle_easing() ) );

  if ( m_placement[id].get_position().x != end.get_position().x )
    m_tweeners.insert
      ( claw::tween::single_tweener
        (m_placement[id].get_position().x, end.get_position().x, d,
         boost::bind
         ( &model_mark_placement::set_x_position, &m_placement[id],
           boost::placeholders::_1 ),
         m_placement[id].get_x_position_easing() ) );

  if ( m_placement[id].get_position().y != end.get_position().y )
    m_tweeners.insert
      ( claw::tween::single_tweener
        (m_placement[id].get_position().y, end.get_position().y, d,
         boost::bind
         ( &model_mark_placement::set_y_position, &m_placement[id],
           boost::placeholders::_1 ),
         m_placement[id].get_y_position_easing() ) );

  if ( m_placement[id].get_size().x != end.get_size().x )
    m_tweeners.insert
      ( claw::tween::single_tweener
        (m_placement[id].get_size().x, end.get_size().x, d,
         boost::bind
         ( &model_mark_placement::set_width, &m_placement[id],
           boost::placeholders::_1 ),
         m_placement[id].get_width_easing() ) );

  if ( m_placement[id].get_size().y != end.get_size().y )
    m_tweeners.insert
      ( claw::tween::single_tweener
        (m_placement[id].get_size().y, end.get_size().y, d,
         boost::bind
         ( &model_mark_placement::set_height, &m_placement[id],
           boost::placeholders::_1 ),
         m_placement[id].get_height_easing() ) );
} // model_snapshot_tweener::insert_tweener()
