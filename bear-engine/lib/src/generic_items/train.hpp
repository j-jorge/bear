/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The class describing the base of a train platform.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_TRAIN_HPP__
#define __BEAR_TRAIN_HPP__

#include "generic_items/class_export.hpp"
#include "generic_items/base_train.hpp"
#include "generic_items/block.hpp"
#include "universe/physical_item_state.hpp"

namespace bear
{
  /**
   * \brief The class describing a train platform.
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT train:
    public block,
    public base_train
  {
    DECLARE_BASE_ITEM(train);

  public:
    /** \brief The type of the parent class. */
    typedef block super;

  public:
    train();

    void on_enters_layer();
    void progress( universe::time_type elapsed_time );

  protected:
    void to_string( std::string& str ) const;

    void collision( engine::base_item& that, universe::collision_info& info );
    void move( universe::time_type elapsed_time );

  private:
    void get_dependent_items( universe::physical_item::item_list& d ) const;
  }; // class train
} // namespace bear

#endif // __BEAR_TRAIN_HPP__
