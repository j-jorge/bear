/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief OpenGL implementation of an image.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_IMAGE_HPP__
#define __VISUAL_GL_IMAGE_HPP__

#include "visual/base_image.hpp"

#include "visual/gl.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief OpenGL implementation of an image.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_image:
      public base_image
    {
    public:
      gl_image( unsigned int width, unsigned int height );
      explicit gl_image( const claw::graphic::image& data );
      ~gl_image();

      GLuint texture_id() const;
      claw::math::coordinate_2d<unsigned int> size() const;
      bool has_transparency() const;

      void draw
        ( const claw::graphic::image& data,
          claw::math::coordinate_2d<unsigned int> pos );
      claw::graphic::image read() const;

    private:
      void create_texture();
      void copy_scanlines( const claw::graphic::image& pixels );

    private:
      /** \brief OpenGL texture identifier. */
      GLuint m_texture_id;

      /** \brief Image's size. */
      claw::math::coordinate_2d<unsigned int> m_size;

      /** \brief Is there any transparent pixel in the image ? */
      bool m_has_transparency;

    }; // class gl_image
  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_IMAGE_HPP__
