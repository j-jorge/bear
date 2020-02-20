/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for the actions on a level, that can be undone.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_ACTION_HPP__
#define __BF_LEVEL_ACTION_HPP__

#include <claw/non_copyable.hpp>
#include <wx/string.h>

namespace bf
{
  class gui_level;

  /**
   * \brief Base class for the actions on a level, that can be undone.
   * \author Julien Jorge
   */
  class level_action
    : public claw::pattern::non_copyable
  {
  public:
    /** \brief Destructor. */
    virtual ~level_action() { }

    /**
     * \brief Apply the action to a level.
     * \param lvl The level to which the action is applied.
     */
    virtual void execute( gui_level& lvl ) = 0;

    /**
     * \brief Undo the action.
     * \param lvl The level in which the action is undone.
     */
    virtual void undo( gui_level& lvl ) = 0;

    /** \brief Tell if the action does nothing. */
    virtual bool is_identity( const gui_level& gui ) const = 0;

    /** \brief Get a short description of the action. */
    virtual wxString get_description() const = 0;

  }; // class level_action
} // namespace bf

#endif // __BF_LEVEL_ACTION_HPP__
