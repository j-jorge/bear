/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_snapshot_tweener class;
 * \author Julien Jorge
 */
#include "bf/model_snapshot_tweener.hpp"

#include "bf/action.hpp"
#include "bf/mark.hpp"
#include "bf/mark_placement.hpp"
#include "bf/snapshot.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param init The initial positions of the marks.
 * \remark This constructor can be use when there is no target snapshot.
 */
bf::model_snapshot_tweener::model_snapshot_tweener( const snapshot& init )
  : m_placement( init.mark_placement_begin(), init.mark_placement_end() )
{

} // model_snapshot_tweener::model_snapshot_tweener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param init The initial positions of the marks.
 * \param end The final positions of the marks.
 * \param init_action The action from which init was taken.
 * \param end_action The action from which end was taken.
 * \param d The duration of the movement.
 */
bf::model_snapshot_tweener::model_snapshot_tweener
( const snapshot& init, const snapshot& end,
  const action& init_action, const action& end_action, double d )
  : m_placement( init.mark_placement_begin(), init.mark_placement_end() )
{
  for ( std::vector<mark_placement>::iterator it=m_placement.begin();
        it!=m_placement.end(); ++it )
    {
      const mark* m = it->get_mark();

      if ( (m != NULL) && end_action.has_mark_label( m->get_label() ) )
        {
          const mark* end_mark = end_action.get_mark( m->get_label() );
          const mark_placement end_placement =
            get_mark_in_local_coordinates(init, end, end_mark);
          insert_tweener( *it, end_placement, d );
        }
    }
} // model_snapshot_tweener::model_snapshot_tweener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the mark_placements.
 */
bf::model_snapshot_tweener::const_mark_placement_iterator
bf::model_snapshot_tweener::mark_placement_begin() const
{
  return m_placement.begin();
} // model_snapshot_tweener::mark_placement_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the mark_placements.
 */
bf::model_snapshot_tweener::const_mark_placement_iterator
bf::model_snapshot_tweener::mark_placement_end() const
{
  return m_placement.end();
} // model_snapshot_tweener::mark_placement_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the tweeners for a given amount of time.
 * \param elapsed_time The duration of the update.
 */
void bf::model_snapshot_tweener::update
( double elapsed_time )
{
  m_tweeners.update(elapsed_time);
} // model_snapshot_tweener::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark placement from the target snapshot in the coordinates of
 *        the current snapshot.
 * \param init The current snapshot.
 * \param end The target snapshot.
 * \param m The mark.
 */
bf::mark_placement
bf::model_snapshot_tweener::get_mark_in_local_coordinates
( const snapshot& init, const snapshot& end, const mark* m ) const
{
  double dx = end.get_x_alignment_value();
  double dy = end.get_y_alignment_value();

  const std::string x_alignment = end.get_x_alignment();

  if ( x_alignment ==  snapshot::horizontal_alignment::align_center )
    dx += (end.get_width() - init.get_width()) / 2;
  else if ( x_alignment ==  snapshot::horizontal_alignment::align_right )
    dx += end.get_width() - init.get_width();
  else if ( x_alignment ==  snapshot::horizontal_alignment::align_left )
    dx += 0;

  const std::string y_alignment = end.get_y_alignment();
 
  if ( y_alignment == snapshot::vertical_alignment::align_center )
    dy += (end.get_height() - init.get_height()) / 2;
  else if ( y_alignment == snapshot::vertical_alignment::align_top )
    dy += end.get_height() - init.get_height();
  else if ( y_alignment == snapshot::vertical_alignment::align_bottom )
    dy += 0;

  mark_placement result( end.get_placement(m) );
  result.set_x_position( result.get_x_position() - dx );
  result.set_y_position( result.get_y_position() - dy );

  return result;
} // model_snapshot_tweener::get_mark_in_local_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a tweener to move a given mark placement to reach an other
 *        placement.
 * \param m The mark placement to tween.
 * \param end The mark placement at the final position.
 * \param d The duration of the movement.
 */
void bf::model_snapshot_tweener::insert_tweener
( mark_placement& m, const mark_placement& end, double d )
{
  m_tweeners.insert
  ( claw::tween::single_tweener
    (m.get_angle(), end.get_angle(), d,
      boost::bind( &mark_placement::set_angle, &m, _1 ),
      m.get_angle_easing().to_claw_easing_function() ) );
  m_tweeners.insert
  ( claw::tween::single_tweener
    ( m.get_x_position(), end.get_x_position(), d,
      boost::bind( &mark_placement::set_x_position, &m, _1 ),
      m.get_x_easing().to_claw_easing_function() ) );
  m_tweeners.insert
  ( claw::tween::single_tweener
    ( m.get_y_position(), end.get_y_position(), d,
      boost::bind( &mark_placement::set_y_position, &m, _1 ),
      m.get_y_easing().to_claw_easing_function() ) );
  m_tweeners.insert
  ( claw::tween::single_tweener
    ( m.get_width(), end.get_width(), d,
      boost::bind( &mark_placement::set_width, &m, _1 ),
      m.get_width_easing().to_claw_easing_function() ) );
  m_tweeners.insert
  ( claw::tween::single_tweener
    ( m.get_height(), end.get_height(), d,
      boost::bind( &mark_placement::set_height, &m, _1 ),
      m.get_height_easing().to_claw_easing_function() ) );
} // model_snapshot_tweener::insert_tweener()
