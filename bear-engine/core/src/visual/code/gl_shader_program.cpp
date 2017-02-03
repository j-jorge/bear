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

#include "visual/gl_renderer.hpp"

bear::visual::gl_shader_program::gl_shader_program
( const std::string& fragment_code, const std::string& vertex_code )
  : m_fragment_shader( fragment_code ),
    m_vertex_shader( vertex_code )
{
  m_program_id =
    gl_renderer::get_instance().create_shader_program
    ( m_fragment_shader, m_vertex_shader );
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Cleans up the program and destroys the GL variables.
 */
bear::visual::gl_shader_program::~gl_shader_program()
{
  gl_renderer::get_instance().delete_shader_program( m_program_id );
} // gl_shader_program::~gl_shader_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the OpenGL identifier of the program.
 */
GLuint bear::visual::gl_shader_program::program_id() const
{
  return m_program_id;
} // gl_shader_program::program_id()
