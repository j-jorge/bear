/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of adding layer in a level.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_ADD_LAYER_HPP__
#define __BF_ACTION_ADD_LAYER_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  class layer;

  /**
   * \brief The action of adding a layer in a level.
   * \author Julien Jorge
   */
  class action_add_layer:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param lay The layer to add.
     * \param index The index of the layer in the level.
     */
    action_add_layer( layer* lay, unsigned int index );

    /** \brief Destructor. */
    ~action_add_layer();

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The layer to add. */
    layer* m_layer;

    /** \brief The index of the layer in the level. */
    unsigned int m_index;

  }; // class action_add_layer
} // namespace bf

#endif // __BF_ACTION_ADD_LAYER_HPP__
