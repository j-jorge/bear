/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to use OpenGL shader programs.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_SHADER_PROGRAM_HPP__
#define __VISUAL_GL_SHADER_PROGRAM_HPP__

#include "visual/base_shader_program.hpp"
#include "visual/gl_fragment_shader.hpp"
#include "visual/gl_vertex_shader.hpp"

#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class to use OpenGL shader programs. The implementation supports
     *        the fragment shaders only.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_shader_program:
      public base_shader_program
    {
    public:
      gl_shader_program
        ( const std::string& fragment_code, const std::string& vertex_code );
      
      ~gl_shader_program();
    
      GLuint program_id() const;

    private:
      void log_errors( std::string step ) const;

    private:
      /** \brief The OpenGL identifier of the shader program. */
      GLuint m_program_id;

      /** \brief The fragment shader linked to this program. */
      gl_fragment_shader m_fragment_shader;

      gl_vertex_shader m_vertex_shader;

    }; // gl_shader_program

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_SHADER_PROGRAM_HPP__
