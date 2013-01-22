/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the model_mark_reference_point class.
 * \author Sébastien Angibaud
 */
#include "engine/reference_point/model_mark_reference_point.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item of which the attack point is returned.
 */
template<class Base>
bear::engine::model_mark_reference_point<Base>::model_mark_reference_point
( bear::universe::physical_item& item, const std::string& mark_name )
  : m_item(item), m_mark_name(mark_name)
{

} // model_mark_reference_point::model_mark_reference_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Duplicate this instance.
 */
template<class Base>
bear::universe::base_reference_point*
bear::engine::model_mark_reference_point<Base>::clone() const
{
  return new model_mark_reference_point<Base>(*this);
} // model_mark_reference_point::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this reference is usable.
 */
template<class Base>
bool bear::engine::model_mark_reference_point<Base>::is_valid() const
{  
  if ( has_item() ) 
    {
      bear::engine::model_mark_placement m;
      return m_item->get_mark_placement(m_mark_name, m); 
    }
  else
    return false;
} // model_mark_reference_point::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the center of mass of the item.
 */
template<class Base>
bear::universe::position_type
bear::engine::model_mark_reference_point<Base>::get_point() const
{
  CLAW_PRECOND( has_item() );

  return m_item->get_mark_world_position(m_mark_name);
} // model_mark_reference_point::get_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is still valid.
 */
template<class Base>
bool bear::engine::model_mark_reference_point<Base>::has_item() const
{
  return m_item != (Base*)NULL;
} // model_mark_reference_point::has_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item of which the center of mass is returned.
 */
template<class Base>
bear::universe::physical_item&
bear::engine::model_mark_reference_point<Base>::get_item() const
{
  CLAW_PRECOND( has_item() );
  
  return *m_item.get_item();
} // model_mark_reference_point::get_item()
