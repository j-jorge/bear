/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of removing a snapshot from an actionl.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_REMOVE_SNAPSHOT_HPP__
#define __BF_ACTION_REMOVE_SNAPSHOT_HPP__

#include "bf/history/model_action.hpp"
#include "bf/snapshot.hpp"

namespace bf
{
  class action;

  /**
   * \brief The action of removing a snapshot from an action.
   * \author Sébastien Angibaud
   */
  class action_remove_snapshot:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param action_name The name of the action containing the snapshot.
     * \param s The snapshot to remove.
    */
    action_remove_snapshot( const std::string& action_name, snapshot* s );

    /** \brief Destructor. */
    ~action_remove_snapshot();

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The snapshot removed. */
    snapshot* m_snapshot;

    /** \brief The name of the action in the model. */
    const std::string m_action_name;

    /** \brief Tell if the action has been done or not. */
    bool m_done;
  }; // class action_remove_snapshot
} // namespace bf

#endif // __BF_ACTION_REMOVE_SNAPSHOT_HPP__
