/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the
 *        bear::engine::forced_movement_creator_with_reference class.
 * \author Sebastien Angibaud
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<class Base, class MovementType>
bear::engine::forced_movement_creator_with_reference<Base, MovementType>::
forced_movement_creator_with_reference()
  : m_ratio(0.5, 0.5), m_gap(0, 0)
{

} // forced_movement_creator_with_reference::forced_movement_creator_with_reference()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base, class MovementType>
bool
bear::engine::forced_movement_creator_with_reference<Base, MovementType>::
set_real_field
( const std::string& name, double value )
{
  bool ok(true);

  if (name == "forced_movement_creator_with_reference.target.ratio.x")
    m_ratio.x = value;
  else if (name == "forced_movement_creator_with_reference.target.ratio.y")
    m_ratio.y = value;
  else if (name == "forced_movement_creator_with_reference.target.gap.x")
    m_gap.x = value;
  else if (name == "forced_movement_creator_with_reference.target.gap.y")
    m_gap.y = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // forced_movement_creator_with_reference::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
template<class Base, class MovementType>
bool
bear::engine::forced_movement_creator_with_reference<Base, MovementType>::
set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( (name == "forced_movement_creator_with_reference.target")
       && (value != NULL) )
    m_movement.set_ratio_reference_point(*value, m_ratio, m_gap);
  else
    result = super::set_item_field(name, value);

  return result;
} // forced_movement_creator_with_reference::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
template<class Base, class MovementType>
bool
bear::engine::forced_movement_creator_with_reference<Base, MovementType>::
is_valid() const
{
  return m_movement.has_reference_item() && super::is_valid();
} // forced_movement_creator_with_reference::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
template<class Base, class MovementType>
void
bear::engine::forced_movement_creator_with_reference<Base, MovementType>::
build()
{
  super::build();

  this->set_forced_movement(m_movement);

  this->kill();
} // forced_movement_creator_with_reference::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the movement.
 */
template<class Base, class MovementType>
MovementType&
bear::engine::forced_movement_creator_with_reference<Base, MovementType>::
get_movement()
{
  return m_movement;
} // forced_movement_creator_with_reference::get_movement()
