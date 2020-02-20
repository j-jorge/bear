/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::teleport_item class.
 * \author Sébastien Angibaud
 */
#include "bear/generic_items/teleport_item.hpp"

#include "bear/engine/game.hpp"
#include "bear/engine/level.hpp"
#include "bear/engine/world.hpp"

BASE_ITEM_EXPORT( teleport_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::teleport_item::teleport_item()
: m_reference(NULL)
{

} // teleport_item::teleport_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c item.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::teleport_item::set_item_field
( const std::string& name, bear::engine::base_item* value )
{
  bool result(true);

  if ( name == "teleport_item.reference_point" )
    m_reference = handle_type(value);
  else
    result = super::set_item_field(name, value);

  return result;
} // teleport_item::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialized.
 */
bool bear::teleport_item::is_valid() const
{
  return m_reference != handle_type(NULL);
} // teleport_item::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the gap of the teleportation.
 */
bear::universe::position_type bear::teleport_item::get_gap() const
{
  if ( m_reference != handle_type() )
    return m_reference->get_center_of_mass() - get_center_of_mass();
  else
    return bear::universe::position_type(0,0);
} // teleport_item::get_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::teleport_item::on_toggle_on( engine::base_item* activator )
{
  if ( m_reference != handle_type(NULL) )
    {
      bear::universe::position_type gap
        ( m_reference->get_center_of_mass() - get_center_of_mass() );
      universe::rectangle_type rect = get_level().get_camera_focus();
      
      bear::engine::world::const_item_iterator it;
      for (it=get_world().living_items_begin(); 
           it!=get_world().living_items_end(); ++it)
        if ( rect.intersects( it->get_bounding_box() ) )
          it->set_center_of_mass(it->get_center_of_mass() + gap);
    }
} // teleport_item::on_toggle_on()
