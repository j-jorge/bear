/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of adding action in a model.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_ADD_ACTION_HPP__
#define __BF_ACTION_ADD_ACTION_HPP__

#include "bf/history/model_action.hpp"
#include <string>

namespace bf
{
  class action;

  /**
   * \brief The action of adding a action in a model.
   * \author Sébastien Angibaud
   */
  class action_add_action:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param a The action to add.
     */
    action_add_action( action* a );

    /** \brief Destructor. */
    ~action_add_action();

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The action to add. */
    action* m_action;

    /** \brief The name of the action. */
    std::string m_action_name;

  }; // class action_add_action
} // namespace bf

#endif // __BF_ACTION_ADD_ACTION_HPP__
