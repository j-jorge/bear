/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::sdl_error class.
 * \author Julien Jorge
 */
#include "visual/sdl_error.hpp"

#include <claw/exception.hpp>
#include <sstream>

#include <SDL/SDL.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Throws an exception if there is a problem with SDL.
 * \param function The name of the function in which the error occured.
 * \param line The line at which the error is detected.
 */
void bear::visual::sdl_error::throw_on_error
( const std::string& function, unsigned int line )
{
  std::ostringstream err_string;
  err_string << function << ":" << line << ": " << SDL_GetError();

  throw claw::exception( err_string.str() );
} // sdl_error::throw_on_error()
