/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Safe way to point to an item that could die between two uses.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_DERIVED_ITEM_HANDLE_HPP__
#define __UNIVERSE_DERIVED_ITEM_HANDLE_HPP__

#include "bear/universe/item_handle.hpp"
#include "bear/universe/const_item_handle.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Safe way to point an item that could die between two uses.
     * \author Julien Jorge
     *
     * \b Template \b parameters:
     *  - \a DerivedType: the final type of the pointed item.
     *  - \a ItemType: a class that inherits from universe::physical_item.
     *
     * \b Sample \b usage:
     *
     * Let's consider the following item class
     * \code
     * class my_item:
     *   public physical_item,
     *   public an_other_class
     * {
     *   // ...
     * }; // my_item
     * \endcode
     *
     * Let \c special_item be an item that deals only with instances of \c
     * an_other_class. \c special_item can keep a handle on an item of type \c
     * my_item and still consider the \c an_other_class part by using a \c
     * derived_item_handle.
     * \code
     * class special_item
     * {
     * public:
     *   void set_item( physical_item* item ) { m_item = item; }
     *
     * private:
     *   // The item on which we work.
     *   derived_item_handle<an_other_class, physical_item> m_item;
     *
     * }; // special_item
     * \endcode
     *
     * \sa derived_item_handle_maker, const_derived_item_handle_maker
     */
    template<typename DerivedType, typename ItemType = physical_item>
    class derived_item_handle
    {
    public:
      typedef derived_item_handle<DerivedType, ItemType> self_type;
      typedef item_handle handle_type;
      typedef ItemType item_type;
      typedef DerivedType derived_type;

    public:
      derived_item_handle();
      derived_item_handle( ItemType* item );
      derived_item_handle( ItemType& item );
      derived_item_handle( const handle_type& item );
      derived_item_handle( const self_type& that );

      static self_type create_from_derived( DerivedType* item );
      static self_type create_from_derived( DerivedType& item );

      DerivedType* get() const;
      ItemType* get_item() const;

      DerivedType& operator*() const;
      DerivedType* operator->() const;

      self_type& operator=( ItemType* item );
      self_type& operator=( const self_type& that );
      self_type& operator=( const handle_type& item );

      bool operator==( const ItemType* item ) const;
      bool operator==( const DerivedType* item ) const;
      bool operator==( const self_type& that ) const;
      bool operator!=( const ItemType* item ) const;
      bool operator!=( const DerivedType* item ) const;
      bool operator!=( const self_type& that ) const;
      bool operator<( const self_type& that ) const;

    private:
      void cast_item();

    private:
      /** \brief The critical item. */
      handle_type m_item;

      DerivedType* m_derived;

    }; // class derived_item_handle

    /**
     * \brief Safe way to point a const item that could die between two uses.
     * \author Julien Jorge
     *
     * \sa derived_item_handle, const_derived_item_handle_maker
     */
    template<typename DerivedType, typename ItemType = const physical_item>
    class const_derived_item_handle
    {
    public:
      typedef const_derived_item_handle<DerivedType, ItemType> self_type;
      typedef const_item_handle handle_type;
      typedef ItemType item_type;
      typedef DerivedType derived_type;

    public:
      const_derived_item_handle();
      const_derived_item_handle( const ItemType* item );
      const_derived_item_handle( const ItemType& item );
      const_derived_item_handle( const handle_type& item );
      const_derived_item_handle( const self_type& that );

      static self_type create_from_derived( DerivedType* item );
      static self_type create_from_derived( DerivedType& item );

      const DerivedType* get() const;
      const ItemType* get_item() const;

      const DerivedType& operator*() const;
      const DerivedType* operator->() const;

      self_type& operator=( const ItemType* item );
      self_type& operator=( const self_type& that );
      self_type& operator=( const handle_type& item );

      bool operator==( const ItemType* item ) const;
      bool operator==( const DerivedType* item ) const;
      bool operator==( const self_type& that ) const;
      bool operator!=( const ItemType* item ) const;
      bool operator!=( const DerivedType* item ) const;
      bool operator!=( const self_type& that ) const;
      bool operator<( const self_type& that ) const;

    private:
      void cast_item();

    private:
      /** \brief The critical item. */
      handle_type m_item;

      const DerivedType* m_derived;

    }; // class const_derived_item_handle

    /**
     * \brief A wrapper to easily create a derived_item_handle for a non
     *        constant class type inheriting from physical_item.
     */
    template<typename DerivedType>
    derived_item_handle<DerivedType> item_handle_from( DerivedType* item );

    /**
     * \brief A wrapper to easily create a derived_item_handle for a constant
     *        class type inheriting from physical_item.
     */
    template<typename DerivedType>
    const_derived_item_handle<DerivedType>
    const_item_handle_from( const DerivedType* item );

  } // namespace universe
} // namespace bear

template<typename DerivedType, typename ItemType>
bool operator==
( const DerivedType* item,
  const bear::universe::derived_item_handle<DerivedType, ItemType>& handle );

template<typename DerivedType, typename ItemType>
bool operator!=
( const DerivedType* item,
  const bear::universe::derived_item_handle<DerivedType, ItemType>& handle );

template<typename DerivedType, typename ItemType>
bool operator==
( const DerivedType* item,
  const bear::universe::const_derived_item_handle<DerivedType, ItemType>&
  handle );

template<typename DerivedType, typename ItemType>
bool operator!=
( const DerivedType* item,
  const bear::universe::const_derived_item_handle<DerivedType, ItemType>&
  handle );

#include "bear/universe/impl/derived_item_handle.tpp"

#endif // __UNIVERSE_DERIVED_ITEM_HANDLE_HPP__
