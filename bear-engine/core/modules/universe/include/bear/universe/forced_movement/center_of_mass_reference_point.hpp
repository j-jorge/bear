/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Instances of center_of_mass_reference_point compute the reference
 *        point of in forced movements as the center of mass of a given item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CENTER_OF_MASS_REFERENCE_POINT_HPP__
#define __UNIVERSE_CENTER_OF_MASS_REFERENCE_POINT_HPP__

#include "bear/universe/forced_movement/base_reference_point.hpp"
#include "bear/universe/item_handle.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Instances of center_of_mass_reference_point compute the reference
     *        point of in forced movements as the center of mass of a given
     *        item.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT center_of_mass_reference_point:
      public base_reference_point
    {
    public:
      explicit center_of_mass_reference_point( physical_item& item );

      virtual base_reference_point* clone() const;

      virtual bool is_valid() const;
      virtual position_type get_point() const;

      virtual bool has_item() const;
      virtual physical_item& get_item() const;

    private:
      /** The item on which we take the center of mass. */
      item_handle m_item;

    }; // class center_of_mass_reference_point
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_CENTER_OF_MASS_REFERENCE_POINT_HPP__
