/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Description of a physical item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_PHYSICAL_ITEM_ATTRIBUTES_HPP__
#define __UNIVERSE_PHYSICAL_ITEM_ATTRIBUTES_HPP__

#include "universe/contact_info.hpp"
#include "universe/types.hpp"

#include "universe/class_export.hpp"

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

    protected:
      /** \brief Item's mass. */
      double m_mass;

      /** \brief Item's density. */
      double m_density;

      /** \brief Current item's angular speed. */
      double m_angular_speed;

      /** \brief Current item's speed. */
      speed_type m_speed;

      /** \brief Current item's acceleration (forces coming from the item). */
      force_type m_acceleration;

      /** \brief Current item's internal force. */
      force_type m_internal_force;

      /** \brief Current item's external force. */
      force_type m_external_force;

      /** \brief Item friction as a percentage of the kept movement. */
      double m_self_friction;

      /** \brief Friction applied to the item by a contact with an other item,
          as a percentage of the kept movement. */
      double m_contact_friction;

      /** \brief Item elasticity. */
      double m_elasticity;

      /** \brief Item hardness. */
      double m_hardness;

      /** \brief Current item's position. */
      position_type m_position;

      /** \brief Item's size. */
      size_box_type m_size;

      /** \brief Item's orientation. */
      double m_system_angle;

      /** \brief Tell if the item's system is not modified by the contacts. */
      bool m_free_system_angle;

      /** \brief Tell if the item can move an other item. */
      bool m_can_move_items;

      /** \brief The structure describing the contacts. */
      contact_info m_contact;

      /** \brief Indicate if the item is a phantom, i.e. the item is never
          aligned by an other item. */
      bool m_is_phantom;

      /** \brief Indicate if the item is artificial, i.e. we don't care about
          the collisions. */
      bool m_is_artificial;

      /** \brief Tell if the item has to be considered as weak collisions. */
      bool m_weak_collisions;

      /** \brief How many temporary constaints the item has on its
          X-position. */
      unsigned int m_x_fixed;

      /** \brief How many temporary constaints the item has on its
          Y-position. */
      unsigned int m_y_fixed;

      /** \brief Indicates if the item is global. */
      bool m_global;

    }; // class physical_item_attributes
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_PHYSICAL_ITEM_ATTRIBUTES_HPP__
