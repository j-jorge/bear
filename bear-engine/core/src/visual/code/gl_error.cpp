/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_error class.
 * \author Julien Jorge
 */
#include "visual/gl_error.hpp"

#include <claw/exception.hpp>
#include <sstream>

#include "visual/gl.hpp"
#include <GL/glext.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Throws an exception if there is a problem with OpenGL.
 * \param prefix A prefix to the message of the exception.
 */
void bear::visual::gl_error::throw_on_error( const std::string& prefix )
{
  const GLenum err = glGetError();

  if ( err != GL_NO_ERROR )
    {
      std::ostringstream err_string;
      err_string << prefix << ": ";

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
        case GL_STACK_OVERFLOW: err_string << "stack overflow.";
          break;
        case GL_STACK_UNDERFLOW: err_string << "stack underflow.";
          break;
        case GL_OUT_OF_MEMORY:
          err_string << "not enough memory to execute the command.";
          break;
        case GL_TABLE_TOO_LARGE:
          err_string <<
            "table exceeds the implementation's maximum supported table size.";
          break;
        default:
          err_string << "unknow error code " << err << '.';
        }

      throw claw::exception( err_string.str() );
    }
} // gl_error::throw_on_error()
