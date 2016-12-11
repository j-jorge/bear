/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
#include "visual/bitmap_writing.hpp"

#include "visual/scene_writing.hpp"
#include "visual/font/font.hpp"
#include "visual/text_layout.hpp"

#include <limits>
#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to display the text.
 * \param str The text to arrange.
 * \param list (out) The sprites of the text.
 *
 * The instance keep the references on each parameter. So they must live longer
 * than \a this.
 */
bear::visual::bitmap_writing::arrange_sprite_list::arrange_sprite_list
( const font& f, const std::string& str, sprite_list& list )
  : m_text(str), m_font(f), m_sprites(list),
    m_bottom( std::numeric_limits< coordinate_type >::max() )
{

} // bitmap_writing::arrange_sprite_list::arrange_sprite_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw a word on the screen.
 * \param p The position where the word starts.
 * \param first The first character of the word to display.
 * \param last The character just past the last character to display.
 */
void bear::visual::bitmap_writing::arrange_sprite_list::operator()
( position_type p, std::size_t first, std::size_t last )
{
  position_type baseline_position( p );

  for (; first!=last; ++first)
    {
      const glyph_metrics m( m_font.get_metrics( m_text[first] ) );
      const sprite s( m_font.get_sprite( m_text[first] ) );
      const position_type sprite_position
        ( baseline_position + m.get_bearing() );

      m_sprites.push_back( placed_sprite( sprite_position, s) );

      baseline_position.x += m.get_advance().x;
      m_bottom = std::min( m_bottom, sprite_position.y );
    }
} // bitmap_writing::arrange_sprite_list::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the y-position of the bottom line of the text.
 */
bear::visual::coordinate_type
bear::visual::bitmap_writing::arrange_sprite_list::get_bottom() const
{
  return m_bottom;
} // bitmap_writing::arrange_sprite_list::get_bottom()




/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of sprites in the writing.
 */
std::size_t bear::visual::bitmap_writing::get_sprites_count() const
{
  return m_sprites.size();
} // bitmap_writing::get_sprites_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite from the writing.
 * \param i The index of the sprite to get.
 */
bear::visual::placed_sprite
bear::visual::bitmap_writing::get_sprite( std::size_t i ) const
{
  placed_sprite result;

  result = m_sprites[i];
  result.get_sprite().combine(*this);

  result.get_sprite().colorize( m_effect.get_color(i) );
  result.set_position( result.get_position() + m_effect.get_delta(i) );

  return result;
} // bitmap_writing::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the effect to apply to the sprites of the text.
 * \param e The effect.
 */
void bear::visual::bitmap_writing::set_effect( sequence_effect e )
{
  m_effect = e;
} // bitmap_writing::set_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the rendering of the writing and its effects.
 * \param t The elapsed type since the last call.
 */
void bear::visual::bitmap_writing::update( double t )
{
  m_effect.update(t);
} // bitmap_writing::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the rendering of the writing.
 * \param f The font used to render the text.
 * \param str The text to render.
 * \param s The maximum size of the bitmap_writing.
 * \param h The horizontal alignment of the lines of text.
 * \param v The vertical alignment of the lines of text.
 */
void bear::visual::bitmap_writing::create
( const font& f, const std::string& str, const size_box_type& s,
  text_align::horizontal_align h, text_align::vertical_align v )
{
  set_size(s);
  m_sprites.clear();
  m_sprites.reserve( str.length() );

  arrange_sprite_list func( f, str, m_sprites );
  text_layout layout(f, str, s, h );

  layout.arrange_text<arrange_sprite_list&>( func );

  if ( v == text_align::align_bottom )
    shift_vertically( - func.get_bottom() );
  else if ( v == text_align::align_middle )
    shift_vertically( - func.get_bottom() / 2 );
} // bitmap_writing::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the render method of a scene_writing (double dispach) .
 * \param s The scene_writing on which we call the method.
 * \param scr The screen to pass to \a s.
 */
void bear::visual::bitmap_writing::call_render
( const scene_writing& s, base_screen& scr ) const
{
  s.render(*this, scr);
} // bitmap_writing::call_render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Applies an offset on the y-position of all the sprites in m_sprites.
 * \param offset The offset to apply.
 */
void bear::visual::bitmap_writing::shift_vertically( coordinate_type offset )
{
  for ( sprite_list::iterator it=m_sprites.begin(); it!=m_sprites.end(); ++it )
    it->set_position
      ( position_type(it->get_position().x, it->get_position().y + offset) );
} // bitmap_writing::shift_vertically()
