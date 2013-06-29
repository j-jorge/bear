/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to display a link between two items with a chain.
 * \author Julien Jorge
 */
#ifndef __BEAR_CHAIN_LINK_VISUAL_HPP__
#define __BEAR_CHAIN_LINK_VISUAL_HPP__

#include "generic_items/link/base_link_visual.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A class to display a link between two items as a chain.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT chain_link_visual:
    public engine::item_with_decoration
    < engine::basic_renderable_item<base_link_visual> >
  {
    DECLARE_BASE_ITEM(chain_link_visual);

  public:
    typedef engine::item_with_decoration
    < engine::basic_renderable_item<base_link_visual> > super;

  public:
    chain_link_visual();

    bool set_real_field( const std::string& name, double value );
    bool set_u_integer_field( const std::string& name, unsigned int value );
    bool set_bool_field( const std::string& name, bool value );

    void get_visual( std::list<engine::scene_visual>& visuals ) const;
    
    void set_link_count(std::size_t link_count);
    void set_max_fall(universe::coordinate_type max_fall);
    void set_dynamic_length(bool dynamic_length);
    
  private:
    unsigned int get_link_count() const;

  private:
    /** \brief The count of links in the chain. */
    std::size_t m_link_count;

    /** \brief How far the the links can fall. */
    universe::coordinate_type m_max_fall;

    /** \brief Indicates if the number of links is dynamically computed. */
    bool m_dynamic_length;
  }; // class chain_link_visual
} // namespace bear

#endif // __BEAR_CHAIN_LINK_VISUAL_HPP__
