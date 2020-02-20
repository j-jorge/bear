/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::screen class.
 * \author Julien Jorge
 */
#include "bear/visual/screen.hpp"

#include "bear/visual/gl_screen.hpp"

#include <claw/exception.hpp>
#include <claw/bitmap.hpp>
#include <claw/logger.hpp>

#include "bear/time/time.hpp"

/*----------------------------------------------------------------------------*/
bear::visual::screen::sub_system
bear::visual::screen::s_sub_system(screen_undef);

/*----------------------------------------------------------------------------*/
/**
 * \brief Global initializations common to all screens. Must be called at the
 *        begining of your program.
 */
void bear::visual::screen::initialize( sub_system sub_sys )
{
  if ( s_sub_system != screen_undef )
    release();

  s_sub_system = sub_sys;

  switch( s_sub_system )
    {
    case screen_gl:
      gl_screen::initialize();
      break;
    case screen_undef:
      {
        // nothing to do
      }
    }
} // screen::initialize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Global uninitializations common to all screens. Must be called at the
 *        end of your program.
 */
void bear::visual::screen::release()
{
  switch( s_sub_system )
    {
    case screen_gl:
      gl_screen::release();
      break;
    case screen_undef:
      {
        // nothing to do
      }
    }

  s_sub_system = screen_undef;
} // screen::release()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the sub system.
 */
bear::visual::screen::sub_system bear::visual::screen::get_sub_system()
{
  return s_sub_system;
} // screen::get_sub_system()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size Size of the screen.
 * \param title The title of the window created.
 * \param full Tell if the window is full screen or not.
 */
bear::visual::screen::screen
( const claw::math::coordinate_2d<unsigned int>& size,
  const std::string& title, bool full )
  : m_mode(SCREEN_IDLE), m_render_opaque_box(false), m_dumb_rendering(false)
{
  switch( s_sub_system )
    {
    case screen_gl:
      m_impl = new gl_screen(size, title, full);
      break;
    case screen_undef:
      claw::exception("screen sub system has not been set.");
    }
} // screen::screen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::visual::screen::~screen()
{
  delete m_impl;
} // screen::~screen()

void bear::visual::screen::pause()
{
  m_impl->pause();
}

void bear::visual::screen::unpause()
{
  m_impl->unpause();
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn fullscreen mode on/off.
 * \param b Tell if we want a fullscreen mode.
 */
void bear::visual::screen::fullscreen( bool b )
{
  m_impl->fullscreen(b);
} // screen::fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the screen.
 */
claw::math::coordinate_2d<unsigned int> bear::visual::screen::get_size() const
{
  return m_impl->get_size();
} // screen::get_size()

claw::math::coordinate_2d<unsigned int>
bear::visual::screen::get_viewport_size() const
{
  return m_impl->get_viewport_size();
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the container of the screen.
 */
claw::math::coordinate_2d<unsigned int>
bear::visual::screen::get_container_size() const
{
  return m_impl->get_container_size();
} // screen::get_container_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on or off the rendering of the opaque boxes
 * \param b The new value of the flag.
 */
void bear::visual::screen::set_opaque_box_visible( bool b )
{
  m_render_opaque_box = b;
} // screen::set_opaque_box_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on or off the simple rendering procedure.
 * \param b The new value of the flag.
 */
void bear::visual::screen::set_dumb_rendering( bool b )
{
  m_dumb_rendering = b;
} // screen::set_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the simple rendering procedure is active.
 */
bool bear::visual::screen::get_dumb_rendering() const
{
  return m_dumb_rendering;
} // screen::get_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the background.
 * \param c The color.
 */
void bear::visual::screen::set_background_color( const color_type& c )
{
  m_impl->set_background_color(c);
} // screen::set_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color of the background.
 */
bear::visual::color_type bear::visual::screen::get_background_color() const
{
  return m_impl->get_background_color();
} // screen::get_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the rendering process.
 */
void bear::visual::screen::begin_render()
{
  CLAW_PRECOND(m_mode == SCREEN_IDLE);

  m_mode = SCREEN_RENDER;
  m_impl->begin_render();
} // screen::begin_render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw something on the screen.
 * \param e Something.
 */
void bear::visual::screen::render( const scene_element& e )
{
  CLAW_PRECOND(m_mode == SCREEN_RENDER);

  if ( !e.always_displayed() && e.get_bounding_box().empty() )
    return;

  if ( e.has_shadow() )
    {
      scene_element shadow( e );
      shadow.set_shadow( 0, 0 );
      shadow.set_shadow_opacity( 0 );

      shadow.get_rendering_attributes().set_intensity(0, 0, 0);
      shadow.get_rendering_attributes().set_opacity
        ( e.get_rendering_attributes().get_opacity() * e.get_shadow_opacity() );

      shadow.set_position( e.get_position() + e.get_shadow() );

      m_scene_element.push_back( shadow );
    }

  m_scene_element.push_back(e);
} // screen::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the rendering process.
 * \return false if this screen has been closed by the user.
 */
void bear::visual::screen::end_render()
{
  CLAW_PRECOND(m_mode == SCREEN_RENDER);

  render_elements();

  m_impl->end_render();

  m_mode = SCREEN_IDLE;
} // screen::end_render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do a screen shot.
 * \param bitmap_name The name of the bitmap file where we'll dump image.
 */
void bear::visual::screen::shot( const std::string& bitmap_name ) const
{
  std::ofstream f(bitmap_name.c_str());

  if (!f)
    claw::logger << claw::log_warning
                 << "bear::visual::screen::shot: Can't open file '"
                 << bitmap_name << "'" << std::endl;
  else
    {
      claw::graphic::bitmap bmp( get_size().x, get_size().y );
      shot(bmp);
      bmp.save(f);
      f.close();
    }
} // screen::shot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do a screen shot.
 * \param img The image in which we save the content of the screen.
 */
void bear::visual::screen::shot( claw::graphic::image& img ) const
{
  m_impl->shot(img);
} // screen::shot()

bear::visual::capture bear::visual::screen::capture_scene() const
{
  return m_impl->capture_scene();
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the opaque box of an element.
 * \param e The element whose opaque box is rendered.
 */
void bear::visual::screen::render_opaque_box( const scene_element& e ) const
{
  const rectangle_type box = e.get_opaque_box();

  std::vector<position_type> border(4);
  border[0] = box.top_left();
  border[1] = box.top_right();
  border[2] = box.bottom_right();
  border[3] = box.bottom_left();

  m_impl->draw_polygon( color("#80C0C0C0"), border );
  m_impl->draw_line( color("#F0F0F0"), border, 2, true );
} // screen::render_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an element.
 * \param e The element to render
 */
void bear::visual::screen::render_element( const scene_element& e ) const
{
  if ( !e.always_displayed() && e.get_bounding_box().empty() )
    return;

  e.render(*m_impl);

  if ( m_render_opaque_box )
    render_opaque_box( e );
} // screen::render_element()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the elements of the scene.
 */
void bear::visual::screen::render_elements()
{
  if ( m_dumb_rendering )
    {
      for ( scene_element_list::const_iterator it( m_scene_element.begin() );
            it != m_scene_element.end(); ++it )
        render_element( *it );
      
      m_scene_element.clear();
    }
  else
    {
      scene_element_list final_elements; // Elements to render, finally.
      rectangle_list boxes;              // Empty parts of the screen.

      boxes.push_front( rectangle_type(0, 0, get_size().x, get_size().y) );

      // Elements are ordered from the background to the foreground. We split
      // the screen in reverse order so we won't display hidden elements.
      for ( ; !m_scene_element.empty(); m_scene_element.pop_back() )
        {
          const scene_element& e( m_scene_element.back() );

          if ( e.always_displayed()
               || intersects_any( e.get_bounding_box(), boxes ) )
            split( e, final_elements, boxes );
        }

      // split() push the elements at the end of the list, so they are now
      // ordered from the foreground to the background
      for ( ; !final_elements.empty(); final_elements.pop_back() )
        render_element( final_elements.back() );
    }
} // screen::render_elements()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a rectangle intersects a rectangle from a list.
 * \param r The rectangle to check.
 * \param boxes The boxes to compare to.
 */
bool bear::visual::screen::intersects_any
( const rectangle_type& r, const rectangle_list& boxes ) const
{
  bool result=false;
  rectangle_list::const_iterator it;

  for (it=boxes.begin(); !result && (it!=boxes.end()); ++it)
    if ( r.intersects(*it) )
      {
        const rectangle_type inter = r.intersection(*it);
        result = (inter.width() > 0) && (inter.height() > 0);
      }

  return result;
} // screen::intersects_any()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split a scene element to only keep its visible parts and update the
 *        screen cover.
 * \param e The element that will be rendered.
 * \param output The parts of \a e to render.
 * \param boxes The cover of the screen.
 */
void bear::visual::screen::split
( const scene_element& e, scene_element_list& output,
  rectangle_list& boxes ) const
{
  e.burst(boxes, output);

  const rectangle_type r( e.get_opaque_box() );
  const double opaque_box_min_size(0);

  if ( (r.width() > opaque_box_min_size) && (r.height() > opaque_box_min_size) )
    {
      rectangle_list input_boxes;
      rectangle_list::const_iterator it;

      std::swap( input_boxes, boxes );

      for ( it=input_boxes.begin(); it!=input_boxes.end(); ++it )
        subtract( *it, r, boxes );
    }
} // screen::split()

/*----------------------------------------------------------------------------*/
/**
 * \brief Subtract a rectangle \a b from a rectangle \a a.
 * \param a The target.
 * \param b The rectangle to remove in \a a.
 * \param boxes The subparts of \a a after the subtraction.
 */
void bear::visual::screen::subtract
( const rectangle_type& a, const rectangle_type& b,
  rectangle_list& result ) const
{
  if ( !a.intersects(b) )
    result.push_front(a);
  else
    {
      const rectangle_type inter = a.intersection(b);

      if ( (inter.width() <= 8) || (inter.height() <= 8) )
        result.push_front(a);
      else
        {
          if ( a.left() != inter.left() )
            result.push_front
              ( rectangle_type( a.left(), a.bottom(), inter.left(), a.top() ) );

          if ( a.top() != inter.top() )
            result.push_front
              ( rectangle_type
                ( inter.left(), inter.top(), inter.right(), a.top() ) );

          if ( a.right() != inter.right() )
            result.push_front
              ( rectangle_type
                ( inter.right(), a.bottom(), a.right(), a.top() ) );

          if ( a.bottom() != inter.bottom() )
            result.push_front
              ( rectangle_type
                ( inter.left(), a.bottom(), inter.right(), inter.bottom() ) );
        }
    }
} // screen::subtract()
