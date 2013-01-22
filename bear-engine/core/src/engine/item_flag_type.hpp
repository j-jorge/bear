/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The type of the flags passed to a layer when inserting a new item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_FLAG_TYPE_HPP__
#define __ENGINE_ITEM_FLAG_TYPE_HPP__

namespace bear
{
  namespace engine
  {
    /** \brief Flags that can be passed to the item method. */
    enum item_flag_type
      {
        /** \brief No flag specified. */
        item_flag_none = 0,

        /** \brief Insert the item with a definitive size and position. */
        item_flag_insert_as_static = 1L << 0,

        /** \brief The z position of the item can't change. */
        item_flag_z_fixed = 1L << 1,

        /** \brief The item has been built. */
        item_flag_built = 1L << 2

      }; // enum item_flag_type

    item_flag_type operator&(item_flag_type a, item_flag_type b);
    item_flag_type operator|(item_flag_type a, item_flag_type b);
    item_flag_type operator^(item_flag_type a, item_flag_type b);
    item_flag_type operator~(item_flag_type a);

    item_flag_type& operator&=(item_flag_type& a, item_flag_type b);
    item_flag_type& operator|=(item_flag_type& a, item_flag_type b);
    item_flag_type& operator^=(item_flag_type& a, item_flag_type b);

  } // namespace engine
} // namespace bear

#endif // __ENGINE_ITEM_FLAG_TYPE_HPP__
