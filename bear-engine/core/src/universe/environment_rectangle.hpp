/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A rectangle in the world, where the environment is different from the
 *        default environment.
 * \author Sébastien Angibaud
 */
#ifndef __UNIVERSE_ENVIRONMENT_RECTANGLE_HPP__
#define __UNIVERSE_ENVIRONMENT_RECTANGLE_HPP__

#include "universe/types.hpp"
#include "universe/class_export.hpp"
#include "universe/environment_type.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A rectangle in the world, where the environment is different
     * from the default environment.
     * \author Sebastien Angibaud.
     */
    class UNIVERSE_EXPORT environment_rectangle
    {
    public:
      environment_rectangle();
      environment_rectangle
      ( const rectangle_type& r, const universe::environment_type e );

    public:
      /** \brief The rectangle where the environment is different. */
      rectangle_type rectangle;

      /** \brief The environment in this rectangle. */
      universe::environment_type environment;

    }; // class environment_rectangle
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ENVIRONMENT_RECTANGLE_HPP__
