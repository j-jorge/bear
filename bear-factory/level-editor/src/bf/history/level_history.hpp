/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to manage undo/redo informations on a level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_HISTORY_HPP__
#define __BF_LEVEL_HISTORY_HPP__

#include "bf/item_selection.hpp"

#include <wx/string.h>
#include <list>

namespace bf
{
  class action_move_selection;
  class gui_level;
  class level_action;

  /**
   * \brief A class to manage undo/redo informations on a level.
   * \author Julien Jorge
   */
  class level_history
  {
  public:
    level_history( gui_level* lvl );
    ~level_history();

    bool can_undo() const;
    bool can_redo() const;

    void undo();
    void redo();
    void revert();

    bool do_action( level_action* action );
    bool do_action( action_move_selection* action );

    wxString get_undo_description() const;
    wxString get_redo_description() const;

    void set_saved();
    bool level_is_saved() const;
    void set_compiled();
    bool level_is_compiled() const;

    gui_level& get_level();
    const gui_level& get_level() const;

  private:
    void clear_past();
    void clear_future();

    void push_action( level_action* a );

  private:
    /** \brief The level on which we are working. */
    gui_level* m_level;

    /** \brief The actions done in the past. */
    std::list<level_action*> m_past;

    /** \brief The actions in the future. */
    std::list<level_action*> m_future;

    /** \brief Maximum size of the undo history. */
    std::size_t m_max_history;

    /** \brief The last action done before saving. */
    level_action* m_saved_action;

    /** \brief The last action done before compiling. */
    level_action* m_compiled_action;

    /** \brief The last action of moving the selection, used to merge successive
        little moves. */
    action_move_selection* m_last_selection_move;

    /** \brief The date of the last movement of the selection. */
    time_t m_last_selection_move_date;

    /** \brief The selection moved by m_last_selection_move. */
    item_selection m_last_selection_move_items;

  }; // class level_history
} // namespace bf

#endif // __BF_LEVEL_HISTORY_HPP__
