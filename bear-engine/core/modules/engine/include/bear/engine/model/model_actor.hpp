/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A model is a group of animations and sounds.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_ACTOR_HPP__
#define __ENGINE_MODEL_ACTOR_HPP__

#include "bear/engine/model/model_animation.hpp"
#include "bear/engine/class_export.hpp"

#include <map>
#include <string>
#include <claw/iterator.hpp>
#include <claw/functional.hpp>

namespace bear
{
  namespace engine
  {
    class model_action;

    /**
     * \brief A model is a group of animations and sounds.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_actor
    {
    private:
      /** \brief A map of actions. */
      typedef std::map<std::string, model_action*> action_map;

    public:
      /** \brief Iterators on the actions of the actor. */
      typedef claw::wrapped_iterator
      < const model_action,
        action_map::const_iterator,
        claw::unary_compose
        < claw::const_dereference<model_action>,
          claw::const_pair_second<action_map::value_type> > >
      ::iterator_type const_action_iterator;

    public:
      model_actor();
      model_actor( const model_actor& that );
      ~model_actor();

      model_actor& operator=( model_actor that );
      void swap( model_actor& that ) throw();

      model_action* get_action( const std::string& action_name ) const;
      void add_action( const std::string& name, const model_action& a );

      void set_global_substitute
      ( const std::string& mark_name, const model_animation& anim );
      void remove_global_substitute( const std::string& mark_name );

      const_action_iterator action_begin() const;
      const_action_iterator action_end() const;

    private:
      /** \brief The actions. */
      action_map m_actions;

    }; // class model_actor

    void swap( engine::model_actor& a, engine::model_actor& b ) throw();
  } // namespace engine
} // namespace bear

namespace std
{
  template<>
  void swap( bear::engine::model_actor& a, bear::engine::model_actor& b );
} // namespace std

#endif // __ENGINE_MODEL_ACTOR_HPP__
