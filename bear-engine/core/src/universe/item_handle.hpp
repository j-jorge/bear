/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Safe way to point an item that could die between two uses.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ITEM_HANDLE_HPP__
#define __UNIVERSE_ITEM_HANDLE_HPP__

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;

    /**
     * \brief Safe way to point an item that could die between two uses.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT item_handle
    {
    public:
      typedef physical_item item_type;

    public:
      item_handle();
      item_handle( item_type* item );
      item_handle( item_type& item );
      item_handle( const item_handle& that );
      ~item_handle();

      item_type* get() const;

      item_type& operator*() const;
      item_type* operator->() const;

      item_handle& operator=( item_type* item );
      item_handle& operator=( const item_handle& that );

      bool operator==( const item_type* item ) const;
      bool operator==( const item_handle& that ) const;
      bool operator!=( const item_type* item ) const;
      bool operator!=( const item_handle& that ) const;
      bool operator<( const item_handle& that ) const;

    private:
      /** \brief The critical item. */
      item_type* m_item;

    }; // class item_handle

  } // namespace universe
} // namespace bear

bool operator==
( const bear::universe::physical_item* a,
  const bear::universe::item_handle& b );
bool operator!=
( const bear::universe::physical_item* a,
  const bear::universe::item_handle& b );

#endif // __UNIVERSE_ITEM_HANDLE_HPP__
