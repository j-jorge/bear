/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_with_friction class.
 * \author Sebastien Angibaud
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<class Base>
bear::engine::item_with_friction<Base>::item_with_friction()
  : m_left_friction(1), m_right_friction(1),
    m_top_friction(1), m_bottom_friction(1)
{

} // item_with_friction::item_with_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the left friction.
 */
template<class Base>
double bear::engine::item_with_friction<Base>::get_left_friction() const
{
  return m_left_friction;
} // item_with_friction::get_left_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the left friction.
 * \param f The new friction.
 */
template<class Base>
void bear::engine::item_with_friction<Base>::set_left_friction( double f )
{
  m_left_friction = f;
} // item_with_friction::set_left_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the right friction.
 */
template<class Base>
double bear::engine::item_with_friction<Base>::get_right_friction() const
{
  return m_right_friction;
} // item_with_friction::get_right_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the right friction.
 * \param f The new friction.
 */
template<class Base>
void bear::engine::item_with_friction<Base>::set_right_friction( double f )
{
  m_right_friction = f;
} // item_with_friction::set_right_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the top friction.
 */
template<class Base>
double bear::engine::item_with_friction<Base>::get_top_friction() const
{
  return m_top_friction;
} // item_with_friction::get_top_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the top friction.
 * \param f The new friction.
 */
template<class Base>
void bear::engine::item_with_friction<Base>::set_top_friction( double f )
{
  m_top_friction = f;
} // item_with_friction::set_top_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bottom.
 */
template<class Base>
double bear::engine::item_with_friction<Base>::get_bottom_friction() const
{
  return m_bottom_friction;
} // item_with_friction::get_bottom_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the bottom friction.
 * \param f The new friction.
 */
template<class Base>
void bear::engine::item_with_friction<Base>::set_bottom_friction( double f )
{
  m_bottom_friction = f;
} // item_with_friction::set_bottom_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_friction<Base>::set_real_field
( const std::string& name, double value )
{
  bool ok(true);

  if (name == "item_with_friction.left_friction")
    m_left_friction = value;
  else if (name == "item_with_friction.right_friction")
    m_right_friction = value;
  else if (name == "item_with_friction.top_friction")
    m_top_friction = value;
  else if (name == "item_with_friction.bottom_friction")
    m_bottom_friction = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // invisible_block::set_real_field()
