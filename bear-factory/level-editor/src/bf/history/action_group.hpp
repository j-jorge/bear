/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An action made of sub actions.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_GROUP_HPP__
#define __BF_ACTION_GROUP_HPP__

#include "bf/history/level_action.hpp"

#include <list>
#include <wx/intl.h>

namespace bf
{
  /**
   * \brief The action made of sub actions.
   * \author Julien Jorge
   */
  class action_group:
    public level_action
  {
  private:
    /** \brief The type of the collection of sub actions. */
    typedef std::list<level_action*> action_collection;

  public:
    action_group( const wxString& desc = _("Group of actions") );
    ~action_group();

    void add_action( level_action* a );
    void move( action_group& g );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief A description of the actions in the group. */
    const wxString m_description;

    /** \brief The sub actions. */
    action_collection m_actions;

  }; // class action_group
} // namespace bf

#endif // __BF_ACTION_GROUP_HPP__
