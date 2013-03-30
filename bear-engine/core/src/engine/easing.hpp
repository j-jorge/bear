/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The types related to the easing functions.
 * \author Julien Jorge
 */
#ifndef __ENGINE_EASING_HPP__
#define __ENGINE_EASING_HPP__

#include <claw/tween/single_tweener.hpp>

namespace bear
{
  namespace engine
  {

    /** \brief The type of the easing functions passed to the tweeners. */
    typedef claw::tween::single_tweener::easing_function easing_function;

  } // namespace engine
} // namespace bear

#endif // __ENGINE_EASING_HPP__
