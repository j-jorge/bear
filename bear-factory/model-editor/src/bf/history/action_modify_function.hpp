/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of modify the function called in a snapshot.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_FUNCTION_HPP__
#define __BF_ACTION_MODIFY_FUNCTION_HPP__

#include "bf/history/model_action.hpp"
#include "bf/any_animation_edit.hpp"

#include <string>

namespace bf
{
  class snapshot;

  /**
   * \brief The action of modify the function called in a snapshot.
   * \author Sébastien Angibaud
   */
  class action_modify_function:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param s The snapshot to consider.
     * \param function The new function of the snapshot.
     */
    action_modify_function
    ( snapshot* s, const std::string& function);

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

    /** \brief The new function of the snapshot. */
    std::string m_function;
  }; // class action_modify_function
} // namespace bf

#endif // __BF_ACTION_MODIFY_FUNCTION_HPP__
