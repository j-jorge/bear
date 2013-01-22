/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of modify the size of the snpashot.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_SNAPSHOT_SIZE_HPP__
#define __BF_ACTION_MODIFY_SNAPSHOT_SIZE_HPP__

#include "bf/history/model_action.hpp"
#include "bf/snapshot.hpp"

#include <string>

namespace bf
{
  class snapshot;

  /**
   * \brief The action of modify the size of a snapshot.
   * \author Sébastien Angibaud
   */
  class action_modify_snapshot_size:
    public model_action
  {
  public:
    action_modify_snapshot_size
    ( snapshot* s, const double width, const double heigth );

    /**
     * \remark Calling execute() two times will restore the initial size.
     */
    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The snapshot to modify. */
    snapshot* m_snapshot;

    /** \brief The width of the snapshot. */
    double m_width;

    /** \brief The height of the snapshot. */
    double m_height;
  }; // class action_modify_snapshot_size
} // namespace bf

#endif // __BF_ACTION_MODIFY_SNAPSHOT_SIZE_HPP__
