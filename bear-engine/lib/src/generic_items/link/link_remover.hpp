/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that removes all the links on the colliding items.
 * \author Julien Jorge
 */
#ifndef __BEAR_LINK_REMOVER_HPP__
#define __BEAR_LINK_REMOVER_HPP__

#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that removes all the links on the colliding items.
   *
   * The parameters accepted by this item are:
   *  - \a sound: \c string The name of a sound to play when cutting the links
   *    (default = none).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT link_remover :
    public engine::item_with_decoration
    < engine::basic_renderable_item<engine::base_item> >
  {
    DECLARE_BASE_ITEM(link_remover);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_decoration
    < engine::basic_renderable_item<engine::base_item> > super;

  public:
    bool set_string_field( const std::string& name, const std::string& value );

  protected:
    void collision_remove_links
    ( engine::base_item& that, universe::collision_info& info );

  private:
    void collision
    ( engine::base_item& that, universe::collision_info& info );

  private:
    /** \brief The name of the sound to play. */
    std::string m_sound_name;

  }; // class link_remover
} // namespace bear

#endif // __BEAR_LINK_REMOVER_HPP__
