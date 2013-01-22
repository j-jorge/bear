/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of removing action from a model.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_REMOVE_ACTION_HPP__
#define __BF_ACTION_REMOVE_ACTION_HPP__

#include "bf/history/model_action.hpp"
#include <string>

namespace bf
{
  class action;

  /**
   * \brief The action of removing a action from a model.
   * \author Sébastien Angibaud
   */
  class action_remove_action:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param action_name The name of the action to remove.
     */
    action_remove_action( const std::string& action_name );

    /** \brief Destructor. */
    ~action_remove_action();

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The action removed. */
    action* m_action;

    /** \brief The name of the action in the model. */
    const std::string m_action_name;

  }; // class action_remove_action
} // namespace bf

#endif // __BF_ACTION_REMOVE_ACTION_HPP__
