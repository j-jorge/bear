/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to use OpenGL vertex shaders.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_VERTEX_SHADER_HPP__
#define __VISUAL_GL_VERTEX_SHADER_HPP__

#include "visual/gl.hpp"
#include "visual/class_export.hpp"

#include <string>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A vertex shader finalizes the rendering of the pixels.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_vertex_shader
    {
    public:
      gl_vertex_shader( const std::string& p );
      ~gl_vertex_shader();
    
      GLuint shader_id() const;

    private:
      /** \brief The OpenGL identifier of the vertex shader. */
      GLuint m_vertex_shader;
    
    }; // gl_vertex_shader

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_VERTEX_SHADER_HPP__
