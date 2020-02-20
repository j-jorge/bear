/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of removing layer from a level.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_REMOVE_LAYER_HPP__
#define __BF_ACTION_REMOVE_LAYER_HPP__

#include "bf/item_selection.hpp"
#include "bf/history/level_action.hpp"

namespace bf
{
  class layer;

  /**
   * \brief The action of removing a layer from a level.
   * \author Julien Jorge
   */
  class action_remove_layer:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param index The index of the layer to remove.
     */
    action_remove_layer( unsigned int index );

    /** \brief Destructor. */
    ~action_remove_layer();

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The layer removed. */
    layer* m_layer;

    /** \brief The selection in the layer. */
    item_selection m_selection;

    /** \brief The index of the layer in the level. */
    unsigned int m_index;

  }; // class action_remove_layer
} // namespace bf

#endif // __BF_ACTION_REMOVE_LAYER_HPP__
