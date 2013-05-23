/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to manage to which items the operations are applied.
 * \author Julien Jorge
 */
#ifndef __BF_EDIT_MODE_HPP__
#define __BF_EDIT_MODE_HPP__

#include "bf/item_selection.hpp"

#include <vector>

namespace bf
{
  class gui_level;

  /**
   * \brief A class to manage to which items the operations are applied.
   * \author Julien Jorge
   */
  class edit_mode
  {
  public:

    /**
     * \brief The various ways the operations can be applied.
     */
    enum value_type
      {
        /** \brief The operations are applied to the selection of the active
            layer. */
        active_layer,

        /** \brief The operations are applied to the selections of the layers
            with the same tag than the active layer. */
        layers_by_tag,

        /** \brief The operations are applied to the selections of all
            layers. */
        all_layers

      }; // enum value_type

  public:
    edit_mode();

    value_type get_value() const;
    void set_value( value_type v );

    item_selection get_selection( const gui_level& lvl ) const;
    std::vector<std::size_t> get_edit_layers( const gui_level& lvl ) const;
    bool is_in_edited_layer( const gui_level& lvl, item_instance* item ) const;

  private:
    std::vector<std::size_t>
    get_layer_indices_by_tag( const gui_level& lvl ) const;
    std::vector<std::size_t>
    get_all_layers_indices( const gui_level& lvl ) const;

    item_selection get_selections_by_layer_index
    ( const gui_level& lvl, std::vector<std::size_t> layers,
      std::size_t main_index ) const;

  private:
    /** \brief The current way the operations are applied. */
    value_type m_current_mode;

  }; // class ingame_view_frame
} // namespace bf

#endif // __BF_EDIT_MODE_HPP__
