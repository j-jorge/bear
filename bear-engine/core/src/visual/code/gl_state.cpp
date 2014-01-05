/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_state class.
 * \author Julien Jorge
 */
#include "visual/gl_state.hpp"

#include "visual/gl_error.hpp"
#include "visual/gl_shader_program.hpp"

#include <claw/exception.hpp>

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new setter for a given program.
 * \param program The identifier of the shader program in which the variables
 *        are set.
 */
bear::visual::gl_state::uniform_setter::uniform_setter( GLuint program )
  : m_program( program )
{

} // gl_state::uniform_setter::uniform_setter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the value of an integer uniform.
 * \param name The name of the uniform.
 * \param value The value to assign to the uniform.
 */
void
bear::visual::gl_state::uniform_setter::operator()
( std::string name, int value ) const
{
  glUniform1i( glGetUniformLocation( m_program, name.c_str() ), value);
  VISUAL_GL_ERROR_THROW();
} // gl_state::uniform_setter::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the value of a float uniform.
 * \param name The name of the uniform.
 * \param value The value to assign to the uniform.
 */
void
bear::visual::gl_state::uniform_setter::operator()
( std::string name, double value ) const
{
  glUniform1f( glGetUniformLocation( m_program, name.c_str() ), value);
  VISUAL_GL_ERROR_THROW();
} // gl_state::uniform_setter::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the value of a boolean uniform.
 * \param name The name of the uniform.
 * \param value The value to assign to the uniform.
 */
void
bear::visual::gl_state::uniform_setter::operator()
( std::string name, bool value ) const
{
  glUniform1i( glGetUniformLocation( m_program, name.c_str() ), value);
  VISUAL_GL_ERROR_THROW();
} // gl_state::uniform_setter::operator()()





/*----------------------------------------------------------------------------*/
/**
 * \brief Tests if the visited variables exists with the same value in a given
 *        var map.
 * \param result The result of the visit.
 * \param vars The variable_map to compare to.
 */
bear::visual::gl_state::variables_are_included::variables_are_included
( bool& result, const shader_program::input_variable_map& vars )
  : m_result( result ), m_variables( vars )
{

} // gl_state::variables_are_included::variables_are_included()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the value of an integer uniform.
 * \param name The name of the uniform.
 * \param value The value to assign to the uniform.
 */
template<typename T>
void bear::visual::gl_state::variables_are_included::operator()
( std::string name, T value ) const
{
  if ( !m_result )
    return;

  if ( !m_variables.exists<T>( name ) )
    m_result = false;
  else
    m_result = ( value == m_variables.get<T>( name ) );
} // gl_state::variables_are_included::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the range of elements for a given texture.
 * \param t The texture to use when rendering the vertices.
 * \param i The index of the first vertex to render.
 * \param c The count of vertices to render.
 */
bear::visual::gl_state::element_range::element_range
( GLuint t, std::size_t i, std::size_t c )
  : texture_id(t), vertex_index(i), count(c)
{

} // gl_state::element_range::element_range()




/*----------------------------------------------------------------------------*/
const std::size_t bear::visual::gl_state::gl_state::s_vertex_size(2);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a state to render a filled polygon.
 * \param shader The shader to use when drawing.
 * \param vertices The vertices to draw.
 * \param c The color of the vertices.
 */
bear::visual::gl_state::gl_state
( const shader_program& shader, const position_vector& vertices,
  const color_type& c )
  : m_mode( render_triangles ), m_shader( shader ),
    m_line_width( 0 )
{
  const position_vector v( polygon_to_triangles( vertices ) );

  push_vertices( v );
  push_colors( c, v.size() );
} // gl_state::gl_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a state to render a line.
 * \param shader The shader to use when drawing.
 * \param vertices The vertices to draw.
 * \param c The color of the vertices.
 * \param line_width The width of the line.
 */
bear::visual::gl_state::gl_state
( const shader_program& shader, const position_vector& vertices,
  const color_type& c, double line_width )
  : m_mode( render_lines ), m_shader( shader ),
    m_line_width( line_width )
{
  push_vertices( vertices );
  push_colors( c, vertices.size() );
} // gl_state::gl_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a state to render a given texture in a given mode.
 * \param texture_id The identifier of the texture to use when drawing.
 * \param shader The shader to use when drawing.
 * \param vertices The vertices to draw.
 * \param texture_coordinates The coordinates of the vertices in the texture.
 * \param c The color of the vertices.
 */
bear::visual::gl_state::gl_state
( GLuint texture_id, const shader_program& shader,
  const position_vector& texture_coordinates, const position_vector& vertices,
  const color_type& c )
  : m_mode( render_triangles ),
    m_shader( shader ), m_line_width( 0 )
{
  const position_vector v( polygon_to_triangles( vertices ) );

  push_vertices( v );
  push_texture_coordinates( polygon_to_triangles(texture_coordinates) );
  push_colors( c, v.size() );

  m_elements.push_back( element_range( texture_id, 0, get_vertex_count() ) );
} // gl_state::gl_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this state is compatible with another state.
 * \param state The other state.
 */
bool bear::visual::gl_state::is_compatible_with( const gl_state& state ) const
{
  if ( (m_mode == render_triangles)
       && ( m_mode == state.m_mode )
       && ( m_elements.empty() == state.m_elements.empty() ) )
    {
      if ( m_shader.is_valid() != state.m_shader.is_valid() )
        return false;

      if ( !m_shader.is_valid() && !state.m_shader.is_valid() )
        return true;

      const gl_shader_program* const this_shader
        ( static_cast<const gl_shader_program*>( m_shader.get_impl() ) );
      const gl_shader_program* const that_shader
        ( static_cast<const gl_shader_program*>( state.m_shader.get_impl() ) );
      
      if ( this_shader->program_id() != that_shader->program_id() )
        return false;

      shader_program::variable_visitor_type visitor;
      shader_program::input_variable_map this_shader_vars
        ( m_shader.get_variables() );
      shader_program::input_variable_map that_shader_vars
        ( state.m_shader.get_variables() );
      bool equal_variables( true );

      visitor.run
        ( this_shader_vars,
          variables_are_included( equal_variables, that_shader_vars ) );
      visitor.run
        ( that_shader_vars,
          variables_are_included( equal_variables, this_shader_vars ) );

      return equal_variables;
    }

  return false;
} // gl_state::is_compatible_with()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inserts in this state the content of another compatible state.
 * \param state The other state.
 */
void bear::visual::gl_state::merge( const gl_state& state )
{
  CLAW_PRECOND( is_compatible_with( state ) );

  const std::size_t index( get_vertex_count() );

  for ( element_range_list::const_iterator it( state.m_elements.begin() );
        it != state.m_elements.end(); ++it )
    {
      element_range& back( m_elements.back() );

      if ( it->texture_id == back.texture_id )
        back.count += it->count;
      else
        m_elements.push_back
          ( element_range
            ( it->texture_id, it->vertex_index + index, it->count ) );
    }

  m_vertices.insert
    ( m_vertices.end(), state.m_vertices.begin(), state.m_vertices.end() );
  m_colors.insert
    ( m_colors.end(), state.m_colors.begin(), state.m_colors.end() );
  m_texture_coordinates.insert
    ( m_texture_coordinates.end(),
      state.m_texture_coordinates.begin(), state.m_texture_coordinates.end() );
} // gl_state::merge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Asks OpenGL to draw the vertices of the state.
 */
void bear::visual::gl_state::draw() const
{
  if ( m_elements.empty() )
    draw_shape();
  else
    draw_textured();
} // gl_state::draw()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draws the vertices in the case where there is no texture.
 */
void bear::visual::gl_state::draw_shape() const
{
  if ( m_vertices.empty() )
    return;

  enable_shader();

  if ( m_line_width > 0 )
    {
      glLineWidth( m_line_width );
      VISUAL_GL_ERROR_THROW();
    }

  set_colors();
  set_vertices();

  glBindTexture( GL_TEXTURE_2D, 0 );
  VISUAL_GL_ERROR_THROW();

  glDrawArrays( get_gl_render_mode(), 0, get_vertex_count() );

  disable_states();
} // gl_state::draw_shape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draws the vertices in the case where there are textures.
 */
void bear::visual::gl_state::draw_textured() const
{
  if ( m_vertices.empty() )
    return;

  enable_shader();

  set_colors();
  set_vertices();
  set_texture_coordinates();

  for ( element_range_list::const_iterator it(m_elements.begin());
        it!=m_elements.end(); ++it )
    {
      glBindTexture( GL_TEXTURE_2D, it->texture_id );
      VISUAL_GL_ERROR_THROW();

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      VISUAL_GL_ERROR_THROW();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      VISUAL_GL_ERROR_THROW();

      glDrawArrays( get_gl_render_mode(), it->vertex_index, it->count );
      VISUAL_GL_ERROR_THROW();
    }

  disable_states();
} // gl_state::draw_textured()

/*----------------------------------------------------------------------------*/
/**
 * \brief Passes the color pointer to OpenGL.
 */
void bear::visual::gl_state::set_colors() const
{
  glEnableClientState( GL_COLOR_ARRAY );
  VISUAL_GL_ERROR_THROW();
  glColorPointer( 4, GL_FLOAT, 0, m_colors.data() );
  VISUAL_GL_ERROR_THROW();
} // gl_state::set_colors()

/*----------------------------------------------------------------------------*/
/**
 * \brief Passes the vertex pointer to OpenGL.
 */
void bear::visual::gl_state::set_vertices() const
{
  glEnableClientState( GL_VERTEX_ARRAY );
  VISUAL_GL_ERROR_THROW();
  glVertexPointer( 2, GL_FLOAT, 0, m_vertices.data() );
  VISUAL_GL_ERROR_THROW();
} // gl_state::set_vertices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Passes the pointer on the texture coordinates to OpenGL.
 */
void bear::visual::gl_state::set_texture_coordinates() const
{
  glEnableClientState( GL_TEXTURE_COORD_ARRAY );
  VISUAL_GL_ERROR_THROW();
  glTexCoordPointer( 2, GL_FLOAT, 0, m_texture_coordinates.data() );
  VISUAL_GL_ERROR_THROW();
} // gl_state::set_texture_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells OpenGL to disable the texture coordinates, the vertices and the
 *        colors.
 */
void bear::visual::gl_state::disable_states() const
{
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  VISUAL_GL_ERROR_THROW();

  glDisableClientState( GL_VERTEX_ARRAY );
  VISUAL_GL_ERROR_THROW();

  glDisableClientState( GL_COLOR_ARRAY );
  VISUAL_GL_ERROR_THROW();
} // gl_state::draw_textured()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the number of vertices in the drawing.
 */
std::size_t bear::visual::gl_state::get_vertex_count() const
{
  return m_vertices.size() / s_vertex_size;
} // gl_state::get_vertex_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the OpenGL equivalent to the render mode.
 */
GLenum bear::visual::gl_state::get_gl_render_mode() const
{
  switch( m_mode )
    {
    case render_lines: return GL_LINE_STRIP;
    case render_triangles: return GL_TRIANGLES;
    }

  throw new claw::exception( "Unknown render mode." );
} // gl_state::get_gl_render_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds some vertices at the end of m_vertices.
 * \param v The coordinates of the vertices to add.
 */
void bear::visual::gl_state::push_vertices( const position_vector& v )
{
  for ( std::size_t i(0); i!=v.size(); ++i )
    {
      m_vertices.push_back( v[i].x );
      m_vertices.push_back( v[i].y );
    }
} // gl_state::push_vertices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds some texture coordinates at the end of m_texture_coordinates.
 * \param v The coordinates to add.
 */
void bear::visual::gl_state::push_texture_coordinates
( const position_vector& v )
{
  for ( std::size_t i(0); i!=v.size(); ++i )
    {
      m_texture_coordinates.push_back( v[i].x );
      m_texture_coordinates.push_back( v[i].y );
    }
} // gl_state::push_texture_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds some vertex colors at the end of m_colors.
 * \param c The color to add.
 * \param count The number of times the color must be added.
 */
void bear::visual::gl_state::push_colors
( const color_type& c, std::size_t count )
{
  const GLfloat max( std::numeric_limits<color_type::component_type>::max() );
  const GLfloat r( (GLfloat)c.components.red / max );
  const GLfloat g( (GLfloat)c.components.green / max );
  const GLfloat b( (GLfloat)c.components.blue / max );
  const GLfloat a( (GLfloat)c.components.alpha / max );

  for ( std::size_t i(0); i != count; ++i )
    {
      m_colors.push_back(r);
      m_colors.push_back(g);
      m_colors.push_back(b);
      m_colors.push_back(a);
    }
} // gl_state::push_colors()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds a collection of vertices representing a given polygon as a
 *        collection of triangles.
 * \param v The vertices of the polygon to convert.
 */
bear::visual::gl_state::position_vector
bear::visual::gl_state::polygon_to_triangles( const position_vector& v ) const
{
  CLAW_PRECOND( v.size() >= 3 );

  position_vector result;
  result.reserve( 3 * (v.size() - 2) );

  const position_vector::const_iterator root_vertex( v.begin() );
  position_vector::const_iterator first_vertex( root_vertex + 1 );
  position_vector::const_iterator second_vertex( root_vertex + 2 );

  do
    {
      result.push_back( *root_vertex );
      result.push_back( *first_vertex );
      result.push_back( *second_vertex );

      ++first_vertex;
      ++second_vertex;
    }
  while ( second_vertex != v.end() );

  return result;
} // gl_state::polygon_to_triangles()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enables the shader of the state.
 */
void bear::visual::gl_state::enable_shader() const
{
  if ( !m_shader.is_valid() )
    {
      glUseProgram( 0 );
      VISUAL_GL_ERROR_THROW();
      return;
    }

  const gl_shader_program* const s
    ( static_cast<const gl_shader_program*>( m_shader.get_impl() ) );

  glUseProgram( s->program_id() );
  VISUAL_GL_ERROR_THROW();

  shader_program::variable_visitor_type visitor;
  shader_program::input_variable_map vars( m_shader.get_variables() );
  visitor.run( vars, uniform_setter( s->program_id() ) );
} // gl_state::enable_shader()
