/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_player class.
 * \author Julien Jorge
 */
#include "bf/action_player.hpp"

#include "bf/action.hpp"
#include "bf/mark.hpp"
#include "bf/snapshot.hpp"

#include <claw/real_number.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param a The action read by the player.
 */
bf::action_player::action_player( const action* a )
  : m_action(NULL)
{
  set_action(a);
} // action_player::action_player()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the action read by the player.
 * \param a The action read by the player.
 */
void bf::action_player::set_action( const action* a )
{
  m_player.clear();
  m_action = a;
  m_date = 0;

  if ( m_action != NULL )
    {
      action::const_mark_ptr_iterator it;

      for ( it=m_action->mark_ptr_begin(); it!=m_action->mark_ptr_end(); ++it )
        if ( (*it)->has_animation() )
          m_player[*it].set_animation( (*it)->get_animation_data() );
    }

  fill_sprites();
} // action_player::set_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set all animations on date zero.
 */
void bf::action_player::reset()
{
  m_date = 0;

  player_map::iterator it;

  for (it=m_player.begin(); it!=m_player.end(); ++it)
    it->second.reset();

  fill_sprites();
} // action_player::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go on the next change.
 */
void bf::action_player::next()
{
  if ( !is_finished() )
    next( get_duration_until_next() );
} // action_player::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the action during a given duration.
 * \param d The duration.
 * \pre d >= 0
 */
void bf::action_player::next( double d )
{
  CLAW_PRECOND( d >= 0 );

  player_map::iterator it;
  const double new_date(m_date + d);

  for (it=m_player.begin(); it!=m_player.end(); ++it)
    it->second.next
      ( m_action->accumulated_mark_visibility(it->first, m_date, new_date) );

  m_date = new_date;

  fill_sprites();
} // action_player::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current date.
 */
double bf::action_player::get_date() const
{
  return m_date;
} // action_player::get_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current date.
 * \param d The new date.
 */
void bf::action_player::set_date( double d )
{
  if ( d >= m_date )
    next( d - m_date );
  else
    {
      reset();
      next(d);
    }
} // action_player::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the remaining time until the next change.
 */
double bf::action_player::get_duration_until_next() const
{
  double result;
  const snapshot* s = m_action->get_snapshot_after_date(m_date);

  if ( s != NULL )
    result = s->get_date() - m_date;
  else
    result = m_action->get_duration() - m_date;

  player_map::const_iterator it;

  for (it=m_player.begin(); it!=m_player.end(); ++it)
    result = std::min(result, it->second.get_duration_until_next());

  result = std::max(result, 0.001);

  if ( s != NULL )
    result = std::min(result, s->get_date() - m_date);

  return result;
} // action_player::get_duration_until_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite of a given mark at the current date.
 * \param m The mark for which we want the sprite.
 */
bf::sprite bf::action_player::get_current_sprite_for( const mark* m ) const
{
  sprite result;

  sprite_map::const_iterator it=m_sprite.find(m);

  if ( it!=m_sprite.end() )
    result = it->second;

  return result;
} // action_player::get_current_sprite_for()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the action has been completely read.
 */
bool bf::action_player::is_finished() const
{
  bool result;

  if ( m_action != NULL )
    result = (m_date >= m_action->get_duration());
  else
    result = true;

  return result;
} // action_player::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill m_sprite.
 */
void bf::action_player::fill_sprites()
{
  m_sprite.clear();

  if ( m_action == NULL )
    return;

  const snapshot* s = m_action->get_snapshot_before_or_at_date(m_date);

  if ( s == NULL )
    return;

  snapshot::const_mark_placement_iterator it;

  for( it=s->mark_placement_begin(); it!=s->mark_placement_end(); ++it )
    {
      const mark* m( it->get_mark() );

      if ( m->has_animation() )
        {
          player_map::const_iterator itp=m_player.find(m);

          CLAW_ASSERT
            ( itp!=m_player.end(),
              "There is no player for a mark with an animation." );

          m_sprite[it->get_mark()] = itp->second.get_sprite();

          if ( m->apply_angle_to_animation() )
            m_sprite[it->get_mark()].set_angle
              ( m_sprite[it->get_mark()].get_angle()
                + it->get_angle() );
        }
    }
} // action_player::fill_sprites()
