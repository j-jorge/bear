/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Instances of base_reference_point compute the reference
 *        point used in forced movements.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_BASE_REFERENCE_POINT_HPP__
#define __UNIVERSE_BASE_REFERENCE_POINT_HPP__

#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;

    /**
     * \brief Instances of base_reference_point compute the reference
     *        point used in forced movements.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT base_reference_point
    {
    public:
      /** \brief Destructor. */
      virtual ~base_reference_point() {}

      /** \brief Create a copy of this instance using the operator new. */
      virtual base_reference_point* clone() const = 0;

      /** \brief Tell if get_point() can be safely called. */
      virtual bool is_valid() const = 0;

      /** \brief Get the position of the reference point. */
      virtual position_type get_point() const = 0;

      /** \brief Tell if a call to get_item() can be done. */
      virtual bool has_item() const = 0;

      /** \brief Get the item used to compute the position. */
      virtual physical_item& get_item() const = 0;

    }; // class base_reference_point
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_BASE_REFERENCE_POINT_HPP__
