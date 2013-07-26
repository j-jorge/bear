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
#include "visual/gl_shader_program.hpp"

#include "visual/gl_error.hpp"

#include <string>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new program.
 * \param program_code The code of the shader program.
 */
bear::visual::gl_shader_program::gl_shader_program( std::istream& program_code )
  : m_fragment_shader(program_code)
{
  m_program_id = glCreateProgram();

  VISUAL_GL_ERROR_THROW();
  glAttachShader( m_program_id, m_fragment_shader.shader_id() );
  VISUAL_GL_ERROR_THROW();

  glLinkProgram( m_program_id );
  log_errors( "link" );

  glValidateProgram( m_program_id );
  log_errors( "validation" );
} // gl_shader_program::gl_shader_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cleans up the program and destroys the GL variables.
 */
bear::visual::gl_shader_program::~gl_shader_program()
{
  if ( !glIsProgram( m_program_id ) )
    return;

  GLint shader_count;

  glGetProgramiv( m_program_id, GL_ATTACHED_SHADERS, &shader_count );

  if ( shader_count != 0 )
    {
      GLuint* shaders = new GLuint[ shader_count ];
      glGetAttachedShaders( m_program_id, shader_count, NULL, shaders );

      for ( GLint i(0); i != shader_count; ++i )
        glDetachShader( m_program_id, shaders[i] );
    }

  glDeleteProgram( m_program_id );
} // gl_shader_program::~gl_shader_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the OpenGL identifier of the program.
 */
GLuint bear::visual::gl_shader_program::program_id() const
{
  return m_program_id;
} // gl_shader_program::program_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logs the validation errors of the shader.
 * \param step The name of the step that has generated the logs.
 */
void bear::visual::gl_shader_program::log_errors( std::string step ) const
{
  GLint buffer_size;
  glGetProgramiv( m_program_id, GL_INFO_LOG_LENGTH, &buffer_size );

  if ( buffer_size <= 1 )
    return;

  char* buffer = new char[ buffer_size ];

  glGetProgramInfoLog( m_program_id, buffer_size, NULL, buffer );

  claw::logger << claw::log_error << "Program " << m_program_id << ' ' << step
               << " errors: " << buffer << std::endl;
} // gl_shader_program::log_errors()
