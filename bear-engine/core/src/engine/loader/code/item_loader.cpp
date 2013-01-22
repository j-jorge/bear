/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_loader class.
 * \author Julien Jorge.
 */
#include "engine/loader/item_loader.hpp"

#include "engine/loader/item_loader_base.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::item_loader::item_loader()
  : m_impl(NULL)
{

} // item_loader::item_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param impl The implementation that effectively loads the item.
 */
bear::engine::item_loader::item_loader( const item_loader_base& impl )
  : m_impl( impl.clone() )
{

} // item_loader::item_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::item_loader::item_loader( const item_loader& that )
  : m_impl( (that.m_impl == NULL) ? NULL : that.m_impl->clone() )
{

} // item_loader::item_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::item_loader::~item_loader()
{
  delete m_impl;
} // item_loader::~item_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign an other loader to this one.
 * \param that The instance to copy from.
 */
bear::engine::item_loader&
bear::engine::item_loader::operator=( item_loader that )
{
  swap(that);
  return *this;
} // item_loader::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap this loader with an other one.
 * \param that The loader to swap with.
 */
void bear::engine::item_loader::swap( item_loader& that )
{
  std::swap( m_impl, that.m_impl );
} // item_loader::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name assigned to this loader.
 */
std::string bear::engine::item_loader::get_name() const
{
  return m_impl->get_name();
} // item_loader::get_name()
