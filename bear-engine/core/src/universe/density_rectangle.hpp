/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A rectangle in the world, where the density is different from the
 *        default density.
 * \author Sebastien Angibaud
 */
#ifndef __UNIVERSE_DENSITY_RECTANGLE_HPP__
#define __UNIVERSE_DENSITY_RECTANGLE_HPP__

#include "universe/types.hpp"
#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A rectangle in the world, where the density is different from the
     *        default density.
     * \author Sebastien Angibaud.
     */
    class UNIVERSE_EXPORT density_rectangle
    {
    public:
      density_rectangle();
      density_rectangle( const rectangle_type& r, double f );

    public:
      /** \brief The rectangle where the density is different. */
      rectangle_type rectangle;

      /** \brief The density in this rectangle. */
      double density;

    }; // class density_rectangle
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_DENSITY_RECTANGLE_HPP__
