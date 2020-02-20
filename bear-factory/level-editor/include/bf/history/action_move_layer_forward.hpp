/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of moving a layer forward.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_MOVE_LAYER_FORWARD_HPP__
#define __BF_ACTION_MOVE_LAYER_FORWARD_HPP__

#include "bf/history/level_action.hpp"

namespace bf
{
  /**
   * \brief The action of moving a layer forward.
   * \author Julien Jorge
   */
  class action_move_layer_forward:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param index The index of the layer to move.
     */
    action_move_layer_forward( unsigned int index );

    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The index of the layer to move. */
    const unsigned int m_original_index;

  }; // class action_move_layer_forward
} // namespace bf

#endif // __BF_ACTION_MOVE_LAYER_FORWARD_HPP__
