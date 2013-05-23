/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The selection manager manages the selected items in the level
 *        according to the policy of an edit_mode.
 * \author Julien Jorge
 */
#ifndef __BF_SELECTION_MANAGER_HPP__
#define __BF_SELECTION_MANAGER_HPP__

#include "bf/edit_mode.hpp"

namespace bf
{
  class gui_level;
  class properties_frame;

  /**
   * \brief The selection manager manages the selected items in the level
   *        according to the policy of an edit_mode.
   * \author Julien Jorge
   */
  class selection_manager
  {
  public:
    selection_manager( gui_level& lvl, properties_frame& properties );

    edit_mode get_edit_mode() const;
    void set_edit_mode( edit_mode m );

    void refresh_properties() const;

    bool has_selection() const;
    void clear_selection();
    void select_all();

    void toggle_selection( item_instance* item );
    void add_selection( item_instance* item );
    void add_selection( const std::vector<item_instance*>& item );
    void set_selection( const std::vector<item_instance*>& item );
    void set_selection
    ( const std::vector<item_instance*>& item, item_instance* selected,
      bool add );
    void set_selection( item_instance* item );

  private:
    /** \brief The level in which we select the items. */
    gui_level& m_level;

    /** \brief The frame in which the properties of the items are displayed. */
    properties_frame& m_properties;

    /** \brief Tells to which items the operations are applied. */
    edit_mode m_edit_mode;

  }; // class selection_manager
} // namespace bf

#endif // __BF_SELECTION_MANAGER_HPP__
