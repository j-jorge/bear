/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Description of a physical item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_PHYSICAL_ITEM_ATTRIBUTES_HPP__
#define __UNIVERSE_PHYSICAL_ITEM_ATTRIBUTES_HPP__

#include "bear/universe/contact_info.hpp"
#include "bear/universe/physical_item_flags.hpp"
#include "bear/universe/shape/shape.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Description of a physical item.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT physical_item_attributes
    {
    public:
      physical_item_attributes();

      /** \brief The shape of the item, defines its position and its size. */
      shape m_shape;
      physical_item_flags::type m_flags;
      
      /** \brief Current item's internal force. */
      force_type m_internal_force;

      /** \brief Current item's external force. */
      force_type m_external_force;

      /** \brief Item's orientation. */
      double m_system_angle;

      /** \brief Item's mass. */
      double m_mass;

      /** \brief Item friction as a percentage of the kept movement. */
      double m_self_friction;

      /** \brief Friction applied to the item by a contact with an other item,
          as a percentage of the kept movement. */
      double m_contact_friction;

      /** \brief Current item's speed. */
      speed_type m_speed;

      /** \brief Current item's angular speed. */
      double m_angular_speed;


      /** \brief How many temporary constaints the item has on its
          X-position. */
      std::uint8_t m_x_fixed;

      /** \brief How many temporary constaints the item has on its
          Y-position. */
      std::uint8_t m_y_fixed;

      
      /** \brief Item's density. */
      double m_density;

      /** \brief Current item's acceleration (forces coming from the item). */
      force_type m_acceleration;

      /** \brief Item elasticity. */
      double m_elasticity;

      /** \brief Item hardness. */
      double m_hardness;

      /** \brief The structure describing the contacts. */
      contact_info m_contact;

    }; // class physical_item_attributes
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_PHYSICAL_ITEM_ATTRIBUTES_HPP__
