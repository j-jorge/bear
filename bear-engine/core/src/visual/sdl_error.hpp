/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Error management for SDL.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SDL_ERROR_HPP__
#define __VISUAL_SDL_ERROR_HPP__

#include "visual/class_export.hpp"

#include <string>

#define VISUAL_SDL_ERROR_THROW()                                \
  bear::visual::sdl_error::throw_on_error                       \
  ( std::string(__FILE__) + ':' + __FUNCTION__, __LINE__ )

namespace bear
{
  namespace visual
  {
    /**
     * \brief Error management for SDL.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT sdl_error
    {
    public:
      static void throw_on_error
        ( const std::string& function, unsigned int line );

    }; // class sdl_error

  } // namespace visual
} // namespace bear

#endif // __VISUAL_SDL_ERROR_HPP__
