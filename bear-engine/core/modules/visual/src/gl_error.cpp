/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_error class.
 * \author Julien Jorge
 */
#include "bear/visual/gl_error.hpp"

#include <claw/exception.hpp>
#include <claw/logger/logger.hpp>
#include <sstream>

#include "bear/visual/gl.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Throws an exception if there is a problem with OpenGL.
 * \param line The line where the error occurred.
 * \param file The file where the error occurred.
 */
void bear::visual::gl_error::throw_on_error
( std::size_t line, const std::string& file )
{
  const GLenum err = glGetError();

  if ( err != GL_NO_ERROR )
    {
      std::ostringstream err_string;
      err_string << file << ':' << line << ": ";

      switch (err)
        {
        case GL_NO_ERROR:
          err_string << "no error (?).";
          break;
        case GL_INVALID_ENUM:
          err_string <<
            "unacceptable value is specified for an enumerated argument.";
          break;
        case GL_INVALID_VALUE:
          err_string << "numeric argument is out of range.";
          break;
        case GL_INVALID_OPERATION:
          err_string << "operation is not allowed in the current state.";
          break;
        case GL_OUT_OF_MEMORY:
          err_string << "not enough memory to execute the command.";
          break;

#ifdef GL_STACK_OVERFLOW
        case GL_STACK_OVERFLOW: err_string << "stack overflow.";
          break;
#endif

#ifdef GL_STACK_UNDERFLOW
        case GL_STACK_UNDERFLOW: err_string << "stack underflow.";
          break;
#endif

#ifdef GL_TABLE_TOO_LARGE
        case GL_TABLE_TOO_LARGE:
          err_string <<
            "table exceeds the implementation's maximum supported table size.";
          break;
#endif

        default:
          err_string << "unknow error code " << err << '.';
        }

      claw::logger << claw::log_error << err_string.str() << std::endl;

      throw claw::exception( err_string.str() );
    }
} // gl_error::throw_on_error()
