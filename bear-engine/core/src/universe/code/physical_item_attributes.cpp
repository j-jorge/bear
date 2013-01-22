/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::physical_item_attributes class.
 * \author Julien Jorge
 */
#include "universe/physical_item_attributes.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::physical_item_attributes::physical_item_attributes()
  : m_mass(std::numeric_limits<double>::infinity()), m_density(1),
    m_angular_speed(0), m_speed(0, 0), m_acceleration(0, 0),
    m_internal_force(0, 0), m_external_force(0, 0), m_self_friction(0.98),
    m_contact_friction(1), m_elasticity(0), m_hardness(1), m_position(0, 0),
    m_size(0, 0), m_system_angle(0), m_free_system_angle(false),
    m_can_move_items(true), m_is_phantom(false), m_is_artificial(false),
    m_weak_collisions(false), m_x_fixed(0), m_y_fixed(0), m_global(false)
{

} // physical_item_attributes::physical_item_attributes()
