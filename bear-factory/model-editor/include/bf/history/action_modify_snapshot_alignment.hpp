/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of modify the alignment of the snpashot.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_SNAPSHOT_ALIGNMENT_HPP__
#define __BF_ACTION_MODIFY_SNAPSHOT_ALIGNMENT_HPP__

#include "bf/history/model_action.hpp"
#include "bf/snapshot.hpp"

#include <string>

namespace bf
{
  class snapshot;
  class alignment_description;

  /**
   * \brief The action of modify the alignment of a snapshot.
   * \author Sébastien Angibaud
   */
  class action_modify_snapshot_alignment:
    public model_action
  {
  public:
    action_modify_snapshot_alignment
    ( snapshot* s, const std::string& x_alignment, double x_value,
      const std::string& y_alignment, double y_value );

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

    /** \brief The alignment on x-coordinate. */
    std::string m_x_alignment;

    /** \brief The alignment on x-coordinate. */
    double m_x_alignment_value;

    /** \brief The alignment on y-coordinate. */
    std::string m_y_alignment;

    /** \brief The alignment on y-coordinate. */
    double m_y_alignment_value;
  }; // class action_modify_snapshot_alignment
} // namespace bf

#endif // __BF_ACTION_MODIFY_SNAPSHOT_ALIGNMENT_HPP__
