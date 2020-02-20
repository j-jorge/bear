/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item loader for a claw::tween::single_tweener.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SINGLE_TWEENER_LOADER_HPP__
#define __ENGINE_SINGLE_TWEENER_LOADER_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/engine/loader/item_loader_base.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The item loader for the claw::tween::single_tweener.
     *
     * The custom fields of this class are:
     * - origin: real, the initial value of the tweener (default = 0),
     * - target: real, the final value of the tweener (default = 1),
     * - duration: real, the duration of the interpolation in seconds
     *   (default = 1),
     * - easing: easing_function, the easing function applied to the tweener
     *   (default = none).
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT single_tweener_loader:
      public item_loader_base
    {
    public:
      single_tweener_loader
        ( std::string prefix, claw::tween::single_tweener& tweener );

      virtual single_tweener_loader* clone() const;

      virtual bool set_field( const std::string& name, double value );
      virtual bool set_field( const std::string& name, easing_function value );

    private:
      /** \brief The tweener loaded by this loader. */
      claw::tween::single_tweener& m_tweener;

    }; // class single_tweener_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SINGLE_TWEENER_LOADER_HPP__
