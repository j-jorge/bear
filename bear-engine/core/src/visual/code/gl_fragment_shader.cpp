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

#include "visual/gl_renderer.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new fragment shader from a given source.
 * \param p The stream from which is read the shader.
 */
bear::visual::gl_fragment_shader::gl_fragment_shader( std::istream& p )
{
  m_fragment_shader = gl_renderer::get_instance().create_fragment_shader( p );
} // gl_fragment_shader::gl_fragment_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor. Cleans up the GL variables.
 */
bear::visual::gl_fragment_shader::~gl_fragment_shader()
{
  gl_renderer::get_instance().delete_fragment_shader( m_fragment_shader );
} // gl_fragment_shader::~gl_fragment_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the OpenGL identifier of the shader.
 */
GLuint bear::visual::gl_fragment_shader::shader_id() const
{
  return m_fragment_shader;
} // gl_fragment_shader::shader_id()
