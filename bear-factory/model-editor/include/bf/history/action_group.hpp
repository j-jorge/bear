/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An action made of sub actions.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_GROUP_HPP__
#define __BF_ACTION_GROUP_HPP__

#include "bf/history/model_action.hpp"

#include <list>

namespace bf
{
  /**
   * \brief The action made of sub actions.
   * \author Julien Jorge
   */
  class action_group:
    public model_action
  {
  private:
    /** \brief The type of the collection of sub actions. */
    typedef std::list<model_action*> action_collection;

  public:
    action_group();
    ~action_group();

    void set_description( const wxString& description );

    void add_action( model_action* a );

    void execute( gui_model& lvl );
    void undo( gui_model& lvl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The sub actions. */
    action_collection m_actions;

    /** \brief The descriptions of the group. */
    wxString m_description;

  }; // class action_group
} // namespace bf

#endif // __BF_ACTION_GROUP_HPP__
