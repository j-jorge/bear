/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action class.
 * \author Julien Jorge
 */
#include "bf/action.hpp"

#include "bf/mark.hpp"
#include "bf/snapshot.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param action_name The name of the action.
 * \param duration The duration of the action.
 * \param sound The sound file.
 * \param auto_next The new next action.
 */
bf::action::action
( const std::string& action_name, double duration,
  const sound_description& sound, const std::string& auto_next )
  : m_name(action_name), m_duration(duration), m_sound(sound),
    m_auto_next(auto_next)
{
  create_initial_snapshot();
} // action::action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param action_name The name of the action.
 * \param duration The duration of the action.
 */
bf::action::action
(const std::string& action_name, double duration)
  : m_name(action_name), m_duration(duration)
{

} // action::action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bf::action::action( const action& that )
  : m_name(that.m_name), m_duration(that.m_duration), m_sound(that.m_sound),
    m_auto_next(that.m_auto_next)
{
  mark_list::const_iterator it;
  std::map<const mark*, const mark*> marks;

  for ( it=that.m_marks.begin(); it!=that.m_marks.end(); ++it)
    {
      m_marks.push_back( new mark(**it) );
      marks[*it] = m_marks.back();
    }

  snapshot_list::const_iterator it_2;
  for ( it_2=that.m_snapshots.begin(); it_2!=that.m_snapshots.end(); ++it_2)
    m_snapshots.push_back( new snapshot(**it_2, marks) );
} // action::action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::action::~action()
{
  mark_list::const_iterator it;

  for ( it = m_marks.begin(); it != m_marks.end(); ++it)
    delete *it;

  m_marks.clear();

  snapshot_list::const_iterator it_2;
  for ( it_2 = m_snapshots.begin(); it_2 != m_snapshots.end(); ++it_2)
    delete *it_2;

  m_snapshots.clear();
} // action::~action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a snapshot in the context of this action.
 */
bf::snapshot* bf::action::create_snapshot() const
{
  snapshot* result = new snapshot;
  init_snapshot(*result);
  return result;
} // action::create_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize a snapshot with the marks of the action.
 * \param s The snapshot to add.
 */
void bf::action::init_snapshot( snapshot& s ) const
{
  // Add the marks in the snapshot
  for ( mark_list::const_iterator mit=m_marks.begin();
        mit!=m_marks.end(); ++mit )
    if ( !s.has_mark(*mit) )
      s.add_mark(*mit);
} // action::init_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a snapshot.
 * \param s The snapshot to add.
 */
void bf::action::add_snapshot( snapshot* s )
{
  CLAW_PRECOND( s != NULL );

  snapshot_list::iterator it=get_iterator_greater_equal(s->get_date());

  init_snapshot( *s );

  if ( it==m_snapshots.end() )
    // add the new snapshot
    m_snapshots.push_back(s);
  else if ( (*it)->get_date() != s->get_date() )
    // create a snapshot with a copy of the marks
    it = m_snapshots.insert( it, s );
} // action::add_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a snapshot.
 * \param s The snapshot to remove.
 */
void bf::action::remove_snapshot( snapshot* s )
{
  snapshot_list::iterator it;
  bool ok = false;
  for ( it = m_snapshots.begin(); (it != m_snapshots.end()) && !ok;)
    if ( *it == s )
      ok = true;
    else
      ++it;

  if (ok)
    m_snapshots.erase(it);
} // action::remove_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the action contains a snapshot.
 * \param s The snapshot to test.
 */
bool bf::action::has_snapshot( const snapshot* s ) const
{
  snapshot_list::const_iterator it;
  bool ok = false;

  for ( it = m_snapshots.begin(); (it != m_snapshots.end()) && !ok; ++it)
    if ( *it == s )
      ok = true;

  return ok;
} // action::has_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the action contains a snapshot at a given date.
 * \param date The snapshot date to test.
 */
bool bf::action::has_snapshot_date( double date ) const
{
  snapshot_list::const_iterator it;
  bool ok = false;

  for ( it = m_snapshots.begin(); (it != m_snapshots.end()) && !ok; ++it)
    if ( (*it)->get_date() == date )
      ok = true;

  return ok;
} // action::has_snapshot_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there exists a greater date.
 * \param date The given date.
*/
bool bf::action::has_greater_date( double date ) const
{
  bool result = false;

  if ( !m_snapshots.empty() )
    result = ( date < (*m_snapshots.rbegin())->get_date() );

  return result;
} // action::has_greater_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the last snapshot before a given date.
 * \param date The snapshot date considered.
 */
const bf::snapshot* bf::action::get_snapshot_before_date( double date ) const
{
  snapshot_list::const_iterator it( get_const_iterator_greater_equal(date) );

  if ( it==m_snapshots.begin() )
    return NULL;
  else
    {
      --it;
      return *it;
    }
} // action::get_snapshot_before_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the snapshot at a given date or before if there is no such
 *        snapshot.
 * \param date The snapshot date considered.
 */
const bf::snapshot*
bf::action::get_snapshot_before_or_at_date( double date ) const
{
  snapshot_list::const_iterator it( get_const_iterator_less_equal(date) );

  if ( it == m_snapshots.end() )
    return NULL;
  else
    return *it;
} // action::get_snapshot_before_or_at_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the snapshot at a given date or before if there is no such
 *        snapshot.
 * \param date The snapshot date considered.
 */
bf::snapshot* bf::action::get_snapshot_before_or_at_date( double date )
{
  snapshot_list::iterator it( get_iterator_less_equal(date) );

  if ( it == m_snapshots.end() )
    return NULL;
  else
    return *it;
} // action::get_snapshot_before_or_at_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get snapshot after a given date.
 * \param date The snapshot date considered.
 */
const bf::snapshot* bf::action::get_snapshot_after_date( double date ) const
{
  snapshot_list::const_iterator it=get_const_iterator_greater_equal(date);

  if ( it != m_snapshots.end() )
    {
      if ( (*it)->get_date() == date )
        ++it;

      if ( it != m_snapshots.end() )
        return *it;
      else
        return NULL;
    }
  else
    return NULL;
} // action::get_snapshot_after_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a snapshot.
 * \param date The date of the snapshot to return.
 */
bf::snapshot* bf::action::get_snapshot_after_or_at_date( double date )
{
  CLAW_PRECOND( has_snapshot_date(date) );

  return *get_iterator_greater_equal(date);
} // action::get_snapshot_after_or_at_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a snapshot.
 * \param date The date of the snapshot to return.
 */
const bf::snapshot*
bf::action::get_snapshot_after_or_at_date( double date ) const
{
  CLAW_PRECOND( has_snapshot_date(date) );

  return *get_const_iterator_greater_equal(date);
} // action::get_snapshot_after_or_at_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get snapshot after a given date.
 * \param date The snapshot date considered.
 * \param snapshots The list of snapshots in return.
 */
void bf::action::get_all_snapshots_after_or_at_date
( double date, snapshot_list& snapshots ) const
{
  snapshot_list::const_iterator it=get_const_iterator_greater_equal(date);

  for ( ; it != m_snapshots.end(); ++it )
    snapshots.push_back(*it);
} // action::get_all_snapshots_after_or_at_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the beginning of the snapshot list.
 */
bf::action::snapshot_iterator bf::action::snapshot_begin()
{
  return m_snapshots.begin();
} // snapshot::snapshot_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the snapshot list.
 */
bf::action::snapshot_iterator bf::action::snapshot_end()
{
  return m_snapshots.end();
} // snapshot::snapshot_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a const iterator at the begin of snapshot list.
 */
bf::action::const_snapshot_iterator bf::action::snapshot_begin() const
{
  return m_snapshots.begin();
} // snapshot::snapshot_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a const iterator on the end of snapshot list.
 */
bf::action::const_snapshot_iterator bf::action::snapshot_end() const
{
  return m_snapshots.end();
} // snapshot::snapshot_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new mark.
 * The mark is added in all snapshots.
 */
void bf::action::add_mark(mark* m)
{
  snapshot_list::iterator it;

  for (it=m_snapshots.begin(); it!=m_snapshots.end(); ++it)
    (*it)->add_mark(m);

  m_marks.push_back(m);
} // action::add_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a mark from the action.
 * \param m The mark to remove.
 *
 * The mark is removed from all snapshots.
 */
void bf::action::remove_mark( mark* m )
{
  CLAW_PRECOND( has_mark(m) );

  snapshot_list::iterator it;

  for (it=m_snapshots.begin(); it!=m_snapshots.end(); ++it)
    (*it)->remove_mark(m);

  mark_list::iterator it_m;
  bool ok = false;
  for ( it_m = m_marks.begin(); (it_m != m_marks.end()) && !ok;)
    if ( *it_m == m )
      ok = true;
    else
      ++it_m;

  m_marks.erase(it_m);
} // action::remove_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the action contains mark.
 */
bool bf::action::has_marks( ) const
{
  return !m_marks.empty();
} // action::has_marks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the action contains a mark.
 * \param m The mark to test.
 */
bool bf::action::has_mark( const mark* m ) const
{
  mark_list::const_iterator it_m;
  bool ok = false;

  for ( it_m = m_marks.begin(); (it_m != m_marks.end()) && !ok; ++it_m)
    if ( *it_m == m )
      ok = true;

  return ok;
} // action::has_mark()

/*----------------------------------------------------------------------------*/
bool bf::action::has_mark_label(const std::string& mark_label) const
{
  mark_list::const_iterator it;
  bool ok = false;

  for ( it = m_marks.begin(); (it != m_marks.end()) && !ok;)
    if ( (*it)->get_label() == mark_label )
      ok = true;
    else
      ++it;

  return ok;
} // action::has_mark_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a mark.
 * \param label The name of the mark to return.
 */
bf::mark* bf::action::get_mark( const std::string& label ) const
{
  CLAW_PRECOND( has_mark_label(label) );

  mark_list::const_iterator it_m;
  bool ok = false;

  for ( it_m = m_marks.begin(); (it_m != m_marks.end()) && !ok;)
    if ( (*it_m)->get_label() == label )
      ok = true;
    else
      ++it_m;

  return *it_m;
} // action::get_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a const iterator at the begin of the marks.
 */
bf::action::const_mark_iterator bf::action::mark_begin() const
{
  return m_marks.begin();
} // action::mark_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a const iterator on the end of the marks.
 */
bf::action::const_mark_iterator bf::action::mark_end() const
{
  return m_marks.end();
} // action::mark_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a const iterator at the begin of mark pointers list.
 */
bf::action::const_mark_ptr_iterator bf::action::mark_ptr_begin() const
{
  return m_marks.begin();
} // action::mark_ptr_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a const iterator on the end of mark pointers list.
 */
bf::action::const_mark_ptr_iterator bf::action::mark_ptr_end() const
{
  return m_marks.end();
} // action::mark_ptr_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the cumulated visibility of a mark in a time interval.
 * \param m The mark for which we want the visibility.
 * \param from The beginning of the time interval.
 * \param to The end of the time interval.
 */
double bf::action::accumulated_mark_visibility
( const mark* m, double from, double to ) const
{
  CLAW_PRECOND( m!=NULL );
  CLAW_PRECOND( from <= to );

  snapshot_list::const_iterator it( get_const_iterator_less_equal(from) );
  const snapshot_list::const_iterator to_s( get_const_iterator_less_equal(to) );

  if ( it == m_snapshots.end() )
    return 0;

  CLAW_PRECOND( to_s != m_snapshots.end() );

  double result;

  // The first iterator is before \a from. If the mark is visible, we
  // remove the time between the snapshot and \a from.
  if ( (*it)->get_placement(m).is_visible() )
    result = -(from - (*it)->get_date());
  else
    result = 0;

  // Then we sum the duration of the snapshots where the mark is
  // visible until the last snapshot.
  while ( it!=to_s )
    {
      snapshot_list::const_iterator n(it);
      ++n;

      if ( (*it)->get_placement(m).is_visible() )
        result += (*n)->get_date() - (*it)->get_date();

      it = n;
    }

  // Finally, we add the time during which the mark is visible during
  // the last snapshot
  if ( (*to_s)->get_placement(m).is_visible() )
    result += to - (*to_s)->get_date();

  return result;
} // action::accumulated_mark_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual bounds of this action, among all snapshots.
 * \param min_x The minimum x-coordinate with whether an animation or a mark or
 *              the bounding box.
 * \param min_y The minimum y-coordinate with whether an animation or a mark or
 *              the bounding box.
 * \param max_x The maximum x-coordinate with whether an animation or a mark or
 *              the bounding box.
 * \param max_y The maximum y-coordinate with whether an animation or a mark or
 *              the bounding box.
 */
void bf::action::get_bounds
( double& min_x, double& min_y, double& max_x, double& max_y ) const
{
  const_snapshot_iterator it;
  const const_snapshot_iterator eit( snapshot_end() );

  double snapshot_min_x;
  double snapshot_min_y;
  double snapshot_max_x;
  double snapshot_max_y;

  it=snapshot_begin();

  if ( it != eit )
    {
      it->get_bounds
        ( snapshot_min_x, snapshot_min_y, snapshot_max_x, snapshot_max_y );
      min_x = snapshot_min_x;
      min_y = snapshot_min_y;
      max_x = snapshot_max_x;
      max_y = snapshot_max_y;
    }
  else
    {
      min_x = 0;
      min_y = 0;
      max_x = 100;
      max_y = 100;
    }

  for (; it!=eit; ++it)
    {
      it->get_bounds
        ( snapshot_min_x, snapshot_min_y, snapshot_max_x, snapshot_max_y );

      min_x = std::min( min_x, snapshot_min_x );
      min_y = std::min( min_y, snapshot_min_y );
      max_x = std::max( max_x, snapshot_max_x );
      max_y = std::max( max_y, snapshot_max_y );
    }
} // action::get_bounds()

/*----------------------------------------------------------------------------*/
/**
 * \brief Rename the action.
 * \param name The new name of the action.
 */
void bf::action::set_name(const std::string& name)
{
  m_name = name;
} // action::set_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the action.
 */
const std::string&  bf::action::get_name() const
{
  return m_name;
} // action::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the action to start after this one.
 * \param n The name of the action.
 */
void bf::action::set_auto_next(const std::string& n)
{
  m_auto_next = n;
} // action::set_auto_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the action to start after this one.
 */
const std::string&  bf::action::get_auto_next() const
{
  return m_auto_next;
} // action::get_auto_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration of the action.
 * \param duration The new duration.
 */
void bf::action::set_duration(double duration)
{
  m_duration = duration;
} // action::set_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration of the action.
 */
double bf::action::get_duration() const
{
  return m_duration;
} // action::get_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sound played during the action.
 * \param s The sound played during the action.
 */
void bf::action::set_sound( const sound_description& s )
{
  m_sound = s;
} // action::set_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound played during the action.
 */
bf::sound_description& bf::action::get_sound()
{
  return m_sound;
} // action::get_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound played during the action.
 */
const bf::sound_description& bf::action::get_sound() const
{
  return m_sound;
} // action::get_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the action.
 * \param f The file in which we compile.
 * \param anim_ref The references to the animations common to several marks.
 */
void bf::action::compile
( compiled_file& f,
  const std::map<any_animation, std::size_t>& anim_ref ) const
{
  f << m_name << m_duration << m_auto_next;

  m_sound.compile(f);

  f << m_marks.size();

  // The index associated to each mark
  std::map<std::string, unsigned int> label_to_int;
  unsigned int i(0);

  const_mark_iterator itm;
  for (itm=m_marks.begin(); itm!=m_marks.end(); ++itm, ++i)
    {
      label_to_int[itm->get_label()] = i;
      itm->compile(f, anim_ref);
    }

  f << m_snapshots.size();

  const_snapshot_iterator its;
  for (its=m_snapshots.begin(); its!=m_snapshots.end(); ++its)
    its->compile(f, label_to_int);
} // action::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a snapshot at the date 0.
 */
void bf::action::create_initial_snapshot()
{
  snapshot* s = new snapshot();
  s->set_date(0);

  add_snapshot(s);
} // action::create_initial_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first snapshot with a date greater or equal to
 *        a given date.
 * \param d The minimum date of the snapshot searched.
 */
bf::action::snapshot_list::iterator
bf::action::get_iterator_greater_equal( double d )
{
  snapshot_list::iterator it( m_snapshots.begin() );
  bool found(false);

  while ( !found && (it!=m_snapshots.end()) )
    if ( (*it)->get_date() >= d )
      found = true;
    else
      ++it;

  return it;
} // action::get_iterator_greater_equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the first snapshot with a date greater or
 *        equal to a given date.
 * \param d The minimum date of the snapshot searched.
 */
bf::action::snapshot_list::const_iterator
bf::action::get_const_iterator_greater_equal( double d ) const
{
  snapshot_list::const_iterator it( m_snapshots.begin() );
  bool found(false);

  while ( !found && (it!=m_snapshots.end()) )
    if ( (*it)->get_date() >= d )
      found = true;
    else
      ++it;

  return it;
} // action::get_const_iterator_greater_equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first snapshot with a date less or equal to a
 *        given date.
 * \param d The maximum date of the snapshot searched.
 */
bf::action::snapshot_list::iterator
bf::action::get_iterator_less_equal( double d )
{
  snapshot_list::iterator it( m_snapshots.begin() );
  snapshot_list::iterator result( m_snapshots.end() );
  bool stop(false);

  while ( !stop && (it!=m_snapshots.end()) )
    if ( (*it)->get_date() > d )
      stop = true;
    else
      {
        result = it;
        ++it;
      }

  return result;
} // action::get_iterator_less_equal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the first snapshot with a date less or
 *        equal to a given date.
 * \param d The maximum date of the snapshot searched.
 */
bf::action::snapshot_list::const_iterator
bf::action::get_const_iterator_less_equal( double d ) const
{
  snapshot_list::const_iterator it( m_snapshots.begin() );
  snapshot_list::const_iterator result( m_snapshots.end() );
  bool stop(false);

  while ( !stop && (it!=m_snapshots.end()) )
    if ( (*it)->get_date() > d )
      stop = true;
    else
      {
        result = it;
        ++it;
      }

  return result;
} // action::get_const_iterator_less_equal()

