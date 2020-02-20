/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A model is a set of actions, and gives default values for some fields.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_HPP__
#define __BF_MODEL_HPP__

#include "bf/action.hpp"

#include <claw/functional.hpp>
#include <claw/non_copyable.hpp>
#include <claw/iterator.hpp>
#include <list>
#include <string>

namespace bf
{
  class compilation_context;
  class compiled_file;

  /**
   * \brief A model is a set of actions, and gives default values for some
   *        fields.
   * \author Julien Jorge
   */
  class model:
    public claw::pattern::non_copyable
  {
  private:
    /** \brief The structure in which we store the actions. */
    typedef std::list<action*> action_list;

  public:
    typedef claw::wrapped_iterator
    < const action,
      action_list::const_iterator,
      claw::const_dereference<action> >::iterator_type const_action_iterator;

  public:
    model();
    ~model();

    bool empty() const;
    bool has_action( const std::string& name ) const;

    std::string create_unique_action_name( const std::string& prefix ) const;

    action& add_action
    ( const std::string& action_name, double duration,
      const sound_description& sound,
      const std::string& next_action);
    void add_action( action* a );
    action* remove_action( const std::string& name );

    action& get_action( const std::string& name );
    const action& get_action( const std::string& name ) const;

    void compile( compiled_file& f, compilation_context& c ) const;

    const_action_iterator action_begin() const;
    const_action_iterator action_end() const;

  private:
    /** \brief The actions this model can do. */
    action_list m_actions;

  }; // class model
} // namespace bf

#endif // __BF_MODEL_HPP__
