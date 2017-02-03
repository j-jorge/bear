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
    class gl_draw;
    
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
       * \brief The visitor that checks if a variable exists in another
       *        variable_map with the same value.
       */
      class variables_are_included
      {
      public:
        variables_are_included
        ( bool& result, const shader_program::input_variable_map& vars );

        template<typename T>
        void operator()( const std::string& name, const T& value ) const;

      private:
        /** \brief The updated result of the tests. */
        bool& m_result;

        /** \brief The variable_map to which the visited variabled are
            compared. */
        const shader_program::input_variable_map& m_variables;

      }; // class variables_are_included

      /**
       * \brief The element_range class describes how to render a given range
       *        of vertices from the state.
       */
      struct element_range
      {
        element_range( GLuint t, std::size_t i, std::size_t c );

        /** \brief The texture to use when rendering the vertices. */
        GLuint texture_id;

        /** \brief The index of the first vertex to render. */
        std::size_t vertex_index;

        /** \brief The count of vertices to render. */
        std::size_t count;

      }; // struct element_range

      typedef std::vector<element_range> element_range_list;

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

      void draw( gl_draw& output ) const;

    private:
      void draw_shape( gl_draw& output ) const;
      void draw_textured( gl_draw& output ) const;

      void set_colors( gl_draw& output ) const;
      void set_vertices( gl_draw& output ) const;
      void set_texture_coordinates( gl_draw& output ) const;

      std::size_t get_vertex_count() const;
      GLenum get_gl_render_mode() const;
      
      void push_vertices( const position_vector& v );
      void push_texture_coordinates( const position_vector& v );
      void push_colors( const color_type& c, std::size_t count );

      position_vector polygon_to_triangles( const position_vector& v ) const;

    private:
      /** \brief The number of coordinates in a vertex. */
      static const std::size_t s_vertex_size;

      /** \brief Tells how to render the vertices. */
      render_mode m_mode;

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

      /** \brief How to render the vertices in the state. */
      element_range_list m_elements;

    }; // class gl_state
  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_STATE_HPP__
