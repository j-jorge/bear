/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for the actions on a model, that can be undone.
 * \author Sébastien Angibaud
 */
#ifndef __BF_MODEL_ACTION_HPP__
#define __BF_MODEL_ACTION_HPP__

#include <claw/non_copyable.hpp>
#include <wx/string.h>

namespace bf
{
  class gui_model;

  /**
   * \brief Base class for the actions on a model, that can be undone.
   * \author Sébastien Angibaud
   */
  class model_action
    : public claw::pattern::non_copyable
  {
  public:
    /** \brief Destructor. */
    virtual ~model_action() { }

    /**
     * \brief Apply the action to a model.
     * \param mdl The model to which the action is applied.
     */
    virtual void execute( gui_model& mdl ) = 0;

    /**
     * \brief Undo the action.
     * \param mdl The model in which the action is undone.
     */
    virtual void undo( gui_model& mdl ) = 0;

    /** \brief Tell if the action does nothing. */
    virtual bool is_identity( const gui_model& gui ) const = 0;

    /** \brief Get a short description of the action. */
    virtual wxString get_description() const = 0;

  }; // class model_action
} // namespace bf

#endif // __BF_MODEL_ACTION_HPP__
