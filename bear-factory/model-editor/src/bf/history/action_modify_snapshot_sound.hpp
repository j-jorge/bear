/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of modify the sound of the snpashot.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_SNAPSHOT_SOUND_HPP__
#define __BF_ACTION_MODIFY_SNAPSHOT_SOUND_HPP__

#include "bf/history/model_action.hpp"
#include "bf/snapshot.hpp"

#include <string>

namespace bf
{
  class snapshot;
  class sound_description;

  /**
   * \brief The action of modify the sound of a snapshot.
   * \author Sébastien Angibaud
   */
  class action_modify_snapshot_sound:
    public model_action
  {
  public:
    action_modify_snapshot_sound
    ( snapshot* s, const sound_description& sound );

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

    /** \brief The sound played during the snapshot. */
    sound_description m_sound;
  }; // class action_modify_snapshot_sound
} // namespace bf

#endif // __BF_ACTION_MODIFY_SNAPSHOT_SOUND_HPP__
