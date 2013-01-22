/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of adding mark in an action.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_ADD_MARK_HPP__
#define __BF_ACTION_ADD_MARK_HPP__

#include "bf/history/model_action.hpp"

#include "bf/mark.hpp"

namespace bf
{
  class action;

  /**
   * \brief The action of adding a mark in an action.
   * \author Sébastien Angibaud
   */
  class action_add_mark:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param action_name The current action.
     * \param m The new mark.
     */
    action_add_mark( const std::string& action_name, mark* m);

    /** \brief Destructor. */
    ~action_add_mark();

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The current action. */
    const std::string m_action_name;

    /** \brief The lmark to add. */
    mark* m_mark;

    /** \brief Tell if the action has been done or not. */
    bool m_done;
  }; // class action_add_mark
} // namespace bf

#endif // __BF_ACTION_ADD_MARK_HPP__
