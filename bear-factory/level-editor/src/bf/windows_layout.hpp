/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that groups all windows common to all levels.
 * \author Julien Jorge
 */
#ifndef __BF_WINDOWS_LAYOUT_HPP__
#define __BF_WINDOWS_LAYOUT_HPP__

#include "bf/level.hpp"

namespace bf
{
  class main_frame;
  class properties_frame;
  class layer_list_frame;
  class ingame_view_frame;

  /**
   * \brief A class that groups all windows common to all levels.
   * \author Julien Jorge
   */
  class windows_layout
  {
  public:
    typedef claw::wrapped_iterator
    < ingame_view_frame,
      std::set<ingame_view_frame*>::iterator,
      claw::dereference<ingame_view_frame>
    >::iterator_type iterator;

  public:
    windows_layout
    ( main_frame& mf, properties_frame& pf, layer_list_frame& llf );

    main_frame& get_main_frame();
    properties_frame& get_properties_frame();
    layer_list_frame& get_layer_list_frame();

    ingame_view_frame* get_current_level_view();
    bool set_current_level_view( ingame_view_frame& view );
    void add_level_view( ingame_view_frame& view );
    void remove_level_view( ingame_view_frame& view );

    iterator begin();
    iterator end();

  private:
    /** \brief The main window of the program. */
    main_frame& m_main_frame;

    /** \brief The window displaying the properties of an item. */
    properties_frame& m_properties_frame;

    /** \brief The list of the layers in the current level. */
    layer_list_frame& m_layer_list;

    /** \brief All level windows. */
    std::set<ingame_view_frame*> m_level_view;

    /** \brief Active level window. */
    ingame_view_frame* m_current_level_view;

  }; // class windows_layout
} // namespace bf

#endif // __BF_WINDOWS_LAYOUT_HPP__
