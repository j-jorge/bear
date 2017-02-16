#pragma once

#include "visual/gl.hpp"

#include <vector>

namespace bear
{
  namespace visual
  {
    class gl_draw
    {
    public:
      explicit gl_draw( GLuint white );

      void prepare();
      void finalize();
      
      void set_vertices( const std::vector< GLfloat >& vertices );
      void set_colors( const std::vector< GLfloat >& colors );
      void set_texture_coordinates( const std::vector< GLfloat >& coordinates );

      void draw( GLenum mode, GLuint first, GLuint count );

    private:
      void generate_indices();
      
    private:
      const GLuint m_white;
      
      GLuint m_buffers[ 4 ];
      std::vector< GLushort > m_indices;

      std::size_t m_vertex_count;
      std::size_t m_color_count;
      std::size_t m_texture_coordinate_count;
    };
  }
}
