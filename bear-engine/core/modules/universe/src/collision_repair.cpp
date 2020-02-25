/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::collision_repair class.
 * \author Julien Jorge
 */
#include "bear/universe/collision_repair.hpp"

#include "bear/universe/collision_info.hpp"
#include "bear/universe/physical_item_state.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param first_item The first item in the collision.
 * \param second_item The second item in the collision.
 */
bear::universe::collision_repair::collision_repair
( physical_item_state& first_item, physical_item_state& second_item )
  : m_first_item(first_item), m_second_item(second_item),
    m_contact_normal(0, 0), m_contact_reference(NULL)
{

} // collision_repair::collision_repair()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the normal of the contact.
 * \param ref The item to use as the reference.
 * \param normal The normal of the contact.
 */
void bear::universe::collision_repair::set_contact_normal
( const physical_item_state& ref, const vector_type& normal )
{
  CLAW_PRECOND( (&ref == &m_first_item) || (&ref == &m_second_item) );

  if ( &ref == &m_first_item )
    m_contact_reference = &m_first_item;
  else
    m_contact_reference = &m_second_item;

  m_contact_normal = normal;
} // collision_repair::set_contact_normal()

/*----------------------------------------------------------------------------*/
/**
 * \brief Repair the collision.
 */
void bear::universe::collision_repair::apply()
{
  if (m_contact_reference != NULL)
    apply_force_transfert();
} // collision_repair::apply()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the transfert of forces in a collision.
 */
void bear::universe::collision_repair::apply_force_transfert()
{
  physical_item_state* other;

  if( m_contact_reference == &m_first_item )
    other = &m_second_item;
  else
    other = &m_first_item;

  // transfer the forces
  // Find the velocity in the direction of the contact.
  const speed_type relative_velocity
    ( m_contact_reference->get_speed() - other->get_speed() );
  const double separating_velocity
    ( relative_velocity.dot_product(m_contact_normal) );

  if ( separating_velocity < 0 )
    {
      // calculate the new separating velocity
      const double restitution
        ( m_contact_reference->get_hardness() * other->get_elasticity()
          + m_contact_reference->get_elasticity() * other->get_hardness() );

      const double new_sep_velocity( -separating_velocity * restitution );

      const double delta_velocity( new_sep_velocity - separating_velocity );
      const double total_inverse_mass
        ( 1 / m_contact_reference->get_mass() + 1 / other->get_mass() );

      if ( total_inverse_mass > 0 )
        {
          const double impulse( delta_velocity / total_inverse_mass );
          const speed_type impulse_per_inverse_mass
            ( m_contact_normal * impulse );

          m_contact_reference->set_speed
            ( m_contact_reference->get_speed()
              + impulse_per_inverse_mass / m_contact_reference->get_mass() );

          other->set_speed
            ( other->get_speed()
              - impulse_per_inverse_mass / other->get_mass() );
        }
    }
} // collision_repair::apply_force_transfert()
