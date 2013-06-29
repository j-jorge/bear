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
#include "visual/gl_fragment_shader.hpp"

#include <string>
#include <sstream>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new fragment shader from a given source.
 * \param p The stream from which is read the shader.
 */
bear::visual::gl_fragment_shader::gl_fragment_shader( std::istream& p )
{
  m_fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );

  std::ostringstream oss;
  oss << p.rdbuf();

  const std::string code( oss.str() );
  const char *fragmentText = code.c_str();

  glShaderSource( m_fragment_shader, 1, &fragmentText, 0 );
  glCompileShader( m_fragment_shader );
  log_errors();
} // gl_fragment_shader::gl_fragment_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor. Cleans up the GL variables.
 */
bear::visual::gl_fragment_shader::~gl_fragment_shader()
{
  glDeleteShader( m_fragment_shader );
} // gl_fragment_shader::~gl_fragment_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the OpenGL identifier of the shader.
 */
GLuint bear::visual::gl_fragment_shader::shader_id() const
{
  return m_fragment_shader;
} // gl_fragment_shader::shader_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logs the compilation errors of the shader.
 */
void bear::visual::gl_fragment_shader::log_errors() const
{
  GLint buffer_size;
  glGetShaderiv( m_fragment_shader, GL_INFO_LOG_LENGTH, &buffer_size );

  if ( buffer_size <= 1 )
    return;

  char* buffer = new char[ buffer_size ];

  glGetShaderInfoLog( m_fragment_shader, buffer_size, NULL, buffer );

  claw::logger << claw::log_error << "Shader " << m_fragment_shader
               << " compile error: " << buffer << std::endl;

  delete[] buffer;
} // gl_fragment_shader::log_errors()
