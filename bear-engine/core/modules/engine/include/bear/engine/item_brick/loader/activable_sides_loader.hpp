/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item loader for activable_sides.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ACTIVABLE_SIDES_LOADER_HPP__
#define __ENGINE_ACTIVABLE_SIDES_LOADER_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/engine/loader/item_loader_base.hpp"

namespace bear
{
  namespace engine
  {
    class activable_sides;

    /**
     * \brief The item loader for the activable_sides.
     *
     * The custom fields of this class are:
     * - left_side_is_active: bool, tell if the left side is active
     *   (default = false),
     * - right_side_is_active: bool, tell if the right side is active
     *   (default = false),
     * - top_side_is_active: bool, tell if the top side is active
     *   (default = false),
     * - bottom_side_is_active: bool, tell if the bottom side is active
     *   (default = false).
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT activable_sides_loader:
      public item_loader_base
    {
    public:
      explicit activable_sides_loader( activable_sides& item );

      virtual activable_sides_loader* clone() const;

      virtual bool set_field( const std::string& name, bool value );

    private:
      /** \brief The item loaded by this loader. */
      activable_sides& m_item;

    }; // class activable_sides_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_ACTIVABLE_SIDES_LOADER_HPP__
