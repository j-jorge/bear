/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An action of a model.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_ACTION_HPP__
#define __ENGINE_MODEL_ACTION_HPP__

#include "engine/model/model_mark.hpp"

#include "engine/class_export.hpp"

#include <map>
#include <claw/iterator.hpp>
#include <claw/functional.hpp>

namespace bear
{
  namespace engine
  {
    class model_snapshot;

    /**
     * \brief An action of a model.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_action
    {
    private:
      /** \brief The type of a map of snapshots. */
      typedef std::map<universe::time_type, model_snapshot*> snapshot_map;

    public:
      /** \brief Iterators on the snapshots of the action. */
      typedef claw::wrapped_iterator
      < const model_snapshot,
        snapshot_map::const_iterator,
        claw::unary_compose
        < claw::const_dereference<model_snapshot>,
          claw::const_pair_second<snapshot_map::value_type> > >
      ::iterator_type const_snapshot_iterator;

      /** \brief Iterators on the snapshots of the action. */
      typedef claw::wrapped_iterator
      < model_snapshot,
        snapshot_map::iterator,
        claw::unary_compose
        < claw::dereference<model_snapshot>,
          claw::pair_second<snapshot_map::value_type> > >
      ::iterator_type snapshot_iterator;

      /** \brief Iterators on the marks of the action. */
      typedef claw::wrapped_iterator
      < model_mark,
        std::vector<model_mark*>::const_iterator,
        claw::dereference<model_mark> >
      ::iterator_type mark_iterator;

    public:
      model_action();
      model_action
      ( std::size_t n, universe::time_type dur,
        const std::string& next,
        const std::string& snd, bool glob );
      model_action( const model_action& that );
      ~model_action();

      model_action& operator=( model_action that );
      void swap( model_action& that ) throw();

      model_mark& get_mark( std::size_t i ) const;
      model_mark& get_mark( const std::string& n ) const;
      std::size_t get_mark_id( const std::string& n ) const;
      std::size_t get_marks_count() const;

      universe::time_type get_duration() const;
      void add_snapshot( const model_snapshot& s );

      const_snapshot_iterator get_snapshot_at( universe::time_type t ) const;
      const_snapshot_iterator snapshot_begin() const;
      const_snapshot_iterator snapshot_end() const;
      snapshot_iterator snapshot_begin();
      snapshot_iterator snapshot_end();

      const std::string& get_next_action() const;
      const std::string& get_sound_name() const;
      bool sound_is_global() const;

      void get_max_size( double& width, double& height ) const;

      mark_iterator mark_begin() const;
      mark_iterator mark_end() const;

      universe::time_type accumulated_mark_visibility
        ( const model_mark& m, universe::time_type from,
          universe::time_type to ) const;

    private:
      snapshot_map::const_iterator
        get_snapshot_const_iterator_at( universe::time_type t ) const;

    public:
      /** \brief An invalid mark identifier. */
      static const std::size_t not_an_id;

    private:
      /** \brief The marks in the action. */
      std::vector<model_mark*> m_mark;

      /** \brief The snapshots in the action. */
      snapshot_map m_snapshot;

      /** \brief The duration of the action. */
      universe::time_type m_duration;

      /** \brief The action to play after this one. */
      std::string m_next;

      /** \brief The sound to play during this action. */
      std::string m_sound_name;

      /** \brief Tell if the sound is played globally. */
      bool m_sound_is_global;

    }; // class model_action

    void swap( engine::model_action& a, engine::model_action& b ) throw();
  } // namespace engine
} // namespace bear

namespace std
{
  template<>
  void swap( bear::engine::model_action& a, bear::engine::model_action& b );
} // namespace std

#endif // __ENGINE_MODEL_ACTION_HPP__
