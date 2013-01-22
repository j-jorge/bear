/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of adding snapshot in an action.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_ADD_SNAPSHOT_HPP__
#define __BF_ACTION_ADD_SNAPSHOT_HPP__

#include "bf/history/model_action.hpp"

#include "bf/snapshot.hpp"

namespace bf
{
  class action;

  /**
   * \brief The action of adding a snapshot in an action.
   * \author Sébastien Angibaud
   */
  class action_add_snapshot:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param action_name The current action.
     * \param s The new snapshot.
     */
    action_add_snapshot( const std::string& action_name, snapshot* s);

    /** \brief Destructor. */
    ~action_add_snapshot();

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The current action. */
    const std::string m_action_name;

    /** \brief The lsnapshot to add. */
    snapshot* m_snapshot;

    /** \brief Tell if the action has been done or not. */
    bool m_done;
  }; // class action_add_snapshot
} // namespace bf

#endif // __BF_ACTION_ADD_SNAPSHOT_HPP__
