/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::physical_item_attributes class.
 * \author Julien Jorge
 */
#include "universe/physical_item_attributes.hpp"

#include "universe/shape/rectangle.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::physical_item_attributes::physical_item_attributes()
  : m_shape( rectangle() ),
    m_flags( physical_item_flags::can_move_items ),
    m_internal_force(0, 0),
    m_external_force(0, 0),
    m_system_angle(0),
    m_mass(std::numeric_limits<double>::infinity()),
    m_self_friction(0.98),
    m_contact_friction(1),
    m_speed(0, 0),
    m_angular_speed(0),
    m_x_fixed(0),
    m_y_fixed(0),
    m_density(1),
    m_acceleration(0, 0),
    m_elasticity(0),
    m_hardness(1)
{

} // physical_item_attributes::physical_item_attributes()
