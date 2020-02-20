/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to manage undo/redo informations on a model.
 * \author Sébastien Angibaud
 */
#ifndef __BF_MODEL_HISTORY_HPP__
#define __BF_MODEL_HISTORY_HPP__

#include <wx/string.h>
#include <list>

namespace bf
{
  class gui_model;
  class model_action;

  /**
   * \brief A class to manage undo/redo informations on a model.
   * \author Sébastien Angibaud
   */
  class model_history
  {
  public:
    model_history( gui_model* mdl );
    ~model_history();

    bool can_undo() const;
    bool can_redo() const;

    void undo();
    void redo();

    bool do_action( model_action* action );

    wxString get_undo_description() const;
    wxString get_redo_description() const;

    void set_saved();
    bool model_is_saved() const;

    gui_model& get_model();
    const gui_model& get_model() const;

  private:
    void clear_past();
    void clear_future();

  private:
    /** \brief The model on which we are working. */
    gui_model* m_model;

    /** \brief The actions done in the past. */
    std::list<model_action*> m_past;

    /** \brief The actions in the future. */
    std::list<model_action*> m_future;

    /** \brief Maximum size of the undo history. */
    std::size_t m_max_history;

    /** \brief The last action done before saving. */
    model_action* m_saved_action;

  }; // class model_history
} // namespace bf

#endif // __BF_MODEL_HISTORY_HPP__
