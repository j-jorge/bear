/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_action class.
 * \author Julien Jorge
 */
#include "engine/model/model_action.hpp"

#include "engine/model/model_snapshot.hpp"

#include <stdexcept>
#include <limits>
#include <claw/real_number.hpp>

/*----------------------------------------------------------------------------*/
const std::size_t bear::engine::model_action::not_an_id
( std::numeric_limits<std::size_t>::max() );

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bear::engine::model_action::model_action()
{

} // model_action::model_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The count of marks.
 * \param dur The duration of the action.
 * \param next The action to play after this one.
 * \param snd The sound to play during this action.
 * \param glob Tell if the sound is played globally.
 */
bear::engine::model_action::model_action
( std::size_t n, universe::time_type dur, const std::string& next,
  const std::string& snd, bool glob )
  : m_mark(n), m_duration(dur), m_next(next), m_sound_name(snd),
    m_sound_is_global(glob)
{
  for ( std::size_t i=0; i!=m_mark.size(); ++i )
    m_mark[i] = new model_mark;
} // model_action::model_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::model_action::model_action( const model_action& that )
  : m_mark(that.m_mark.size()), m_duration(that.m_duration),
    m_next(that.m_next), m_sound_name(that.m_sound_name),
    m_sound_is_global(that.m_sound_is_global)
{
  for ( std::size_t i=0; i!=that.m_mark.size(); ++i )
    m_mark[i] = new model_mark(that.get_mark(i));

  snapshot_map::const_iterator it;

  for ( it=that.m_snapshot.begin(); it!=that.m_snapshot.end(); ++it )
    m_snapshot[it->first] = new model_snapshot(*it->second);
} // model_action::model_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::model_action::~model_action()
{
  snapshot_map::iterator it;

  for ( it=m_snapshot.begin(); it!=m_snapshot.end(); ++it )
    delete it->second;

  for ( std::size_t i=0; i!=m_mark.size(); ++i )
    delete m_mark[i];
} // model_action::~model_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
bear::engine::model_action&
bear::engine::model_action::operator=( model_action that )
{
  swap(that);
  return *this;
} // model_action::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap with an other action.
 * \param that The instance to swap with.
 */
void bear::engine::model_action::swap( model_action& that ) throw()
{
  std::swap(m_mark, that.m_mark);
  std::swap(m_snapshot, that.m_snapshot);
  std::swap(m_duration, that.m_duration);
  std::swap(m_next, that.m_next);
  std::swap(m_sound_name, that.m_sound_name);
  std::swap(m_sound_is_global, that.m_sound_is_global);
} // model_action::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark.
 * \param i The index of the mark.
 */
bear::engine::model_mark&
bear::engine::model_action::get_mark( std::size_t i ) const
{
  CLAW_PRECOND( i < m_mark.size() );

  return *m_mark[i];
} // model_action::get_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark given his name.
 * \param n The label of the mark.
 */
bear::engine::model_mark&
bear::engine::model_action::get_mark( const std::string& n ) const
{
  return get_mark( get_mark_id(n) );
} // model_action::get_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the id of a mark given its label.
 * \param n The label of the mark.
 * \return The id of the mark, or get_marks_count() if no mark is found with
 *         this label.
 */
std::size_t
bear::engine::model_action::get_mark_id( const std::string& n ) const
{
  std::size_t i;
  bool found(false);

  for ( i=0; !found && (i!=m_mark.size()); ++i )
    found = m_mark[i]->get_label() == n;

  if ( found )
    return i-1;
  else
    return not_an_id;
} // model_action::get_mark_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the count of marks in the action.
 */
std::size_t bear::engine::model_action::get_marks_count() const
{
  return m_mark.size();
} // model_action::get_marks_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration of the action.
 */
bear::universe::time_type bear::engine::model_action::get_duration() const
{
  return m_duration;
} // model_action::get_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a snapshot in the action.
 * \param s The snapshot.
 */
void bear::engine::model_action::add_snapshot( const model_snapshot& s )
{
  CLAW_PRECOND( m_snapshot.find(s.get_date()) == m_snapshot.end() );

  m_snapshot[s.get_date()] = new model_snapshot(s);
} // model_action::add_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the snapshot of a given date (or the one before the date).
 * \param t The date.
 */
bear::engine::model_action::const_snapshot_iterator
bear::engine::model_action::get_snapshot_at( universe::time_type t ) const
{
  return get_snapshot_const_iterator_at(t);
} // model_action::get_snapshot_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the beginning of the snapshot sequence.
 */
bear::engine::model_action::const_snapshot_iterator
bear::engine::model_action::snapshot_begin() const
{
  return m_snapshot.begin();
} // model_action::snapshot_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the snapshot sequence.
 */
bear::engine::model_action::const_snapshot_iterator
bear::engine::model_action::snapshot_end() const
{
  return m_snapshot.end();
} // model_action::snapshot_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the beginning of the snapshot sequence.
 */
bear::engine::model_action::snapshot_iterator
bear::engine::model_action::snapshot_begin()
{
  return m_snapshot.begin();
} // model_action::snapshot_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the snapshot sequence.
 */
bear::engine::model_action::snapshot_iterator
bear::engine::model_action::snapshot_end()
{
  return m_snapshot.end();
} // model_action::snapshot_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the action to play after this one.
 */
const std::string& bear::engine::model_action::get_next_action() const
{
  return m_next;
} // model_action::get_next_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the sound to play.
 */
const std::string& bear::engine::model_action::get_sound_name() const
{
  return m_sound_name;
} // model_action::get_sound_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sound is played globally.
 */
bool bear::engine::model_action::sound_is_global() const
{
  return m_sound_is_global;
} // model_action::sound_is_global()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the greatest size of this action, among all snapshots.
 * \param width The greatest width.
 * \param height The greatest height
 */
void bear::engine::model_action::get_max_size
( double& width, double& height ) const
{
  snapshot_map::const_iterator it(m_snapshot.begin());
  snapshot_map::const_iterator eit(m_snapshot.end());

  if ( it != eit )
    {
      width = it->second->get_width();
      height = it->second->get_height();
    }
  else
    {
      width = 0;
      height = 0;
    }

  for (; it!=eit; ++it)
    {
      if ( width > it->second->get_width() )
        width = it->second->get_width();

      if ( height > it->second->get_height() )
        height = it->second->get_height();
    }
} // model_action::get_max_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the marks.
 */
bear::engine::model_action::mark_iterator
bear::engine::model_action::mark_begin() const
{
  return m_mark.begin();
} // model_action::mark_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the marks.
 */
bear::engine::model_action::mark_iterator
bear::engine::model_action::mark_end() const
{
  return m_mark.end();
} // model_action::mark_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the cumulated visibility of a mark in a time interval.
 * \param m The mark for which we want the visibility.
 * \param from The beginning of the time interval.
 * \param to The end of the time interval.
 */
bear::universe::time_type
bear::engine::model_action::accumulated_mark_visibility
( const model_mark& m, universe::time_type from, universe::time_type to ) const
{
  CLAW_PRECOND( from <= to );

  snapshot_map::const_iterator it( get_snapshot_const_iterator_at(from) );
  snapshot_map::const_iterator to_s( get_snapshot_const_iterator_at(to) );
  const std::size_t id=get_mark_id(m.get_label());
  double result;

  if ( it == m_snapshot.end() )
    --it;

  if ( to_s == m_snapshot.end() )
    --to_s;

  // The first iterator is before \a from. If the mark is visible, we
  // remove the time between the snapshot and \a from.
  if ( it->second->get_mark_placement(id).is_visible() )
    result = -(from - it->first);
  else
    result = 0;

  // Then we sum the duration of the snapshots where the mark is
  // visible until the last snapshot.
  while ( it!=to_s )
   {
      snapshot_map::const_iterator n(it);
      ++n;

      if ( it->second->get_mark_placement(id).is_visible() )
        result += n->first - it->first;

      it = n;
    }

  // Finally, we add the time during which the mark is visible during
  // the last snapshot
  if ( to_s->second->get_mark_placement(id).is_visible() )
    result += to - to_s->first;

  return result;
} // model_action::accumulated_mark_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the snapshot of a given date (or the one before the date).
 * \param t The date.
 */
bear::engine::model_action::snapshot_map::const_iterator
bear::engine::model_action::get_snapshot_const_iterator_at
( universe::time_type t ) const
{
  if ( claw::real_number<universe::time_type>(t) > get_duration() )
    return m_snapshot.end();
  else if ( m_snapshot.empty() )
    return m_snapshot.end();
  else
    {
      snapshot_map::const_iterator it( m_snapshot.lower_bound(t) );
      const snapshot_map::const_iterator eit( m_snapshot.end() );

      if ( it == eit )
        --it;
      else if ( it->first != t )
        --it;

      return it;
    }
} // model_action::get_snapshot_const_iterator_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two actions.
 * \param a The first action.
 * \param b The second action.
 */
void
bear::engine::swap( engine::model_action& a, engine::model_action& b ) throw()
{
  a.swap(b);
} // bear::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two actions.
 * \param a The first action.
 * \param b The second action.
 */
template<>
void std::swap
( bear::engine::model_action& a, bear::engine::model_action& b )
{
  a.swap(b);
} // std::swap()
