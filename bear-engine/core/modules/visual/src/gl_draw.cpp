#include "bear/visual/gl_draw.hpp"

#include "bear/visual/gl_error.hpp"
#include "bear/visual/gl_state.hpp"
#include "bear/visual/detail/gl_vertex_attribute_index.hpp"

#include <cassert>
#include <limits>
#include <numeric>

#include <iostream>

namespace bear
{
  namespace visual
  {
    namespace detail
    {
      static constexpr int vertex_index = 0;
      static constexpr int color_index = 1;
      static constexpr int texture_coordinate_index = 2;
      static constexpr int elements_index = 3;

      static constexpr int vertex_size = 2;
      static constexpr int color_size = 4;
      static constexpr int texture_coordinate_size = 2;
    }
  }
}

bear::visual::gl_draw::gl_draw
( GLuint white, GLuint shader,
  const claw::math::coordinate_2d< unsigned int >& size )
  : m_white( white ),
    m_shader( shader ),
    m_background_color{ 0, 0, 0, 0 },
    m_vertex_count( 0 ),
    m_color_count( 0 ),
    m_texture_coordinate_count( 0 )
{
  glGenBuffers( 4, m_buffers );
  VISUAL_GL_ERROR_THROW();

  set_viewport( size );
}

void bear::visual::gl_draw::set_background_color( const color_type& c )
{
  static constexpr const GLfloat max
    ( std::numeric_limits< color_type::component_type >::max() );

  m_background_color[ 0 ] = ( GLfloat )c.components.red / max;
  m_background_color[ 1 ] = ( GLfloat )c.components.green / max;
  m_background_color[ 2 ] = ( GLfloat )c.components.blue / max;
  m_background_color[ 3 ] = ( GLfloat )c.components.alpha / max;
}

void bear::visual::gl_draw::draw( const std::vector< gl_state >& states )
{
  glClearColor
    ( m_background_color[ 0 ], m_background_color[ 1 ], m_background_color[ 2 ],
      m_background_color[ 3 ] );
  VISUAL_GL_ERROR_THROW();

  glClear( GL_COLOR_BUFFER_BIT );
  VISUAL_GL_ERROR_THROW();

  for ( const gl_state& state : states )
    {
      prepare();
      glUseProgram( m_shader );
      VISUAL_GL_ERROR_THROW();
  
      state.draw( *this );
      VISUAL_GL_ERROR_THROW();

      finalize();
    }
}

void bear::visual::gl_draw::set_vertices
( const std::vector< GLfloat >& vertices )
{
  assert( vertices.size() % detail::vertex_size == 0 );
  
  glBindBuffer( GL_ARRAY_BUFFER, m_buffers[ detail::vertex_index ] );
  VISUAL_GL_ERROR_THROW();

  glBufferData
    ( GL_ARRAY_BUFFER, vertices.size() * sizeof( GLfloat ), vertices.data(),
      GL_DYNAMIC_DRAW );
  VISUAL_GL_ERROR_THROW();

  glVertexAttribPointer
    ( detail::position_attribute, detail::vertex_size, GL_FLOAT, GL_FALSE, 0,
      nullptr );
  VISUAL_GL_ERROR_THROW();

  glEnableVertexAttribArray( detail::position_attribute );
  VISUAL_GL_ERROR_THROW();

  m_vertex_count = vertices.size() / detail::vertex_size;
}

void bear::visual::gl_draw::set_colors
( const std::vector< GLfloat >& colors )
{
  assert( colors.size() % detail::color_size == 0 );

  glBindBuffer( GL_ARRAY_BUFFER, m_buffers[ detail::color_index ] );
  VISUAL_GL_ERROR_THROW();

  glBufferData
    ( GL_ARRAY_BUFFER, colors.size() * sizeof( GLfloat ), colors.data(),
      GL_DYNAMIC_DRAW );
  VISUAL_GL_ERROR_THROW();

  glVertexAttribPointer
    ( detail::color_attribute, detail::color_size, GL_FLOAT, GL_FALSE, 0,
      nullptr );
  VISUAL_GL_ERROR_THROW();

  glEnableVertexAttribArray( detail::color_attribute );
  VISUAL_GL_ERROR_THROW();

  m_color_count = colors.size() / detail::color_size;
}

void bear::visual::gl_draw::set_texture_coordinates
( const std::vector< GLfloat >& coordinates )
{
  assert( coordinates.size() % detail::texture_coordinate_size == 0 );

  glBindBuffer
    ( GL_ARRAY_BUFFER, m_buffers[ detail::texture_coordinate_index ] );
  VISUAL_GL_ERROR_THROW();

  glBufferData
    ( GL_ARRAY_BUFFER, coordinates.size() * sizeof( GLfloat ),
      coordinates.data(), GL_DYNAMIC_DRAW );
  VISUAL_GL_ERROR_THROW();

  glVertexAttribPointer
    ( detail::texture_coordinate_attribute, detail::texture_coordinate_size,
      GL_FLOAT, GL_FALSE, 0, nullptr );
  VISUAL_GL_ERROR_THROW();

  glEnableVertexAttribArray( detail::texture_coordinate_attribute );
  VISUAL_GL_ERROR_THROW();

  m_texture_coordinate_count =
    coordinates.size() / detail::texture_coordinate_size;
}

void bear::visual::gl_draw::draw( GLenum mode, GLuint first, GLuint count )
{
  assert( first + count <= m_vertex_count );
  assert( m_vertex_count != 0 );
  
  if ( m_texture_coordinate_count == 0 )
    glBindTexture( GL_TEXTURE_2D, m_white );
  
  generate_indices();

  glDrawElements
    ( mode, count, GL_UNSIGNED_SHORT,
      reinterpret_cast< GLvoid* >( first * sizeof( GLushort ) ) );
  VISUAL_GL_ERROR_THROW();
}

void bear::visual::gl_draw::set_viewport
( const claw::math::coordinate_2d< unsigned int >& size )
{
  const GLfloat m00( GLfloat( 2 ) / size.x );
  const GLfloat m11( GLfloat( 2 ) / size.y );

  const std::array< float, 16 > transform =
    {
      m00,   0,  0,  0,
        0, m11,  0,  0,
        0,   0, -2,  0,
       -1,  -1,  1,  1
    };

  glUseProgram( m_shader );
  VISUAL_GL_ERROR_THROW();
  
  glUniformMatrix4fv
    ( glGetUniformLocation( m_shader, "transform" ), 1, GL_FALSE,
      transform.data() );
  VISUAL_GL_ERROR_THROW();
}

void bear::visual::gl_draw::prepare()
{
  m_vertex_count = 0;
  m_color_count = 0;
  m_texture_coordinate_count = 0;
}

void bear::visual::gl_draw::finalize()
{
  glDisableVertexAttribArray( detail::texture_coordinate_attribute );
  glDisableVertexAttribArray( detail::color_attribute );
  glDisableVertexAttribArray( detail::position_attribute );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void bear::visual::gl_draw::generate_indices()
{
  assert( ( m_vertex_count == m_color_count ) || ( m_color_count == 0 ) );
  assert
    ( ( m_vertex_count == m_texture_coordinate_count )
      || ( m_texture_coordinate_count == 0 ) );

  const std::size_t count( m_vertex_count );
  
  assert( count <= std::numeric_limits< GLushort >::max() );
  
  if ( count > m_indices.size() )
    {
      const std::size_t old_count( m_indices.size() );
      m_indices.resize( count );
      std::iota( m_indices.begin() + old_count, m_indices.end(), old_count );
    }

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_buffers[ detail::elements_index ] );
  VISUAL_GL_ERROR_THROW();

  glBufferData
    ( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( GLushort ), m_indices.data(),
      GL_STATIC_DRAW );
  VISUAL_GL_ERROR_THROW();
}
