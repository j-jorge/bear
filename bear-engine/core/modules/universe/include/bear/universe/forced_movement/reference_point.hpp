/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Instances of reference_point compute the reference point used in
 *        forced movements.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_REFERENCE_POINT_HPP__
#define __UNIVERSE_REFERENCE_POINT_HPP__

#include "bear/universe/types.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;
    class base_reference_point;

    /**
     * \brief Instances of reference_point compute the reference
     *        point used in forced movements.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT reference_point
    {
    public:
      reference_point();
      reference_point( const reference_point& that );
      reference_point( const base_reference_point& p );
      ~reference_point();

      reference_point& operator=( const reference_point& that );

      bool is_valid() const;
      position_type get_point() const;
      bool has_item() const;
      physical_item& get_item() const;

    private:
      /** \brief The effective reference point. */
      base_reference_point* m_reference;

    }; // class reference_point
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_REFERENCE_POINT_HPP__
