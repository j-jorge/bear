/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the reference_point class.
 * \author Julien Jorge.
 */
#include "universe/forced_movement/reference_point.hpp"

#include "universe/forced_movement/base_reference_point.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::reference_point::reference_point()
  : m_reference(NULL)
{

} // reference_point::reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::universe::reference_point::reference_point( const reference_point& that )
{
  if ( that.m_reference == NULL )
    m_reference = NULL;
  else
    m_reference = that.m_reference->clone();
} // reference_point::reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The effective reference point.
 */
bear::universe::reference_point::reference_point
( const base_reference_point& p )
  : m_reference( p.clone() )
{

} // reference_point::reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::reference_point::~reference_point()
{
  delete m_reference;
} // reference_point::~reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::universe::reference_point&
bear::universe::reference_point::operator=( const reference_point& that )
{
  reference_point tmp(that);
  std::swap(m_reference, tmp.m_reference);
  return *this;
} // reference_point::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this reference is usable.
 */
bool bear::universe::reference_point::is_valid() const
{
  return (m_reference != NULL) && m_reference->is_valid();
} // reference_point::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the point the forced movement has to use.
 */
bear::universe::position_type bear::universe::reference_point::get_point() const
{
  CLAW_PRECOND( is_valid() );
  return m_reference->get_point();
} // reference_point::get_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the reference use an item to compute the point.
 */
bool bear::universe::reference_point::has_item() const
{
  if ( m_reference == NULL )
    return false;
  else
    return m_reference->has_item();
} // reference_point::has_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item this instance uses to compute the point.
 */
bear::universe::physical_item& bear::universe::reference_point::get_item() const
{
  CLAW_PRECOND( has_item() );
  return m_reference->get_item();
} // reference_point::get_item()
