/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
#ifndef __VISUAL_GL_FRAGMENT_SHADER_LOADER_HPP__
#define __VISUAL_GL_FRAGMENT_SHADER_LOADER_HPP__

#include "visual/gl.hpp"

#include <iostream>

namespace bear
{
  namespace visual
  {
    /**
     * \brief The gl_fragment_shader_loader loads a fragment shader from a GLSL
     *        code.
     */
    class gl_fragment_shader_loader
    {
    public:
      GLuint load( std::istream& p ) const;

    private:
      void log_errors( GLuint shader_id ) const;

    }; // gl_fragment_shader_loader

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_FRAGMENT_SHADER_LOADER_HPP__

