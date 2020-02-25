/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of modifying the function of a mark placement.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_SET_PLACEMENT_FUNCTION_HPP__
#define __BF_ACTION_SET_PLACEMENT_FUNCTION_HPP__

#include "bf/history/model_action.hpp"

namespace bf
{
  class snapshot;
  class mark;

  /**
   * \brief The action of modifying the function of a mark placement.
   * \author Julien Jorge
   */
  class action_set_placement_function:
    public model_action
  {
  public:
    action_set_placement_function
    ( snapshot* s, const mark* m, const std::string& f );

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The snapshot containing the mark. */
    snapshot* m_snapshot;

    /** \brief The considered mark. */
    const mark* m_mark;

    /** \brief The new function of the mark. */
    std::string m_function;

  }; // class action_set_placement_function
} // namespace bf

#endif // __BF_ACTION_SET_PLACEMENT_FUNCTION_HPP__
