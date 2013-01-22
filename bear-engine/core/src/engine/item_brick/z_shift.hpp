/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that shift other items on the z axis.
 * \author Julien Jorge
 */
#ifndef __ENGINE_Z_SHIFT_HPP__
#define __ENGINE_Z_SHIFT_HPP__

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief An item that shifts other items on the z axis.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    engine::base_item,
     *
     * The custom fields of this class are:
     * - z_shift: int, Relative z position to put the other item at
     *              (default = 0).
     * - force_z_position : bool, Indicates if the item force the z_position.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT z_shift
    {
    public:
      z_shift();

      void set_z_shift( int s );
      void set_force_z_position( bool b );

      void do_z_shift( base_item& that, const base_item& ref ) const;

      int get_z_shift() const;

    private:
      /** \brief Relative z position to put the other item at. */
      int m_z_shift;

      /** \brief Indicates if the item force the z_position. */
      bool m_force_z_position;

    }; // class z_shift
  } // namespace engine
} // namespace bear

#endif // __ENGINE_Z_SHIFT_HPP__
