/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_fragment_shader class.
 * \author Julien Jorge
 */
#include "visual/gl_fragment_shader_loader.hpp"

#include "visual/gl_error.hpp"

#include <string>
#include <sstream>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new fragment shader from a given source.
 * \param p The stream from which is read the shader.
 */
GLuint bear::visual::gl_fragment_shader_loader::load( std::istream& p ) const
{
  const GLuint result( glCreateShader( GL_FRAGMENT_SHADER ) );
  VISUAL_GL_ERROR_THROW();

  std::ostringstream oss;
  oss << p.rdbuf();

  const std::string code( oss.str() );
  const char *fragmentText = code.c_str();

  glShaderSource( result, 1, &fragmentText, 0 );
  VISUAL_GL_ERROR_THROW();

  glCompileShader( result );
  log_errors( result);

  return result;
} // gl_fragment_shader_loader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logs the compilation errors of the shader.
 * \param shader_id The identifier of the shader of which we read the logs.
 */
void bear::visual::gl_fragment_shader_loader::log_errors
( GLuint shader_id ) const
{
  GLint buffer_size;
  glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &buffer_size );

  if ( buffer_size <= 1 )
    return;

  char* const buffer = new char[ buffer_size ];

  glGetShaderInfoLog( shader_id, buffer_size, NULL, buffer );

  claw::logger << claw::log_error << "Shader " << shader_id
               << " compile error: " << buffer << std::endl;

  delete[] buffer;
} // gl_fragment_shader_loader::log_errors()
