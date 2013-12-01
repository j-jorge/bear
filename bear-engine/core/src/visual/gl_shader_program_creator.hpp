/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
#ifndef __VISUAL_GL_SHADER_PROGRAM_CREATOR_HPP__
#define __VISUAL_GL_SHADER_PROGRAM_CREATOR_HPP__

#include "visual/gl.hpp"

#include <iostream>
#include <string>

namespace bear
{
  namespace visual
  {
    class gl_fragment_shader;

    /**
     * \brief The gl_shader_program_creator creates a shader program from a
     *        given shader.
     */
    class gl_shader_program_creator
    {
    public:
      GLuint create( const gl_fragment_shader& s ) const;

    private:
      void log_errors( std::string step, GLuint program_id ) const;

    }; // gl_shader_program_creator

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_SHADER_PROGRAM_CREATOR_HPP__

