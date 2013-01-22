/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_writing.
 * \author Julien Jorge
 */
#include "visual/scene_writing.hpp"

#include "visual/base_screen.hpp"
#include "visual/scene_element.hpp"
#include "visual/scene_sprite.hpp"
#include "visual/bitmap_writing.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the writing on the screen.
 * \param y Y-position of the writing on the screen.
 * \param s The writing to draw.
 */
bear::visual::scene_writing::scene_writing
( coordinate_type x, coordinate_type y, const writing& s )
  : base_scene_element(x, y), m_writing(s)
{
  get_rendering_attributes().set_size( m_writing.get_size() );
} // scene_writing::scene_writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_writing::clone() const
{
  return new scene_writing(*this);
} // scene_writing::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the writing is completely opaque.
 */
bear::visual::rectangle_type bear::visual::scene_writing::get_opaque_box() const
{
  return rectangle_type(0, 0, 0, 0);
} // scene_writing::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the writing.
 */
bear::visual::rectangle_type
bear::visual::scene_writing::get_bounding_box() const
{
  return rectangle_type
    ( get_position().x, get_position().y,
      get_position().x + m_writing.get_width() * get_scale_factor_x(),
      get_position().y + m_writing.get_height() * get_scale_factor_y() );
} // scene_writing::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the writing in sub writings.
 * \param boxes The boxes describing how to split the writing.
 * \param output The resulting writings (they are inserted to the front).
 */
void bear::visual::scene_writing::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  output.push_back( scene_element(*this) );
} // scene_writing::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the writing on a screen.
 * \param scr The screen on which we render the writing.
 */
void bear::visual::scene_writing::render( base_screen& scr ) const
{
  // double dispach
  m_writing.call_render(*this, scr);
} // scene_writing::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render a bitmap writing on a screen.
 * \param w The writing to render.
 * \param scr The screen on which we render the writing.
 */
void bear::visual::scene_writing::render
( const bitmap_writing& w, base_screen& scr ) const
{
  const double r_x
    ( get_scale_factor_x() * get_rendering_attributes().width() / w.width() );
  const double r_y
    ( get_scale_factor_y() * get_rendering_attributes().height() / w.height() );

  for ( std::size_t i=0; i!=w.get_sprites_count(); ++i )
    {
      placed_sprite s( w.get_sprite(i) );
      position_type p(get_position());

      s.get_sprite().combine( get_rendering_attributes() );

      if ( s.get_sprite().is_mirrored() )
        p.x +=
          ( s.get_sprite().width() - s.get_position().x
            - s.get_sprite().width() ) * r_x;
      else
        p.x += s.get_position().x * r_x;

      if ( s.get_sprite().is_flipped() )
        p.y +=
          ( s.get_sprite().height() - s.get_position().y
            - s.get_sprite().height() ) * r_y;
      else
        p.y += s.get_position().y * r_y;

      s.get_sprite().set_size
        ( s.get_sprite().width() * r_x, s.get_sprite().height() * r_y );

      const double a = get_rendering_attributes().get_angle();
      position_type pos(p);
      p.x += s.get_sprite().width() * r_x / 2.0;
      p.y += s.get_sprite().height() * r_y / 2.0;
      position_type center = 
        get_position() + get_bounding_box().size() / 2;
      pos.rotate(center, a);
      p.x -= s.get_sprite().width() * r_x / 2.0;
      p.y -= s.get_sprite().height() * r_y / 2.0;
      
      scr.render(pos, s.get_sprite());
    }
} // scene_writing::render()
