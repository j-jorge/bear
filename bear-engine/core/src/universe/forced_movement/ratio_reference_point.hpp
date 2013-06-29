/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Instances of ratio_reference_point compute the reference
 *        point of in forced movements of a given item according to
 *        ratio and gap.
 * \author Sebastien Angibaud
 */
#ifndef __UNIVERSE_RATIO_REFERENCE_POINT_HPP__
#define __UNIVERSE_RATIO_REFERENCE_POINT_HPP__

#include "universe/forced_movement/base_reference_point.hpp"
#include "universe/item_handle.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Instances of ratio_reference_point compute the reference
     *        point of in forced movements of a given item according to
     *        ratio and gap.
     *
     * \author Sebastien Angibaud
     */
    class UNIVERSE_EXPORT ratio_reference_point:
      public base_reference_point
    {
    public:
      explicit ratio_reference_point
      ( physical_item& item,
        const universe::position_type& ratio,
        const universe::position_type& gap );

      virtual base_reference_point* clone() const;

      virtual bool is_valid() const;
      virtual position_type get_point() const;

      virtual bool has_item() const;
      virtual physical_item& get_item() const;

    private:
      /** The item on which we take the center of mass. */
      item_handle m_item;

      /* \brief The ratio applied to compute reference point. */
      universe::position_type m_ratio;

       /* \brief The gap applied to compute reference point. */
      universe::position_type m_gap;

    }; // class ratio_reference_point
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_RATIO_REFERENCE_POINT_HPP__
