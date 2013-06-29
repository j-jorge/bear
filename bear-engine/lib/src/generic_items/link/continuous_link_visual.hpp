/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to display a continuous link between two items.
 * \author Julien Jorge
 */
#ifndef __BEAR_CONTINUOUS_LINK_VISUAL_HPP__
#define __BEAR_CONTINUOUS_LINK_VISUAL_HPP__

#include "generic_items/link/base_link_visual.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A class to display a continuous link between two items.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT continuous_link_visual:
    public engine::item_with_decoration
    < engine::basic_renderable_item<base_link_visual> >
  {
    DECLARE_BASE_ITEM(continuous_link_visual);

  public:
    typedef engine::item_with_decoration
    < engine::basic_renderable_item<base_link_visual> > super;

  private:
    /**
     * \brief The item loader for the continuous link visual.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;

    public:
      explicit loader( continuous_link_visual& item );

      loader* clone() const;

      bool set_field( const std::string& name, double value );

    private:
      /** \brief The item loaded by this instance. */
      continuous_link_visual& m_item;

    }; // class loader

  public:
    continuous_link_visual();

    void get_visual( std::list<engine::scene_visual>& visuals ) const;

    void set_line_width( double w );

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    bear::engine::scene_visual build_sprite_visual( visual::sprite s ) const;
    bear::engine::scene_visual build_line_visual() const;

  private:
    /** \brief The width of the line displayed when no sprite has been assigned
        to the item. */
    double m_line_width;

  }; // class continuous_link_visual
} // namespace bear

#endif // __BEAR_CONTINUOUS_LINK_VISUAL_HPP__
