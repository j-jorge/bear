/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::collision_align_policy.
 * \author Julien Jorge
 */
#include "universe/collision_align_policy.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c Tells how the contacts are set.
 */
bear::universe::collision_align_policy::collision_align_policy
( contact_mode::value_type c )
  : m_contact_mode( c )
{

} // collision_align_policy::collision_align_policy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell how the contacts are set.
 */
bear::universe::contact_mode::value_type
bear::universe::collision_align_policy::get_contact_mode() const
{
  return m_contact_mode;
} // collision_align_policy::get_contact_mode()
