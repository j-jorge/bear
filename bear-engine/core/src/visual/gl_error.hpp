/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Error management for OpenGL.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_ERROR_HPP__
#define __VISUAL_GL_ERROR_HPP__

#include "visual/class_export.hpp"

#include <string>

#define VISUAL_GL_ERROR_THROW() \
  bear::visual::gl_error::throw_on_error( __FUNCTION__ )

namespace bear
{
  namespace visual
  {
    /**
     * \brief Error management for OpenGL.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_error
    {
    public:
      static void throw_on_error( const std::string& prefix );

    }; // class gl_error

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_ERROR_HPP__
