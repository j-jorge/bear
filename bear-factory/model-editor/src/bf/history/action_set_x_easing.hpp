/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of modifying the easing function for the x-position.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_X_EASING_HPP__
#define __BF_ACTION_SET_X_EASING_HPP__

#include "bf/history/model_action.hpp"

#include "easing.hpp"

namespace bf
{
  class snapshot;
  class mark;

  /**
   * \brief The action of modifying the easing function for the x-position.
   * \author Julien Jorge
   */
  class action_set_x_easing:
    public model_action
  {
  public:
    action_set_x_easing( snapshot* s, const mark* m, const bear::easing& e );

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The snapshot containing the mark. */
    snapshot* m_snapshot;

    /** \brief The considered mark. */
    const mark* m_mark;

    /** \brief The new easing. */
    bear::easing m_easing;

  }; // class action_set_x_easing
} // namespace bf

#endif // __BF_ACTION_SET_X_EASING_HPP__
