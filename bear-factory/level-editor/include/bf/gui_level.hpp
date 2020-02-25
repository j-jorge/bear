/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A level in the graphical user interface.
 * \author Julien Jorge
 */
#ifndef __BF_GUI_LEVEL_HPP__
#define __BF_GUI_LEVEL_HPP__

#include "bf/item_selection.hpp"
#include "bf/level.hpp"
#include "bf/types.hpp"

#include <vector>

#include <claw/non_copyable.hpp>

namespace bf
{
  /**
   * \brief A level in the graphical user interface.
   * \author Julien Jorge
   */
  class gui_level:
    public level,
    claw::pattern::non_copyable
  {
  public:
    gui_level
    ( const std::string& name, unsigned int width, unsigned int height,
      const std::string& mus );

    bool has_selection( unsigned int layer_index ) const;

    const item_selection& get_selection( unsigned int layer_index ) const;
    item_instance* get_main_selection( unsigned int layer_index ) const;

    bool item_is_selected( item_instance const* item ) const;
    bool item_is_main_selection( item_instance const* item ) const;

    void add_to_selection( const item_selection& s );
    void add_to_selection( item_instance* item, bool main_selection = false );

    void remove_from_selection( item_instance* item );
    void remove_from_selection( const item_selection& s );

    void clear_selection( unsigned int layer_index );

    void remove_item( unsigned int layer_index, item_instance* item );

    bool layer_is_visible( unsigned int layer_index ) const;
    void set_layer_visibility( unsigned int layer_index, bool b );

    void set_active_layer( unsigned int layer_index );
    layer& get_active_layer() const;

    std::size_t get_active_layer_index() const;

    layer& add_layer
    ( const std::string& class_name, const std::string& name,
      bool fit_level = true,
      unsigned int width = 1, unsigned int height = 1 );
    void add_layer( layer* lay, unsigned int layer_index );
    layer* remove_layer( unsigned int layer_index );

    void move_backward( unsigned int layer_index );
    void move_forward( unsigned int layer_index );

    bool check_item_position() const;

    rectangle_type get_visual_box( const item_instance& item ) const;

  private:
    /** \brief The visibility of the layers. */
    std::vector<bool> m_layer_visibility;

    /** \brief The selection in each layer. */
    std::vector<item_selection> m_selection_by_layer;

    /** \brief Index of the layer on which we are working. */
    unsigned int m_active_layer;

  }; // class gui_level
} // namespace bf

#endif // __BF_GUI_LEVEL_HPP__
