/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The description of an element in the OpenGL scene.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_STATE_HPP__
#define __VISUAL_GL_STATE_HPP__

#include "visual/gl.hpp"
#include "visual/shader_program.hpp"
#include "visual/types.hpp"

#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief The description of an element in the OpenGL scene.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_state
    {
    public:
      typedef std::vector<position_type> position_vector;

    private:
      /** \brief The various ways to render the vertices. */
      enum render_mode
      {
        render_lines,
        render_triangles
      }; // enum render_mode

      /**
       * \brief The visitor that sets the variables of a shader program.
       */
      class uniform_setter
      {
      public:
        explicit uniform_setter( GLuint program );

        void operator()( std::string name, int value ) const;
        void operator()( std::string name, double value ) const;
        void operator()( std::string name, bool value ) const;

      private:
        /** \brief The identifier of the shader program in which the variables
            are set. */
        const GLuint m_program;

      }; // class uniform_setter

      /**
       * \brief The visitor that checks if a variable exists in another
       *        variable_map with the same value.
       */
      class variables_are_included
      {
      public:
        variables_are_included
        ( bool& result, const shader_program::input_variable_map& vars );

        template<typename T>
        void operator()( std::string name, T value ) const;

      private:
        /** \brief The updated result of the tests. */
        bool& m_result;

        /** \brief The variable_map to which the visited variabled are
            compared. */
        const shader_program::input_variable_map& m_variables;

      }; // class variables_are_included

    public:
      gl_state
        ( const shader_program& shader, const position_vector& vertices,
          const color_type& c );

      gl_state
        ( const shader_program& shader, const position_vector& vertices,
          const color_type& c, double line_width );
      
      gl_state
        ( GLuint texture_id, const shader_program& shader,
          const position_vector& texture_coordinates,
          const position_vector& vertices,
          const color_type& c );

      bool is_compatible_with( const gl_state& state ) const;

      void merge( const gl_state& state );

      void draw() const;

    private:
      GLenum get_gl_render_mode() const;
      
      void push_vertices( const position_vector& v );
      void push_texture_coordinates( const position_vector& v );
      void push_colors( const color_type& c, std::size_t count );

      position_vector polygon_to_triangles( const position_vector& v ) const;

      void enable_shader() const;

    private:
      /** \brief The number of coordinates in a vertex. */
      static const std::size_t s_vertex_size;

      /** \brief Tells how to render the vertices. */
      render_mode m_mode;

      /** \brief OpenGL texture identifier. */
      GLuint m_texture_id;

      /** \brief The identifier of the shader in use. */
      shader_program m_shader;

      /** \brief The vertices of the shape to render. */
      std::vector<GLfloat> m_vertices;

      /** \brief The coordinates of the vertices in the texture. */
      std::vector<GLfloat> m_texture_coordinates;

      /** \brief The colors of the vertices. */
      std::vector<GLfloat> m_colors;

      /** \brief The width of the line to draw. */
      double m_line_width;

    }; // class gl_state
  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_STATE_HPP__
