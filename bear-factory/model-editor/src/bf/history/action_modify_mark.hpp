/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of modify a mark.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_MARK_HPP__
#define __BF_ACTION_MODIFY_MARK_HPP__

#include "bf/history/model_action.hpp"
#include "bf/any_animation_edit.hpp"

#include <string>

namespace bf
{
  class mark;

  /**
   * \brief The action of modify an mark.
   * \author Sébastien Angibaud
   */
  class action_modify_mark:
    public model_action
  {
  public:
    action_modify_mark
    ( mark* m, const std::string& label, const bf::any_animation& animation,
      bool apply_angle_to_animation, bool pause_hidden,
      bool reset_with_action );

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The mark to modify. */
    mark* m_mark;

    /** \brief The new label of the mark. */
    std::string m_label;

    /** \brief The new animation of the mark. */
    bf::any_animation m_animation;

    /** \brief Tells if the angle applied to the mark is also applied to the
        animation. */
    bool m_apply_angle_to_animation;

    /** \brief Tells if the animation must be paused when the mark is hidden. */
    bool m_pause_animation_when_hidden;

    /** \brief Tells if the animation must be reset when the action starts. */
    bool m_reset_animation_with_action;

  }; // class action_modify_mark
} // namespace bf

#endif // __BF_ACTION_MODIFY_MARK_HPP__
