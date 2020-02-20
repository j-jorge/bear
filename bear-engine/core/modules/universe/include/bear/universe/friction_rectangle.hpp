/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A rectangle in the world, where the friction is different from the
 *        default friction.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FRICTION_RECTANGLE_HPP__
#define __UNIVERSE_FRICTION_RECTANGLE_HPP__

#include "bear/universe/types.hpp"
#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A rectangle in the world, where the friction is different from the
     *        default friction.
     * \author Julien Jorge.
     */
    class UNIVERSE_EXPORT friction_rectangle
    {
    public:
      friction_rectangle();
      friction_rectangle( const rectangle_type& r, double f );

    public:
      /** \brief The rectangle where the friction is different. */
      rectangle_type rectangle;

      /** \brief The friction in this rectangle. */
      double friction;

    }; // class friction_rectangle
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FRICTION_RECTANGLE_HPP__
