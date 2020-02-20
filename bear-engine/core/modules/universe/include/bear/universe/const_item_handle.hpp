/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Safe way to point an item that could die between two uses.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CONST_ITEM_HANDLE_HPP__
#define __UNIVERSE_CONST_ITEM_HANDLE_HPP__

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;

    /**
     * \brief Safe way to point an item that could die between two uses.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT const_item_handle
    {
    public:
      typedef const physical_item item_type;

    public:
      const_item_handle();
      const_item_handle( const item_type* item );
      const_item_handle( const item_type& item );
      const_item_handle( const const_item_handle& that );
      ~const_item_handle();

      const item_type* get() const;

      const item_type& operator*() const;
      const item_type* operator->() const;

      const_item_handle& operator=( const item_type* item );
      const_item_handle& operator=( const const_item_handle& that );

      bool operator==( const item_type* item ) const;
      bool operator==( const const_item_handle& that ) const;
      bool operator!=( const item_type* item ) const;
      bool operator!=( const const_item_handle& that ) const;
      bool operator<( const const_item_handle& that ) const;

    private:
      /** \brief The critical item. */
      const item_type* m_item;

    }; // class const_item_handle

  } // namespace universe
} // namespace bear

bool operator==
( const bear::universe::physical_item* a,
  const bear::universe::const_item_handle& b );
bool operator!=
( const bear::universe::physical_item* a,
  const bear::universe::const_item_handle& b );

#endif // __UNIVERSE_CONST_ITEM_HANDLE_HPP__
