/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief OpenGL implementation of the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_SCREEN_HPP__
#define __VISUAL_GL_SCREEN_HPP__

#include "bear/visual/base_screen.hpp"
#include "bear/visual/gl.hpp"
#include "bear/visual/gl_state.hpp"
#include "bear/visual/shader_program.hpp"

#include <SDL2/SDL.h>

namespace bear
{
  namespace visual
  {
    /**
     * \brief OpenGL implementation of the screen.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_screen:
      public base_screen
    {
    private:
      /** \brief The type used to represent the size of a screen. */
      typedef claw::math::coordinate_2d<unsigned int> screen_size_type;

    public:
      static void initialize();
      static void release();

      gl_screen( const claw::math::coordinate_2d<unsigned int>& size,
                 const std::string& title="", bool full=false );

      void pause() override;
      void unpause() override;
      
      void fullscreen( bool b ) override;
      claw::math::coordinate_2d<unsigned int> get_size() const override;
      claw::math::coordinate_2d<unsigned int>
        get_viewport_size() const override;
      claw::math::coordinate_2d<unsigned int>
        get_container_size() const override;

      void set_background_color( const color_type& c ) override;
      color_type get_background_color() const override;

      void begin_render() override;
      void render( const position_type& pos, const sprite& s ) override;
      void end_render() override;

      void draw_line
      ( const color_type& color,
        const std::vector<position_type>& p, double w = 1.0,
        bool close=false ) override;

      void draw_polygon
      ( const color_type& color, const std::vector<position_type>& p ) override;

      void push_shader( const shader_program& p ) override;
      void pop_shader() override;

      void shot( claw::graphic::image& img ) const override;
      capture capture_scene() const override;

    private:
      void render_sprite( const position_type& pos, const sprite& s );
      
      void get_render_coord
        ( const position_type& pos, const sprite& s,
          std::vector<position_type>& result ) const;
      claw::math::box_2d<GLfloat> get_texture_clip( const sprite& s ) const;

      void render_image
        ( GLuint texture_id, const std::vector<position_type>& render_coord,
          const claw::math::box_2d<GLfloat>& clip,
          const color_type& color );

      position_type
      rotate( const position_type& pos, GLfloat a,
              const position_type& center ) const;

      std::vector<position_type>
        get_texture_coordinates
        ( const claw::math::box_2d<GLfloat>& clip ) const;

      void push_state( const gl_state& state );
      shader_program get_current_shader() const;

    private:
      /** \brief The shaders to apply to the next rendering commands. */
      std::vector<shader_program> m_shader;

      /** \brief The OpenGL drawing commands. */
      std::vector<gl_state> m_gl_state;

    }; // class gl_screen
  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_SCREEN_HPP__
