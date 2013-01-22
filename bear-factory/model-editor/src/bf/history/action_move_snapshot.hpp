/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of move a snapshot.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MOVE_SNAPSHOT_HPP__
#define __BF_ACTION_MOVE_SNAPSHOT_HPP__

#include "bf/history/model_action.hpp"

#include <string>

namespace bf
{
  class snapshot;

  /**
   * \brief The action of moving an snapshot.
   * \author Sébastien Angibaud
   */
  class action_move_snapshot:
    public model_action
  {
  public:
    action_move_snapshot( snapshot* s, double date );

    /**
     * \remark Calling execute() two times will restore the initial position.
     */
    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The snapshot to modify. */
    snapshot* m_snapshot;

    /** \brief The new date of the snapshot. */
    double m_date;
  }; // class action_move_snapshot
} // namespace bf

#endif // __BF_ACTION_MOVE_SNAPSHOT_HPP__
