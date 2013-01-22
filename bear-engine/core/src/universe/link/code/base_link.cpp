/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::base_link class.
 * \author Julien Jorge
 */
#include "universe/link/base_link.hpp"

#include "universe/forced_movement/center_of_mass_reference_point.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
std::size_t bear::universe::base_link::base_link::not_an_id(0);
std::size_t bear::universe::base_link::base_link::s_next_id(1);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param first_item The first linked item.
 * \param second_item The second linked item.
 * \pre \a first_item and \a second_item are distinct.
 */
bear::universe::base_link::base_link
( physical_item& first_item, physical_item& second_item )
  : m_first_item( center_of_mass_reference_point(first_item) ), 
    m_second_item( center_of_mass_reference_point(second_item) ), 
    m_id(s_next_id++)
{
  CLAW_PRECOND( &first_item != &second_item );

  m_first_item.get_item().add_link(*this);
  m_second_item.get_item().add_link(*this);
} // base_link::base_link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param first_item The first linked item.
 * \param second_item The second linked item.
 * \pre \a first_item and \a second_item are distinct.
 */
bear::universe::base_link::base_link
( const reference_point& first_item, const reference_point& second_item )
  : m_first_item(first_item), m_second_item(second_item), m_id(s_next_id++)
{
  CLAW_PRECOND( &first_item.get_item() != &second_item.get_item() );

  m_first_item.get_item().add_link(*this);
  m_second_item.get_item().add_link(*this);
} // base_link::base_link

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::base_link::~base_link()
{
  unlink();
} // base_link::~base_link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier of the link.
 */
std::size_t bear::universe::base_link::get_id() const
{
  return m_id;
} // base_link::get_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unlink the two items.
 */
void bear::universe::base_link::unlink()
{
  if ( m_first_item.has_item() )
    {
      m_first_item.get_item().remove_link(*this);
      m_first_item = reference_point();
    }

  if ( m_second_item.has_item() )
    {
      m_second_item.get_item().remove_link(*this);
      m_second_item = reference_point();
    }
} // base_link::unlink()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the first item concerned by this link.
 */
const bear::universe::physical_item&
bear::universe::base_link::get_first_item() const
{
  return m_first_item.get_item();
} // base_link::get_first_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the second item concerned by this link.
 */
const bear::universe::physical_item&
bear::universe::base_link::get_second_item() const
{
  return m_second_item.get_item();
} // base_link::get_second_item()
