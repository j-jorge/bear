/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An action the model can do.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_HPP__
#define __BF_ACTION_HPP__

#include "bf/sound_description.hpp"

#include <claw/iterator.hpp>
#include <claw/functional.hpp>
#include <list>
#include <map>

namespace bf
{
  class any_animation;
  class compiled_file;
  class mark;
  class snapshot;

  /**
   * \brief An action the model can do.
   * \author Julien Jorge
   */
  class action
  {
  private:
    /** \brief The type of the list of marks. */
    typedef std::list<mark*> mark_list;

    /** \brief The list of snapshots. */
    typedef std::list<snapshot*> snapshot_list;

  public:
    /** \brief The type of const iterator on the marks. */
    typedef claw::wrapped_iterator
    < const mark,
      mark_list::const_iterator,
      claw::const_dereference<mark> >
      ::iterator_type const_mark_iterator;

    /** \brief Iterator on the marks that returns a pointer (used as an id). */
    typedef mark_list::const_iterator const_mark_ptr_iterator;

    /** \brief The type of the iterators on the snapshots. */
    typedef claw::wrapped_iterator
    < snapshot,
      snapshot_list::iterator,
      claw::dereference<snapshot> >
      ::iterator_type snapshot_iterator;

    /** \brief The type of const iterators on the snapshots. */
    typedef claw::wrapped_iterator
    < const snapshot,
      snapshot_list::const_iterator,
      claw::const_dereference<snapshot> >
      ::iterator_type const_snapshot_iterator;

  public:
    action(const std::string& action_name, double duration,
           const sound_description& sound,
           const std::string& auto_next);
    action(const std::string& action_name, double duration);
    action( const action& that );

    ~action();

    snapshot* create_snapshot() const;
    void init_snapshot( snapshot& s ) const;

    void add_snapshot( snapshot* s );
    void remove_snapshot( snapshot* s );
    bool has_snapshot( const snapshot* s ) const;
    bool has_snapshot_date( double date ) const;
    bool has_greater_date(double date) const;

    const snapshot* get_snapshot_before_date( double date ) const;

    snapshot* get_snapshot_before_or_at_date( double date );
    const snapshot* get_snapshot_before_or_at_date( double date ) const;

    const snapshot* get_snapshot_after_date( double date ) const;

    snapshot* get_snapshot_after_or_at_date(double date);
    const snapshot* get_snapshot_after_or_at_date(double date) const;

    void get_all_snapshots_after_or_at_date
    ( double date, snapshot_list& snapshots ) const;

    const_snapshot_iterator snapshot_begin() const;
    const_snapshot_iterator snapshot_end() const;

    snapshot_iterator snapshot_begin();
    snapshot_iterator snapshot_end();

    void add_mark(mark* m);
    void remove_mark( mark* m );
    bool has_marks() const;
    bool has_mark( const mark* m ) const;
    bool has_mark_label(const std::string& mark_label) const;
    mark* get_mark(const std::string& label) const;

    const_mark_iterator mark_begin() const;
    const_mark_iterator mark_end() const;
    const_mark_ptr_iterator mark_ptr_begin() const;
    const_mark_ptr_iterator mark_ptr_end() const;

    double accumulated_mark_visibility
    ( const mark* m, double from, double to ) const;

    void get_bounds
    ( double& min_x, double& min_y, double& max_x, double& max_y ) const;

    void set_name(const std::string& name);
    const std::string& get_name() const;

    void set_auto_next(const std::string& n);
    const std::string& get_auto_next() const;

    void set_duration(double duration);
    double get_duration() const;

    void set_sound( const sound_description& s );
    sound_description& get_sound();
    const sound_description& get_sound() const;

    void compile
    ( compiled_file& f,
      const std::map<any_animation, std::size_t>& anim_ref ) const;

  private:
    void create_initial_snapshot();

    snapshot_list::iterator get_iterator_greater_equal( double d );
    snapshot_list::const_iterator
    get_const_iterator_greater_equal( double d ) const;

    snapshot_list::iterator get_iterator_less_equal( double d );
    snapshot_list::const_iterator
    get_const_iterator_less_equal( double d ) const;

  private:
    /** \brief The snapshots of the marks. */
    snapshot_list m_snapshots;

    /** \brief All the marks of the action. */
    mark_list m_marks;

    /** \brief The name of the action. */
    std::string m_name;

    /** \brief The duration of the action. */
    double m_duration;

    /** \brief The sound played during the action. */
    sound_description m_sound;

    /** \brief The name of the action to start when this one is finished. */
    std::string m_auto_next;

  }; // class action
} // namespace bf

#endif // __BF_ACTION_HPP__
