/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of modify an action.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_ACTION_HPP__
#define __BF_ACTION_MODIFY_ACTION_HPP__

#include "bf/history/model_action.hpp"
#include "bf/snapshot.hpp"

#include <string>

namespace bf
{
  class action;
  class sound_description;

  /**
   * \brief The action of modify an action.
   * \author Sébastien Angibaud
   */
  class action_modify_action:
    public model_action
  {
  public:
    action_modify_action
    ( action* a, const std::string& name,
      double duration, const sound_description& sound,
      const std::string& auto_next);

    ~action_modify_action();

    /**
     * \remark Calling execute() two times will restore the initial size.
     */
    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The action to modify. */
    action* m_action;

    /** \brief The new name of the action. */
    std::string m_name;

    /** \brief The new duration of the action. */
    double m_duration;

    /** \brief The sound played during the action. */
    sound_description m_sound;

    /** \brief The name of the action to start when this one is finished. */
    std::string m_auto_next;

    /** \brief List of snapshots that are delete by changing the duration. */
    std::list<snapshot*> m_snapshots;
  }; // class action_modify_action
} // namespace bf

#endif // __BF_ACTION_MODIFY_ACTION_HPP__
