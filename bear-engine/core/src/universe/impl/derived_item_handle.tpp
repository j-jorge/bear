/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the universe::derived_item_handle class.
 * \author Julien Jorge
 */
#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default Constructor.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
derived_item_handle()
  : m_derived(NULL)
{

} // derived_item_handle::derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item on which we keep a handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
derived_item_handle( ItemType* item )
  : m_item(item)
{
  cast_item();
} // derived_item_handle::derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item on which we keep a handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
derived_item_handle( ItemType& item )
  : m_item(item)
{
  cast_item();
} // derived_item_handle::derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item on which we keep a handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
derived_item_handle( const handle_type& item )
  : m_item(item)
{
  cast_item();
} // derived_item_handle::derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
derived_item_handle( const self_type& that )
  : m_item(that.m_item), m_derived(that.m_derived)
{

} // derived_item_handle::derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a derived_item_handle from a pointer on an instance of the
 *        derived type.
 * \param item The item for which we create the derived_item_handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
create_from_derived( DerivedType* item )
{
  return self_type( dynamic_cast<item_type*>(item) );
} // derived_item_handle::create_from_derived()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a derived_item_handle from a pointer on an instance of the
 *        derived type.
 * \param item The item for which we create the derived_item_handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>
bear::universe::derived_item_handle<DerivedType, ItemType>::
create_from_derived( DerivedType& item )
{
  return self_type( dynamic_cast<item_type*>(&item) );
} // derived_item_handle::create_from_derived()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the derived class.
 */
template<typename DerivedType, typename ItemType>
DerivedType*
bear::universe::derived_item_handle<DerivedType, ItemType>::get() const
{
  if ( m_item.get() == NULL )
    return NULL;
  else
    return m_derived;
} // derived_item_handle::get()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the handled item.
 */
template<typename DerivedType, typename ItemType>
ItemType*
bear::universe::derived_item_handle<DerivedType, ItemType>::get_item() const
{
  return static_cast<ItemType*>(m_item.get());
} // derived_item_handle::get_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
template<typename DerivedType, typename ItemType>
DerivedType&
bear::universe::derived_item_handle<DerivedType, ItemType>::operator*() const
{
  return *get();
} // derived_item_handle::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pointer-to-member operator.
 */
template<typename DerivedType, typename ItemType>
DerivedType*
bear::universe::derived_item_handle<DerivedType, ItemType>::operator->() const
{
  return get();
} // derived_item_handle::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign a new item to keep an handle to.
 * \param item The item.
 */
template<typename DerivedType, typename ItemType>
typename bear::universe::derived_item_handle<DerivedType, ItemType>::self_type&
bear::universe::derived_item_handle<DerivedType, ItemType>::operator=
( ItemType* item )
{
  m_item = item;
  cast_item();
  return *this;
} // derived_item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 */
template<typename DerivedType, typename ItemType>
typename bear::universe::derived_item_handle<DerivedType, ItemType>::self_type&
bear::universe::derived_item_handle<DerivedType, ItemType>::operator=
( const self_type& that )
{
  m_item = that.m_item;
  m_derived = that.m_derived;
  return *this;
} // derived_item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign a new item to keep an handle to.
 * \param item The item.
 */
template<typename DerivedType, typename ItemType>
typename bear::universe::derived_item_handle<DerivedType, ItemType>::self_type&
bear::universe::derived_item_handle<DerivedType, ItemType>::operator=
( const handle_type& item )
{
  m_item = item;
  cast_item();
  return *this;
} // derived_item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param item The item to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator==
( const ItemType* item ) const
{
  return m_item == item;
} // derived_item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param item The item to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator==
( const DerivedType* item ) const
{
  return (m_derived == item) || ((item == NULL) && (m_item == NULL));
} // derived_item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The instance to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator==
( const self_type& that ) const
{
  return m_item == that.m_item;
} // derived_item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param item The pointer to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator!=
( const ItemType* item ) const
{
  return !(*this == item);
} // derived_item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param item The pointer to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator!=
( const DerivedType* item ) const
{
  return !(*this == item);
} // derived_item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param that The instance to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator!=
( const self_type& that ) const
{
  return !(*this == that);
} // derived_item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Less than" operator.
 * \param that The pointer to compare to.
 */
template<typename DerivedType, typename ItemType>
bool bear::universe::derived_item_handle<DerivedType, ItemType>::operator<
( const self_type& that ) const
{
  return m_item < that.m_item;
} // derived_item_handle::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dynamically cast the item into the derived type.
 */
template<typename DerivedType, typename ItemType>
void bear::universe::derived_item_handle<DerivedType, ItemType>::cast_item()
{
  m_derived = dynamic_cast<DerivedType*>(m_item.get());
} // derived_item_handle::cast_item()



/*----------------------------------------------------------------------------*/
/**
 * \brief Default Constructor.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
const_derived_item_handle()
  : m_derived(NULL)
{

} // const_derived_item_handle::const_derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item on which we keep a handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
const_derived_item_handle( const ItemType* item )
  : m_item(item)
{
  cast_item();
} // const_derived_item_handle::const_derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item on which we keep a handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
const_derived_item_handle( const ItemType& item )
  : m_item(item)
{
  cast_item();
} // const_derived_item_handle::const_derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item on which we keep a handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
const_derived_item_handle( const handle_type& item )
  : m_item(item)
{
  cast_item();
} // const_derived_item_handle::const_derived_item_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
const_derived_item_handle( const self_type& that )
  : m_item(that.m_item), m_derived(that.m_derived)
{

} // const_derived_item_handle::const_derived_item_handle()


/*----------------------------------------------------------------------------*/
/**
 * \brief Build a derived_item_handle from a pointer on an instance of the
 *        derived type.
 * \param item The item for which we create the derived_item_handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
create_from_derived( DerivedType* item )
{
  return self_type( dynamic_cast<item_type*>(item) );
} // const_derived_item_handle::create_from_derived()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a derived_item_handle from a pointer on an instance of the
 *        derived type.
 * \param item The item for which we create the derived_item_handle.
 */
template<typename DerivedType, typename ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
create_from_derived( DerivedType& item )
{
  return self_type( dynamic_cast<item_type*>(&item) );
} // const_derived_item_handle::create_from_derived()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the derived class.
 */
template<typename DerivedType, typename ItemType>
const DerivedType*
bear::universe::const_derived_item_handle<DerivedType, ItemType>::get() const
{
  if ( m_item.get() == NULL )
    return NULL;
  else
    return m_derived;
} // const_derived_item_handle::get()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the handled item.
 */
template<typename DerivedType, typename ItemType>
const ItemType*
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
get_item() const
{
  return (ItemType*)m_item.get();
} // const_derived_item_handle::get_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dereference operator.
 */
template<typename DerivedType, typename ItemType>
const DerivedType&
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
operator*() const
{
  return *get();
} // const_derived_item_handle::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pointer-to-member operator.
 */
template<typename DerivedType, typename ItemType>
const DerivedType*
bear::universe::const_derived_item_handle<DerivedType, ItemType>::
operator->() const
{
  return get();
} // const_derived_item_handle::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign a new item to keep an handle to.
 * \param item The item.
 */
template<typename DerivedType, typename ItemType>
typename
bear::universe::const_derived_item_handle<DerivedType, ItemType>::self_type&
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator=
( const ItemType* item )
{
  m_item = item;
  cast_item();
  return *this;
} // const_derived_item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 */
template<typename DerivedType, typename ItemType>
typename
bear::universe::const_derived_item_handle<DerivedType, ItemType>::self_type&
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator=
( const self_type& that )
{
  m_item = that.m_item;
  m_derived = that.m_derived;
  return *this;
} // const_derived_item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign a new item to keep an handle to.
 * \param item The item.
 */
template<typename DerivedType, typename ItemType>
typename
bear::universe::const_derived_item_handle<DerivedType, ItemType>::self_type&
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator=
( const handle_type& item )
{
  m_item = item;
  cast_item();
  return *this;
} // const_derived_item_handle::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param item The item to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator==
( const ItemType* item ) const
{
  return m_item == item;
} // const_derived_item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param item The item to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator==
( const DerivedType* item ) const
{
  return (m_derived == item) || ((item == NULL) && (m_item == NULL));
} // const_derived_item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The instance to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator==
( const self_type& that ) const
{
  return m_item == that.m_item;
} // const_derived_item_handle::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param item The pointer to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator!=
( const ItemType* item ) const
{
  return !(*this == item);
} // const_derived_item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param item The pointer to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator!=
( const DerivedType* item ) const
{
  return !(*this == item);
} // const_derived_item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param that The instance to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator!=
( const self_type& that ) const
{
  return !(*this == that);
} // const_derived_item_handle::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief "Less than" operator.
 * \param that The pointer to compare to.
 */
template<typename DerivedType, typename ItemType>
bool
bear::universe::const_derived_item_handle<DerivedType, ItemType>::operator<
( const self_type& that ) const
{
  return m_item < that.m_item;
} // const_derived_item_handle::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Dynamically cast the item into the derived type.
 */
template<typename DerivedType, typename ItemType>
void
bear::universe::const_derived_item_handle<DerivedType, ItemType>::cast_item()
{
  m_derived = dynamic_cast<const DerivedType*>(m_item.get());
} // const_derived_item_handle::cast_item()




/*----------------------------------------------------------------------------*/
/**
 * \brief A wrapper to easily create a derived_item_handle for a non constant
 *        class type inheriting from physical_item.
 */
template<typename DerivedType>
bear::universe::derived_item_handle<DerivedType>
bear::universe::item_handle_from( DerivedType* item )
{
  return derived_item_handle<DerivedType>( dynamic_cast<physical_item*>(item) );
} // item_handle_from()




/*----------------------------------------------------------------------------*/
/**
 * \brief A wrapper to easily create a derived_item_handle for a constant class
 *        type inheriting from physical_item.
 */
template<typename DerivedType>
bear::universe::const_derived_item_handle<DerivedType>
bear::universe::const_item_handle_from( const DerivedType* item )
{
  return const_derived_item_handle<DerivedType>
    ( dynamic_cast<const physical_item*>(item) );
} // const_item_handle_from()




/*----------------------------------------------------------------------------*/
/**
 * \brief Compare a pointer to an item with an item handle. Return true if the
 *        item handle contains the given item.
 * \param item The item.
 * \param handle The item handle.
 */
template<typename DerivedType, typename ItemType>
bool operator==
( const DerivedType* item,
  const bear::universe::derived_item_handle<DerivedType, ItemType>& handle )
{
  return handle == item;
} // operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare a pointer to an item with an item handle. Return true if the
 *        item handle does not contain the given item.
 * \param item The item.
 * \param handle The item handle.
 */
template<typename DerivedType, typename ItemType>
bool operator!=
( const DerivedType* item,
  const bear::universe::derived_item_handle<DerivedType, ItemType>& handle )
{
  return handle != item;
} // operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare a pointer to an item with an item handle. Return true if the
 *        item handle contains the given item.
 * \param item The item.
 * \param handle The item handle.
 */
template<typename DerivedType, typename ItemType>
bool operator==
( const DerivedType* item,
  const bear::universe::const_derived_item_handle<DerivedType, ItemType>&
  handle )
{
  return handle == item;
} // operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare a pointer to an item with an item handle. Return true if the
 *        item handle does not contain the given item.
 * \param item The item.
 * \param handle The item handle.
 */
template<typename DerivedType, typename ItemType>
bool operator!=
( const DerivedType* item,
  const bear::universe::const_derived_item_handle<DerivedType, ItemType>&
  handle )
{
  return handle != item;
} // operator!=()
