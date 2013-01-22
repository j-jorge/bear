/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::item_handle class.
 * \author Julien Jorge
 */
#include "universe/item_handle.hpp"

#include "universe/physical_item.hpp"
#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::item_handle::item_handle()
  : m_item(NULL)
{

} // item_handle::item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to handle.
 */
bear::universe::item_handle::item_handle( item_type* item )
  : m_item(item)
{
  if ( m_item )
    m_item->add_handle( this );
} // item_handle::item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to handle.
 */
bear::universe::item_handle::item_handle( item_type& item )
  : m_item(&item)
{
  m_item->add_handle( this );
} // item_handle::item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The handle from which we copy.
 */
bear::universe::item_handle::item_handle( const item_handle& that )
  : m_item(that.m_item)
{
  if ( m_item )
    m_item->add_handle( this );
} // item_handle::item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::item_handle::~item_handle()
{
  if ( m_item )
    m_item->remove_handle( this );
} // item_handle::item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer.
 */
bear::universe::item_handle::item_type* bear::universe::item_handle::get() const
{
  return m_item;
} // item_handle::get()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
bear::universe::item_handle::item_type&
bear::universe::item_handle::operator*() const
{
  return *m_item;
} // item_handle::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pointer-to-member operator.
 */
bear::universe::item_handle::item_type*
bear::universe::item_handle::operator->() const
{
  return m_item;
} // item_handle::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigment operator.
 * \param item The item to assign.
 */
bear::universe::item_handle&
bear::universe::item_handle::operator=( item_type* item )
{
  if ( m_item != NULL )
    m_item->remove_handle( this );

  m_item = item;

  if ( m_item != NULL )
    m_item->add_handle( this );

  return *this;
} // item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigment operator.
 * \param that The item to assign.
 */
bear::universe::item_handle&
bear::universe::item_handle::operator=( const item_handle& that )
{
  return *this = that.m_item;
} // item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param item The pointer to compare to.
 */
bool bear::universe::item_handle::operator==
( const item_type* item ) const
{
  return m_item == item;
} // item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The pointer to compare to.
 */
bool bear::universe::item_handle::operator==
( const item_handle& that ) const
{
  return m_item == that.m_item;
} // item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param item The pointer to compare to.
 */
bool bear::universe::item_handle::operator!=
( const item_type* item ) const
{
  return m_item != item;
} // item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param that The instance to compare to.
 */
bool bear::universe::item_handle::operator!=
( const item_handle& that ) const
{
  return m_item != that.m_item;
} // item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Less than" operator.
 * \param that The pointer to compare to.
 */
bool bear::universe::item_handle::operator<
  ( const item_handle& that ) const
{
  return m_item < that.m_item;
} // item_handle::operator<()




/*----------------------------------------------------------------------------*/
/**
 * \brief Compare a pointer to a physical_item with an item_handle.
 * \param a The pointer.
 * \param b The item handle.
 * \return b == a
 */
bool operator==
( const bear::universe::physical_item* a, const bear::universe::item_handle& b )
{
  return b == a;
} // operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare a pointer to a physical_item with an item_handle.
 * \param a The pointer.
 * \param b The item handle.
 * \return b != a
 */
bool operator!=
( const bear::universe::physical_item* a, const bear::universe::item_handle& b )
{
  return b != a;
} // operator!=()
