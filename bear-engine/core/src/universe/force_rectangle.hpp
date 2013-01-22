/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A rectangle in the world, where a force is applied.
 * \author Sebastien Angibaud
 */
#ifndef __UNIVERSE_FORCE_RECTANGLE_HPP__
#define __UNIVERSE_FORCE_RECTANGLE_HPP__

#include "universe/types.hpp"
#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A rectangle in the world, where a force is applied.
     * \author Sebastien Angibaud.
     */
    class UNIVERSE_EXPORT force_rectangle
    {
    public:
      force_rectangle();
      force_rectangle( const rectangle_type& r, universe::force_type f );

    public:
      /** \brief The rectangle where the force is different. */
      rectangle_type rectangle;

      /** \brief The force in this rectangle. */
      universe::force_type force;

    }; // class force_rectangle
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCE_RECTANGLE_HPP__
