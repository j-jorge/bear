/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An implementation of base_item that extends item_that_speaks.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_ITEM_THAT_SPEAKS_HPP__
#define __BEAR_ITEM_THAT_SPEAKS_HPP__

#include "engine/loader/item_loader_map.hpp"
#include "engine/comic/item/speaker_item.hpp"

#include "text_interface/base_exportable.hpp"

namespace bear
{
  namespace engine
  {

  /**
   * \brief An item that speaks.
   *
   * \b template \b parameters :
   * - \a Base : the base class for this item. Must inherit from
   *    engine::base_item,
   * \author Sebastien Angibaud
   */
  template<class Base>
  class item_that_speaks:
    public Base,
    public speaker_item
  {
    typedef Base super;

    TEXT_INTERFACE_DECLARE_METHOD_LIST(super, init_exported_methods)

  public:
    explicit item_that_speaks( std::string balloon_layer_name );
    item_that_speaks( const item_that_speaks<Base>& that );

    void progress( universe::time_type elapsed_time );

  protected:
    virtual void populate_loader_map( item_loader_map& m );

  private:
    static void init_exported_methods();

  private:
    /** \brief Tell if this item has been registered in a balloon layer. */
    bool m_registered_in_layer;

    /** \brief The name of the layer to which the messages are sent to display
        the balloon of this item. */
    std::string m_balloon_layer_name;

  }; // class item_that_speaks

  } // namespace engine
} // namespace bear

#include "engine/comic/item/impl/item_that_speaks.tpp"

#endif // __BEAR_ITEM_THAT_SPEAKS_HPP__
