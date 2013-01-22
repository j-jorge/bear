/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of removing a mark from an actionl.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_REMOVE_MARK_HPP__
#define __BF_ACTION_REMOVE_MARK_HPP__

#include "bf/history/model_action.hpp"
#include "bf/mark.hpp"

#include <list>

namespace bf
{
  class action;
  class mark_placement;

  /**
   * \brief The action of removing a mark from an action.
   * \author Sébastien Angibaud
   */
  class action_remove_mark:
    public model_action
  {
  public:
    /**
     * \brief Constructor.
     * \param action_name The name of the action containing the mark to remove.
     * \param m The mark to remove.
    */
    action_remove_mark( const std::string& action_name, mark* m );

    /** \brief Destructor. */
    ~action_remove_mark();

    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The mark removed. */
    mark* m_mark;

    /** \brief The name of the action in the model. */
    const std::string m_action_name;

    /** \brief Tell if the action has been done or not. */
    bool m_done;

    /** \brief The placement of the mark in each snapshot. */
    std::list< std::pair<double, mark_placement> > m_placements;
  }; // class action_remove_mark
} // namespace bf

#endif // __BF_ACTION_REMOVE_MARK_HPP__
