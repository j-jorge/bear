#include "visual/gl_draw.hpp"

#include "visual/gl_error.hpp"

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

      static constexpr int position_attribute = 0;
      static constexpr int color_attribute = 1;
      static constexpr int texture_coordinate_attribute = 2;
    }
  }
}

bear::visual::gl_draw::gl_draw( GLuint white )
  : m_white( white ),
    m_vertex_count( 0 ),
    m_color_count( 0 ),
    m_texture_coordinate_count( 0 )
{
  glGenBuffers( 4, m_buffers );
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
  
  if ( m_vertex_count == 0 )
    return;

  if ( m_texture_coordinate_count == 0 )
    glBindTexture( GL_TEXTURE_2D, m_white );
  
  generate_indices();

  glDrawElements
    ( mode, count, GL_UNSIGNED_SHORT,
      reinterpret_cast< GLvoid* >( first * sizeof( GLushort ) ) );
  VISUAL_GL_ERROR_THROW();
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
