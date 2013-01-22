/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that groups all windows common to all models.
 * \author Julien Jorge
 */
#ifndef __BF_WINDOWS_LAYOUT_HPP__
#define __BF_WINDOWS_LAYOUT_HPP__

#include "bf/model.hpp"

#include <set>

class wxWindow;

namespace bf
{
  class main_frame;
  class model_frame;

  /**
   * \brief A class that groups all windows common to all models.
   * \author Julien Jorge
   */
  class windows_layout
  {
  public:
    typedef claw::wrapped_iterator
    < model_frame,
      std::set<model_frame*>::iterator,
      claw::dereference<model_frame>
    >::iterator_type iterator;

  public:
    windows_layout( main_frame& mf );

    wxWindow* get_root_window() const;

    model_frame* get_current_model_frame();
    bool set_current_model_frame( model_frame& m );
    void add_model_frame( model_frame& m );
    void remove_model_frame( model_frame& m );

    void update_action();
    void update_snapshot();
    void update_mark();

    iterator begin();
    iterator end();

  private:
    /** \brief The main window of the program. */
    main_frame& m_main_frame;

    /** \brief All model windows. */
    std::set<model_frame*> m_model_frame;

    /** \brief Active model window. */
    model_frame* m_current_model_frame;

  }; // class windows_layout
} // namespace bf

#endif // __BF_WINDOWS_LAYOUT_HPP__
