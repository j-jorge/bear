/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_shader_program class.
 * \author Julien Jorge
 */
#include "visual/gl_shader_program_creator.hpp"

#include "visual/gl_error.hpp"
#include "visual/gl_fragment_shader.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new program.
 * \param s The shader to link into the program.
 */
GLuint bear::visual::gl_shader_program_creator::create
( const gl_fragment_shader& s ) const
{
  const GLuint result( glCreateProgram() );
  VISUAL_GL_ERROR_THROW();

  glAttachShader( result, s.shader_id() );
  VISUAL_GL_ERROR_THROW();

  glLinkProgram( result );
  log_errors( "link", result );

  glValidateProgram( result );
  log_errors( "validation", result );

  return result;
} // gl_shader_program_creator::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logs the validation errors of the shader.
 * \param step The name of the step that has generated the logs.
 * \param program_id The identifier of the program from which we read the logs.
 */
void bear::visual::gl_shader_program_creator::log_errors
( std::string step, GLuint program_id ) const
{
  GLint buffer_size;
  glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &buffer_size );

  if ( buffer_size <= 1 )
    return;

  char* const buffer = new char[ buffer_size ];

  glGetProgramInfoLog( program_id, buffer_size, NULL, buffer );

  claw::logger << claw::log_error << "Program " << program_id << ' ' << step
               << " errors: " << buffer << std::endl;

  delete[] buffer;
} // gl_shader_program_creator::log_errors()
