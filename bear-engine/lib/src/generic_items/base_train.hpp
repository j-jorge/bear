/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The class describing the base of a base_train platform.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_BASE_TRAIN_HPP__
#define __BEAR_BASE_TRAIN_HPP__

#include "universe/physical_item.hpp"
#include "universe/item_handle.hpp"
#include "engine/base_item.hpp"

#include "generic_items/class_export.hpp"

#include <list>

namespace bear
{
  /**
   * \brief The class describing a base_train platform.
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT base_train
  {
  public:
    /** \brief The type of the list in which we store the items depending on the
        base_train. */
    typedef std::list<universe::item_handle> item_list;

  protected:
    void init(const universe::position_type& position);
    void update_items();
    void to_string( std::string& str ) const;
    void collision_as_base_train
    ( engine::base_item& that, universe::collision_info& info );
    void update_item_positions
    ( const universe::position_type& position, 
      const universe::speed_type& speed );
    void get_items( std::list<universe::physical_item*>& d ) const;

  private:
    /** \brief The list of items on the platform. */
    item_list m_list_items;

    /** \brief The list of the items that were on the platform. */
    item_list m_old_items;

    /** \brief The last position of the platform. */
    universe::position_type m_last_position;

  }; // class base_train
} // namespace bear

#endif // __BEAR_BASE_TRAIN_HPP__
