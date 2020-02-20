/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_sprite.
 * \author Julien Jorge
 */
#include "bear/visual/scene_sprite.hpp"

#include "bear/visual/base_screen.hpp"
#include "bear/visual/scene_element.hpp"

#include <limits>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param x X-position of the sprite on the screen.
 * \param y Y-position of the sprite on the screen.
 * \param s The sprite to draw.
 */
bear::visual::scene_sprite::scene_sprite
( coordinate_type x, coordinate_type y, const sprite& s )
  : base_scene_element(x, y), m_sprite(s)
{

} // scene_sprite::scene_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
bear::visual::base_scene_element* bear::visual::scene_sprite::clone() const
{
  return new scene_sprite(*this);
} // scene_sprite::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle where the sprite is completely opaque.
 */
bear::visual::rectangle_type bear::visual::scene_sprite::get_opaque_box() const
{
  bitmap_rendering_attributes attr(get_rendering_attributes());
  attr.combine(m_sprite);

  if ( ( attr.get_opacity() != 1) || (attr.get_angle() != 0) )
    return rectangle_type(0, 0, 0, 0);
  else
    return scale_rectangle( m_sprite.get_opaque_rectangle() );
} // scene_sprite::get_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a rectangle bounding the sprite.
 */
bear::visual::rectangle_type
bear::visual::scene_sprite::get_bounding_box() const
{
  rectangle_type result;
  sprite s(m_sprite);
  s.combine( get_rendering_attributes() );

  const coordinate_type w( s.width() * get_scale_factor_x() );
  const coordinate_type h( s.height() * get_scale_factor_y() );

  if ( s.get_angle() != 0 )
    {
      position_type left_bottom( std::numeric_limits<coordinate_type>::max(),
                                 std::numeric_limits<coordinate_type>::max() );
      position_type right_top( 0, 0 );

      position_type center
        ( get_position().x + w / 2, get_position().y + h / 2 );

      update_side_box( position_type(get_position().x, get_position().y),
                       center, left_bottom, right_top );

      update_side_box( position_type(get_position().x + w, get_position().y),
                       center, left_bottom, right_top );

      update_side_box( position_type(get_position().x, get_position().y + h),
                       center, left_bottom, right_top );

      update_side_box
        ( position_type(get_position().x + w, get_position().y + h), center,
          left_bottom, right_top );

      result = rectangle_type( left_bottom, right_top );
    }
  else
    result =
      rectangle_type
      ( get_position().x, get_position().y, get_position().x + w,
        get_position().y + h );

  CLAW_POSTCOND( get_opaque_box().empty()
                 || result.includes( get_opaque_box() ) );

  return result;
} // scene_sprite::get_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the sprite in sub sprites.
 * \param boxes The boxes describing how to split the sprite.
 * \param output The resulting sprites (they are inserted to the back).
 */
void bear::visual::scene_sprite::burst
( const rectangle_list& boxes, scene_element_list& output ) const
{
  bitmap_rendering_attributes attr(get_rendering_attributes());
  attr.combine(m_sprite);

  if ( (attr.get_angle() != 0) || attr.is_mirrored() || attr.is_flipped() )
    output.push_back( scene_element(*this) );
  else
    {
      rectangle_list::const_iterator it;
      const rectangle_type clip_box
        ( scale_rectangle
          ( rectangle_type
            ( 0, 0, m_sprite.clip_rectangle().width,
              m_sprite.clip_rectangle().height ) ) );

      for (it=boxes.begin(); it!=boxes.end(); ++it)
        if ( clip_box.intersects(*it) )
          {
            const rectangle_type clip_inter = clip_box.intersection(*it);

            if( !clip_inter.empty() )
              output.push_back( burst( *it, clip_inter ) );
          }
    }
} // scene_sprite::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the sprite on a screen.
 * \param scr The screen on which we render the sprite.
 */
void bear::visual::scene_sprite::render( base_screen& scr ) const
{
  if ( !m_sprite.is_valid() )
    return;

  sprite s(m_sprite);
  s.combine( get_rendering_attributes() );
  s.set_size
    ( s.width() * get_scale_factor_x(), s.height() * get_scale_factor_y() );

  scr.render(get_position(), s);
} // scene_sprite::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the sprite of the visual.
 * \param spr The new sprite.
 */
void bear::visual::scene_sprite::set_sprite( const sprite& spr )
{
  m_sprite = spr;
} // scene_sprite::set_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update sides of bounding box when we apply a rotation.
 *
 * \param pos The point to rotate.
 * \param center The center of the rotation.
 * \param left_bottom (out) The left_bottom position of the bounding box.
 * \param right_top (out) The right_top position of the bounding box.
 */
void bear::visual::scene_sprite::update_side_box
( const position_type& pos, const position_type& center,
  position_type& left_bottom, position_type& right_top ) const
{
  sprite s( m_sprite );
  s.combine( get_rendering_attributes() );
  const double a = s.get_angle();

  claw::math::coordinate_2d<double> result(pos);
  result.rotate(center, a);

  if ( result.x < left_bottom.x )
    left_bottom.x = result.x;

  if ( result.y < left_bottom.y )
    left_bottom.y = result.y;

  if ( result.x > right_top.x )
    right_top.x = result.x;

  if ( result.y > right_top.y )
    right_top.y = result.y;
} // scene_sprite::update_side_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two values for which the direction of the axis has been changed.
 * \param a The first value.
 * \param b The second value.
 * \param z The new zero.
 */
void bear::visual::scene_sprite::flip_values_on_axis
( coordinate_type& a, coordinate_type& b, coordinate_type z ) const
{
  const coordinate_type tmp(a);
  a = z - b;
  b = z - tmp;
} // scene_sprite::flip_values_on_axis()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a rectangle in the coordinates of the sprite's clip rectangle
 *        into the equivalent rectangle for the scene sprite. The size of the
 *        rectangle is adapted, the flip and mirror flags too.
 * \param r The rectangle to convert.
 */
bear::visual::rectangle_type
bear::visual::scene_sprite::scale_rectangle( const rectangle_type& r ) const
{
  const double fx =
    ( m_sprite.clip_rectangle().width == 0 )
    ? 0
    : ( m_sprite.width() / m_sprite.clip_rectangle().width
        * get_scale_factor_x() );
  const double fy =
    ( m_sprite.clip_rectangle().height == 0 )
    ? 0
    : ( m_sprite.height() / m_sprite.clip_rectangle().height
        * get_scale_factor_y() );

  coordinate_type left = r.left() * fx;
  coordinate_type bottom = r.bottom() * fy;
  coordinate_type right = r.right() * fx;
  coordinate_type top = r.top() * fy;

  bitmap_rendering_attributes attr( get_rendering_attributes() );
  attr.combine(m_sprite);

  if ( attr.is_mirrored() )
    flip_values_on_axis
      ( left, right, m_sprite.width() * get_scale_factor_x() );
    
  if ( attr.is_flipped() )
    flip_values_on_axis
      ( bottom, top, m_sprite.height() * get_scale_factor_y() );

  left += get_position().x;
  right += get_position().x;
  top += get_position().y;
  bottom += get_position().y;

  return rectangle_type( left, bottom, right, top );
} // scene_sprite::scale_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a rectangle in the coordinates of the scene element into the
 *        equivalent rectangle in the sprite's clip rectangle. The size of the
 *        rectangle is adapted, the flip and mirror flags too.
 * \param r The rectangle to convert.
 */
bear::visual::rectangle_type
bear::visual::scene_sprite::unscale_rectangle( const rectangle_type& r ) const
{
  const double fx =
    m_sprite.width() / m_sprite.clip_rectangle().width
    * get_scale_factor_x();
  const double fy =
    m_sprite.height() / m_sprite.clip_rectangle().height
    * get_scale_factor_y();

  coordinate_type left = r.left() - get_position().x;
  coordinate_type bottom = r.bottom() - get_position().y;
  coordinate_type right = r.right() - get_position().x;
  coordinate_type top = r.top() - get_position().y;

  left /= fx;
  right /= fx;
  top /= fy;
  bottom /= fy;

  bitmap_rendering_attributes attr( get_rendering_attributes() );
  attr.combine(m_sprite);

  if ( attr.is_mirrored() )
    flip_values_on_axis
      ( left, right, m_sprite.clip_rectangle().width );
    
  if ( attr.is_flipped() )
    flip_values_on_axis
      ( bottom, top, m_sprite.clip_rectangle().height );

  return rectangle_type( left, bottom, right, top );
} // scene_sprite::unscale_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Split the sprite in a sub sprite.
 * \param box The box describing how to split the sprite.
 * \param clip_inter The intersection of box and clip_box in the global
 *        coordinates.
 */
bear::visual::scene_sprite bear::visual::scene_sprite::burst
( const rectangle_type& box, const rectangle_type& clip_inter ) const
{
  // Get the sub rectangle of the clip rectangle of the resulting sprite,
  // relatively to the clip rectangle of m_sprite.
  const rectangle_type unscaled_clip = unscale_rectangle( clip_inter );

  sprite spr(m_sprite);

  // Find the part of the sprite to render in the result.
  spr.set_clip_rectangle( get_burst_clip( unscaled_clip ) );
  spr.set_opaque_rectangle( get_burst_opaque_box( box, unscaled_clip ) );

  // Adjust the size of the rendered sprite.
  spr.set_size( clip_inter.width(), clip_inter.height() );

  scene_sprite result(*this);
  result.set_sprite( spr );
  result.set_scale_factor( 1, 1 );
  
  coordinate_type x = clip_inter.left();
  coordinate_type y = clip_inter.bottom();

  if ( x - get_position().x < 0.1 )
    x = get_position().x;

  if ( y - get_position().y < 0.1 )
    y = get_position().y;

  result.set_position( x, y );

  return result;
} // scene_sprite::burst()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opaque box of the sprite after being burst.
 * \param box The box describing how to split the sprite.
 * \param unscaled_clip The intersection of box and the clip rectangle of the
 *        sprites in the coordinates of the sprite's source image.
 */
bear::visual::rectangle_type bear::visual::scene_sprite::get_burst_opaque_box
( const rectangle_type& box, const rectangle_type& unscaled_clip ) const
{
  rectangle_type opaque;

  if ( get_opaque_box().intersects( box ) )
    {
      const rectangle_type unscaled_opaque =
        unscale_rectangle( get_opaque_box().intersection( box ) );
      const coordinate_type x = unscaled_opaque.left() - unscaled_clip.left();
      const coordinate_type y =
        unscaled_opaque.bottom() - unscaled_clip.bottom();

      opaque =
        rectangle_type
        ( x, y, x + unscaled_opaque.width(), y + unscaled_opaque.height() );
    }
  else
    opaque = rectangle_type(0, 0, 0, 0);

  return opaque;
} // scene_sprite::get_burst_opaque_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the clip rectangle of the sprite after being burst.
 * \param unscaled_clip The sub area of the clip rectangle of m_sprite,
 *        relatively to this clip rectangle.
 */
claw::math::rectangle<bear::visual::coordinate_type>
bear::visual::scene_sprite::get_burst_clip
( const rectangle_type& unscaled_clip ) const
{
  claw::math::rectangle<coordinate_type> clip
    ( m_sprite.clip_rectangle().position.x + unscaled_clip.left(),
      m_sprite.clip_rectangle().position.y
      + m_sprite.clip_rectangle().height - unscaled_clip.top(),
      unscaled_clip.width() + 1, unscaled_clip.height() + 1 );

  if ( clip.position.x < m_sprite.clip_rectangle().position.x )
    clip.position.x = m_sprite.clip_rectangle().position.x;

  if ( clip.position.y < m_sprite.clip_rectangle().position.y )
    clip.position.y = m_sprite.clip_rectangle().position.y;

  const coordinate_type w =
    m_sprite.clip_rectangle().width
    - (clip.position.x - m_sprite.clip_rectangle().position.x);

  if ( clip.width > w )
    clip.width = w;

  const coordinate_type h =
    m_sprite.clip_rectangle().height
    - (clip.position.y - m_sprite.clip_rectangle().position.y);

  if ( clip.height > h )
    clip.height = h;

  return clip;
} // scene_sprite::get_burst_clip()
