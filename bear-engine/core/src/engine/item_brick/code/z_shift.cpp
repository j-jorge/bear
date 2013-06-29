/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::z_shift class.
 * \author Julien Jorge
 */
#include "engine/item_brick/z_shift.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::engine::z_shift::z_shift()
  : m_z_shift(0), m_force_z_position(false)
{

} // z_shift::z_shift()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the amount of shifting.
 * \param s The amount.
 */
void bear::engine::z_shift::set_z_shift( int s )
{
  m_z_shift = s;
} // z_shift::set_z_shift()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the item force the z_position.
 * \param b True to force the z position.
 */
void bear::engine::z_shift::set_force_z_position( bool b )
{
  m_force_z_position = b;
} // set_force_z_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Shift a item on the z axis.
 * \param that The item to shift.
 * \param ref The item according to which \a that is shifted.
 */
void bear::engine::z_shift::do_z_shift
( base_item& that, const base_item& ref ) const
{
  if ( !that.is_z_fixed() )
    {
      const int that_z( that.get_z_position() );
      const int ref_z( ref.get_z_position() );

      if( ((m_z_shift > 0) && ( that_z <= ref_z ))
          || ((m_z_shift < 0) && ( that_z >= ref_z ))
          || m_force_z_position )
        that.set_z_position( ref_z + m_z_shift );
    }
} // z_shift::do_z_shift()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the relative z position.
 */
int bear::engine::z_shift::get_z_shift() const
{
  return m_z_shift;
} // z_shift::get_z_shift()
