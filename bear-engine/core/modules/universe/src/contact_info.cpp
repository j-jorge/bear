/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::contact_info class.
 * \author Julien Jorge
 */
#include "bear/universe/contact_info.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::contact_info::contact_info()
  : m_middle_contact(false)
{

} // contact_info::contact_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the range of the contacts on the top side.
 */
const bear::universe::contact_range&
bear::universe::contact_info::get_top_contact() const
{
  return m_top_contact;
} // contact_info::get_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the object has a contact on the top side.
 */
bool bear::universe::contact_info::has_top_contact() const
{
  return !m_top_contact.empty();
} // contact_info::has_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the object has no contact on the top side.
 */
void bear::universe::contact_info::clear_top_contact()
{
  m_top_contact.clear();
} // contact_info::clear_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the range of the contacts on the top side.
 * \param left The coordinate where the contact begins.
 * \param right The coordinate where the contact ends.
 */
void bear::universe::contact_info::set_top_contact
( coordinate_type left, coordinate_type right )
{
  m_top_contact = contact_range( left, right );
} // contact_info::set_top_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the range of the contacts on the bottom side.
 */
const bear::universe::contact_range&
bear::universe::contact_info::get_bottom_contact() const
{
  return m_bottom_contact;
} // contact_info::get_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the object has a contact on the bottom side.
 */
bool bear::universe::contact_info::has_bottom_contact() const
{
  return !m_bottom_contact.empty();
} // contact_info::has_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the object has no contact on the bottom side.
 */
void bear::universe::contact_info::clear_bottom_contact()
{
  m_bottom_contact.clear();
} // contact_info::clear_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the range of the contacts on the bottom side.
 * \param left The coordinate where the contact begins.
 * \param right The coordinate where the contact ends.
 */
void bear::universe::contact_info::set_bottom_contact
( coordinate_type left, coordinate_type right )
{
  m_bottom_contact = contact_range( left, right );
} // contact_info::set_bottom_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the range of the contacts on the left side.
 */
const bear::universe::contact_range&
bear::universe::contact_info::get_left_contact() const
{
  return m_left_contact;
} // contact_info::get_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the object has a contact on the left side.
 */
bool bear::universe::contact_info::has_left_contact() const
{
  return !m_left_contact.empty();
} // contact_info::has_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the object has no contact on the left side.
 */
void bear::universe::contact_info::clear_left_contact()
{
  m_left_contact.clear();
} // contact_info::clear_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the range of the contacts on the left side.
 * \param bottom The coordinate where the contact begins.
 * \param top The coordinate where the contact ends.
 */
void bear::universe::contact_info::set_left_contact
( coordinate_type bottom, coordinate_type top )
{
  m_left_contact = contact_range( bottom, top );
} // contact_info::set_left_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the range of the contacts on the right side.
 */
const bear::universe::contact_range&
bear::universe::contact_info::get_right_contact() const
{
  return m_right_contact;
} // contact_info::get_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the object has a contact on the right side.
 */
bool bear::universe::contact_info::has_right_contact() const
{
  return !m_right_contact.empty();
} // contact_info::has_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the object has no contact on the right side.
 */
void bear::universe::contact_info::clear_right_contact()
{
  m_right_contact.clear();
} // contact_info::clear_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the range of the contacts on the right side.
 * \param bottom The coordinate where the contact begins.
 * \param top The coordinate where the contact ends.
 */
void bear::universe::contact_info::set_right_contact
( coordinate_type bottom, coordinate_type top )
{
  m_right_contact = contact_range( bottom, top );
} // contact_info::set_right_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a contact in the middle of the object.
 */
bool bear::universe::contact_info::has_middle_contact() const
{
  return m_middle_contact;
} // contact_info::has_middle_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a contact in the middle of the object.
 * \param contact Tell if there is a contact.
 */
void bear::universe::contact_info::set_middle_contact( bool contact )
{
  m_middle_contact = contact;
} // contact_info::set_middle_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the object has a contact.
 */
bool bear::universe::contact_info::has_contact() const
{
  return has_side_contact() || has_middle_contact();
} // contact_info::has_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the object has a contact on one of his sides.
 */
bool bear::universe::contact_info::has_side_contact() const
{
  return has_bottom_contact() || has_top_contact()
    || has_right_contact() || has_left_contact();
} // contact_info::has_side_contact()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the object has no contact.
 */
void bear::universe::contact_info::clear_contacts()
{
  clear_left_contact();
  clear_right_contact();
  clear_top_contact();
  clear_bottom_contact();
  set_middle_contact( false );
} // contact_info::clear_contacts()
