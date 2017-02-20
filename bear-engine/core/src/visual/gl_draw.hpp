#pragma once

#include "visual/gl.hpp"
#include "visual/shader_program.hpp"
#include "visual/types.hpp"

#include <vector>

namespace bear
{
  namespace visual
  {
    class gl_state;
    
    class gl_draw
    {
    public:
      explicit gl_draw( GLuint white );

      void set_background_color( const color_type& c );
      void set_default_shader( const shader_program& shader );
      
      void draw( const std::vector< gl_state >& states );
      
      void set_vertices( const std::vector< GLfloat >& vertices );
      void set_colors( const std::vector< GLfloat >& colors );
      void set_texture_coordinates( const std::vector< GLfloat >& coordinates );

      void draw( GLenum mode, GLuint first, GLuint count );

    private:
      void prepare();
      void finalize();
      
      void generate_indices();
      
    private:
      const GLuint m_white;

      const shader_program* m_shader;
      
      GLfloat m_background_color[ 4 ];
      GLuint m_buffers[ 4 ];
      std::vector< GLushort > m_indices;

      std::size_t m_vertex_count;
      std::size_t m_color_count;
      std::size_t m_texture_coordinate_count;
    };
  }
}
