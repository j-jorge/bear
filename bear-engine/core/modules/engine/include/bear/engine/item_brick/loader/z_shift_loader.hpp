/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item loader for z_shift.
 * \author Julien Jorge
 */
#ifndef __ENGINE_Z_SHIFT_LOADER_HPP__
#define __ENGINE_Z_SHIFT_LOADER_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/engine/loader/item_loader_base.hpp"

namespace bear
{
  namespace engine
  {
    class z_shift;

    /**
     * \brief The item loader for the z_shift.
     *
     * The custom fields of this class are:
     * - z_shift: int, Relative z position to put the other item at
     *              (default = 0).
     * - force_z_position : bool, Indicates if the item force the z_position.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT z_shift_loader:
      public item_loader_base
    {
    public:
      explicit z_shift_loader( z_shift& item );

      virtual z_shift_loader* clone() const;

      virtual bool set_field( const std::string& name, int value );
      virtual bool set_field( const std::string& name, bool value );

    private:
      /** \brief The item loaded by this loader. */
      z_shift& m_item;

    }; // class z_shift_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_Z_SHIFT_LOADER_HPP__
