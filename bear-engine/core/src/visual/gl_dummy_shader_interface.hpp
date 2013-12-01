/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Empty interface of GLSL support by OpenGL for the platforms which do
 *        not support shaders.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_DUMMY_SHADER_INTERFACE_HPP__
#define __VISUAL_GL_DUMMY_SHADER_INTERFACE_HPP__

#error "eee"
#define GL_FRAGMENT_SHADER 0
#define GL_ATTACHED_SHADERS 0
#define GL_INFO_LOG_LENGTH 0

#define glCreateShader( a ) 0
#define glShaderSource( a, b, c, d )
#define glCompileShader( a )
#define glIsShader( a ) true
#define glDeleteShader( a )
#define glGetShaderiv( a, b, c ) *(c) = 0
#define glGetShaderInfoLog( a, b, c, d )
#define glGetUniformLocation( a, b )
#define glUniform1i( a, b )
#define glUniform1f( a, b )

#define glCreateProgram( a ) 0
#define glDeleteProgram( a )
#define glAttachShader( a, b )
#define glDetachShader( a, b )
#define glLinkProgram( a )
#define glValidateProgram( a )
#define glIsProgram( a ) true
#define glGetProgramiv( a, b, c ) *(c) = 0
#define glGetProgramInfoLog( a, b, c, d )
#define glGetAttachedShaders( a, b, c, d )

#define glUseProgram( a )

#endif // __VISUAL_GL_DUMMY_SHADER_INTERFACE_HPP__
