/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_screen class.
 * \author Julien Jorge
 */
#include "visual/gl_screen.hpp"

#include "visual/gl_image.hpp"
#include "visual/gl_renderer.hpp"
#include "visual/gl_shader_program.hpp"
#include "visual/sdl_error.hpp"
#include "visual/shader_program.hpp"
#include "visual/sprite.hpp"

#include <claw/exception.hpp>
#include <claw/assert.hpp>
#include <claw/logger.hpp>

#include "visual/gl.hpp"

#include <limits>
#include <list>

#include <SDL2/SDL_main.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Global initializations common to all gl_screens. Must be called at the
 *        begining of your program.
 */
void bear::visual::gl_screen::initialize()
{
  if ( SDL_Init(0) != 0 )
    VISUAL_SDL_ERROR_THROW();

  if ( !SDL_WasInit(SDL_INIT_VIDEO) )
    if ( SDL_InitSubSystem(SDL_INIT_VIDEO) != 0 )
      VISUAL_SDL_ERROR_THROW();

  if ( SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ) != 0 )
    {
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      VISUAL_SDL_ERROR_THROW();
    }

  for (unsigned int i=0; i!=SDL_USEREVENT; ++i)
    SDL_EventState( i, SDL_DISABLE );

  SDL_EventState( SDL_QUIT, SDL_ENABLE );
} // gl_screen::initialize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Global uninitializations common to all gl_screens. Must be called at
 *        the end of your program.
 */
void bear::visual::gl_screen::release()
{
  gl_renderer::terminate();
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
} // gl_screen::release()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size Size of the gl_screen.
 * \param title The title of the window created.
 * \param full Tell if the window is full gl_screen or not.
 */
bear::visual::gl_screen::gl_screen
( const claw::math::coordinate_2d<unsigned int>& size,
  const std::string& title, bool full )
{
  gl_renderer::get_instance().set_video_mode( size, full );
  gl_renderer::get_instance().set_title( title );
} // gl_screen::gl_screen() [constructor]

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells not to render anything for a while.
 */
void bear::visual::gl_screen::set_pause()
{
  gl_renderer::get_instance().set_pause();
} // gl_screen::set_pause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turns the rendering process on again.
 */
void bear::visual::gl_screen::unset_pause()
{
  gl_renderer::get_instance().unset_pause();
} // gl_screen::unset_pause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn fullscreen mode on/off.
 * \param b Tell if we want a fullscreen mode.
 */
void bear::visual::gl_screen::fullscreen( bool b )
{
  gl_renderer::get_instance().set_fullscreen(b);
} // gl_screen::fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the gl_screen.
 */
claw::math::coordinate_2d<unsigned int>
bear::visual::gl_screen::get_size() const
{
  return gl_renderer::get_instance().get_size();
} // gl_screen::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the container of the screen.
 */
claw::math::coordinate_2d<unsigned int>
bear::visual::gl_screen::get_container_size() const
{
  return gl_renderer::get_instance().get_container_size();
} // gl_screen::get_container_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the background, used to clear the screen.
 * \param c The color.
 */
void bear::visual::gl_screen::set_background_color( const color_type& c )
{
  gl_renderer::get_instance().set_background_color( c );
} // gl_screen::set_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color of the background, used to clear the screen.
 */
bear::visual::color_type bear::visual::gl_screen::get_background_color() const
{
  return gl_renderer::get_instance().get_background_color();
} // gl_screen::get_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the rendering process.
 */
void bear::visual::gl_screen::begin_render()
{
  while ( !m_shader.empty() )
    pop_shader();
} // gl_screen::begin_render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a sprite on the gl_screen.
 * \param pos On gl_screen position of the sprite.
 * \param s The sprite to draw.
 */
void bear::visual::gl_screen::render
( const position_type& pos, const sprite& s )
{
  render_sprite( pos, s );
} // gl_screen::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the rendering process.
 * \return false if this screen has been closed by the user.
 */
void bear::visual::gl_screen::end_render()
{
  gl_renderer::get_instance().set_gl_states( m_gl_state );
} // gl_screen::end_render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a line.
 * \param color The color of the line.
 * \param p The points of the line.
 * \param w The width of the line.
 * \param close Tell if the line come back to the first coordinate.
 */
void bear::visual::gl_screen::draw_line
( const color_type& color, const std::vector<position_type>& p,
  double w, bool close )
{
  if ( (w <= 0) || p.empty() )
    return;

  std::vector<position_type> vertices( p );

  if ( close )
    vertices.push_back( vertices.front() );

  push_state( gl_state( get_current_shader(), vertices, color, w ) );
} // gl_screen::draw_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a filled polygon.
 * \param color The color of the polygon.
 * \param p The points of the polygon.
 */
void bear::visual::gl_screen::draw_polygon
( const color_type& color, const std::vector<position_type>& p )
{
  push_state
    ( gl_state( get_current_shader(), p, color ) );
} // gl_screen::draw_polygon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the shader program to apply for the next render commands.
 * \param p The program to apply.
 */
void bear::visual::gl_screen::push_shader( const shader_program& p )
{
  m_shader.push_back( p );
} // gl_screen::push_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes the last shader program passed to push_shader.
 */
void bear::visual::gl_screen::pop_shader()
{
  if ( m_shader.empty() )
    {
      claw::logger << claw::log_warning << "There is no shader to pop."
                   << std::endl;
      return;
    }

  m_shader.pop_back();
} // gl_screen::pop_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do a screen shot.
 * \param img The image in which we save the content of the gl_screen.
 */
void bear::visual::gl_screen::shot( claw::graphic::image& img ) const
{
  gl_renderer::get_instance().shot( img );
} // gl_screen::shot()

boost::signals2::connection bear::visual::gl_screen::schedule_shot
( const boost::function< void( const claw::graphic::image& ) >& f )
{
  return gl_renderer::get_instance().shot( f );
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a sprite with transformation (flip or mirror).
 * \param pos On screen position of the sprite.
 * \param s The sprite to draw.
 */
void bear::visual::gl_screen::render_sprite
( const position_type& pos, const sprite& s )
{
  if ( (s.clip_rectangle().width == 0) || (s.clip_rectangle().height == 0 ) )
    return;

  const claw::math::box_2d<GLdouble> clip_vertices = get_texture_clip(s);

  std::vector<position_type> render_coord(4);
  get_render_coord( pos, s, render_coord );

  color_type color;
  color.set
    ( s.get_red_intensity(), s.get_green_intensity(),
      s.get_blue_intensity(), s.get_opacity() );

  const gl_image* impl = static_cast<const gl_image*>(s.get_image().get_impl());
  GLuint texture_id( impl->texture_id() );

  render_image( texture_id, render_coord, clip_vertices, color );
} // gl_screen::render_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinates of the corners of a sprite after transformation
 *        (mirror, flip, rotation, etc.).
 * \param pos On screen position of the sprite.
 * \param s The sprite of which we want the corners.
 * \param result An array of 4 entries that will receive the coordinates of the
 *        corners, clockwise, starting from the top left corner.
 */
void
bear::visual::gl_screen::get_render_coord
( const position_type& pos, const sprite& s,
  std::vector<position_type>& result ) const
{
  typedef position_type coord_double;

  GLdouble bottom( pos.y );
  GLdouble top( bottom + s.height() );
  GLdouble left( pos.x );
  GLdouble right( left + s.width() );

  if ( s.is_mirrored() )
    std::swap(left, right);

  if ( s.is_flipped() )
    std::swap(top, bottom);

  const coord_double center = pos + s.get_size() / 2;
  const double a = s.get_angle();

  result[0] = rotate( coord_double(left, top), a, center );
  result[1] = rotate( coord_double(right, top), a, center );
  result[2] = rotate( coord_double(right, bottom), a, center );
  result[3] = rotate( coord_double(left, bottom), a, center );

  for ( std::size_t i(0); i!=result.size(); ++i ) {
    result[i].x = (int)( result[i].x + 0.5 );
    result[i].y = (int)( result[i].y + 0.5 );
  }
} // gl_screen::get_render_coord()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the clip rectangle of a sprite in the coordinates of the OpenGL
 *        texture.
 * \param s The sprite from which we take the clip rectangle.
 */
claw::math::box_2d<GLdouble>
bear::visual::gl_screen::get_texture_clip( const sprite& s ) const
{
  const claw::math::box_2d<GLdouble> empty_clip( 0, 0, 0, 0 );
  const claw::math::rectangle<GLdouble> clip_rectangle(s.clip_rectangle());

  if ( (clip_rectangle.width == 0) || (clip_rectangle.height == 0) )
    return empty_clip;

  const claw::math::coordinate_2d<GLdouble> tex_size(s.get_image().size());

  claw::math::box_2d<GLdouble> result;

#ifdef BEAR_SPRITE_CLIP_SUBPIXEL

  const GLdouble min_distance_to_pixel_border( 1.0 / 1000 );
  GLdouble horizontal_shift;
  GLdouble vertical_shift;

  if ( clip_rectangle.width == s.width() )
    horizontal_shift = min_distance_to_pixel_border;
  else
    horizontal_shift = 1.0 / tex_size.x / 2;

  if ( clip_rectangle.height == s.height() )
    vertical_shift = min_distance_to_pixel_border;
  else
    vertical_shift = 1.0 / tex_size.y / 2;

  result.first_point.x =
    clip_rectangle.position.x / tex_size.x + horizontal_shift;

  result.second_point.x =
    clip_rectangle.right() / tex_size.x - horizontal_shift;

  result.first_point.y =
    clip_rectangle.position.y / tex_size.y + vertical_shift;

  result.second_point.y =
    clip_rectangle.bottom() / tex_size.y - vertical_shift;

  if ( result.second_point.x < result.first_point.x )
    result.second_point.x = result.first_point.x;

  if ( result.second_point.y < result.first_point.y )
    result.second_point.y = result.first_point.y;

  CLAW_POSTCOND( result.first_point.x > 0 );
  CLAW_POSTCOND( result.first_point.x < 1 );
  CLAW_POSTCOND( result.first_point.y > 0 );
  CLAW_POSTCOND( result.first_point.y < 1 );

  CLAW_POSTCOND( result.second_point.x > 0 );
  CLAW_POSTCOND( result.second_point.x < 1 );
  CLAW_POSTCOND( result.second_point.y > 0 );
  CLAW_POSTCOND( result.second_point.y < 1 );

#else
  
  result.first_point.x = clip_rectangle.position.x / tex_size.x;
  result.first_point.y = clip_rectangle.position.y / tex_size.y;
  result.second_point.x =
    result.first_point.x + clip_rectangle.width / tex_size.x;
  result.second_point.y =
    result.first_point.y + clip_rectangle.height / tex_size.y;

  CLAW_POSTCOND( result.first_point.x >= 0 );
  CLAW_POSTCOND( result.first_point.x <= 1 );
  CLAW_POSTCOND( result.first_point.y >= 0 );
  CLAW_POSTCOND( result.first_point.y <= 1 );

  CLAW_POSTCOND( result.second_point.x >= 0 );
  CLAW_POSTCOND( result.second_point.x <= 1 );
  CLAW_POSTCOND( result.second_point.y >= 0 );
  CLAW_POSTCOND( result.second_point.y <= 1 );

#endif
  
  return result;
} // gl_screen::get_texture_clip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a textured polygon.
 * \param texture_id The identifier of the texture use.
 * \param render_coord The coordinates of the vertices to draw.
 * \param clip Part of the texture to draw.
 * \param color The color of the vertices.
 */
void bear::visual::gl_screen::render_image
( GLuint texture_id, const std::vector<position_type>& render_coord,
  const claw::math::box_2d<GLdouble>& clip,
  const color_type& color )
{
  push_state
    ( gl_state
      ( texture_id, get_current_shader(), get_texture_coordinates(clip),
        render_coord, color ) );
} // gl_screen::render_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the result of the rotation at a given angle of a point around
 *        a given center.
 * \param pos The point to rotate.
 * \param a The angle of the rotation.
 * \param center The center of the rotation.
 */
bear::visual::position_type bear::visual::gl_screen::rotate
( const position_type& pos, GLdouble a, const position_type& center ) const
{
  position_type result(pos);
  result.rotate(center, a);
  return result;
} // gl_screen::rotate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns a vector of the texture coordinates represented
 *        by a given clip rectangle.
 * \param clip The rectangle from which we computes the coordinates.
 */
std::vector<bear::visual::position_type>
bear::visual::gl_screen::get_texture_coordinates
( const claw::math::box_2d<GLdouble>& clip ) const
{
  std::vector<position_type> result(4);

  // Top-left corner
  result[0].x = clip.first_point.x;
  result[0].y = clip.first_point.y;

  // Top-right corner
  result[1].x = clip.second_point.x;
  result[1].y = clip.first_point.y;
  
  // Bottom-right corner
  result[2].x = clip.second_point.x;
  result[2].y = clip.second_point.y;
  
  // Bottom-left corner
  result[3].x = clip.first_point.x;
  result[3].y = clip.second_point.y;

  return result;
} // gl_screen::get_texture_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pushes a new rendering command.
 * \param state The rendering command.
 */
void bear::visual::gl_screen::push_state( const gl_state& state )
{
  if ( m_gl_state.empty() || !m_gl_state.back().is_compatible_with(state) )
    m_gl_state.push_back( state );
  else
    m_gl_state.back().merge( state );
} // gl_screen::push_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the current shader program to apply.
 */
bear::visual::shader_program
bear::visual::gl_screen::get_current_shader() const
{
  typedef std::vector<shader_program>::const_reverse_iterator iterator_type;

  for ( iterator_type it = m_shader.rbegin(); it != m_shader.rend(); ++it )
    if ( it->is_valid() )
      return *it;

  return shader_program();
} // gl_screen::get_current_shader()
