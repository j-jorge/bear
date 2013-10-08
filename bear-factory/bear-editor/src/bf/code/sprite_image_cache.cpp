/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sprite_image_cache class.
 * \author Julien Jorge
 */
#include "bf/sprite_image_cache.hpp"

#include "bf/image_pool.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>
#include <limits>
#include <sstream>

#include <wx/image.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The sprite.
 * \param w The width of the resulting image.
 * \param h The height of the resulting image.
 */
bf::sprite_image_cache::key_type::key_type
( const sprite& s, unsigned int w, unsigned int h )
  : m_sprite(s), m_width(w), m_height(h)
{
  std::ostringstream oss;

  oss << m_sprite.get_image_name() << "|"
      << trinary_logic::to_string(m_sprite.get_mirrored_status()) << "|"
      << trinary_logic::to_string(m_sprite.get_flipped_status()) << "|"
      << m_sprite.get_clip_width() << "|"
      << m_sprite.get_clip_height() << "|"
      << m_sprite.get_left() << "|"
      << m_sprite.get_top() << "|"
      << m_sprite.get_color().get_red_intensity() << "|"
      << m_sprite.get_color().get_green_intensity() << "|"
      << m_sprite.get_color().get_blue_intensity() << "|"
      << m_sprite.get_angle() << "|"
      << m_sprite.get_color().get_opacity() << "|"
      << m_width << "|"
      << m_height;

  m_key_string = oss.str();
} // sprite_image_cache::key_type::key_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two keys.
 * \param that The other key.
 */
bool bf::sprite_image_cache::key_type::operator<( const key_type& that ) const
{
  return m_key_string < that.m_key_string;
} // sprite_image_cache::key_type::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the key's sprite.
 */
const bf::sprite& bf::sprite_image_cache::key_type::get_sprite() const
{
  return m_sprite;
} // sprite_image_cache::key_type::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the key's width.
 */
unsigned int bf::sprite_image_cache::key_type::get_width() const
{
  return m_width;
} // sprite_image_cache::key_type::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the key's height.
 */
unsigned int bf::sprite_image_cache::key_type::get_height() const
{
  return m_height;
} // sprite_image_cache::key_type::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param env The workspace environment to use.
 */
bf::sprite_image_cache::sprite_image_cache(workspace_environment* env)
  : m_workspace(env)
{

} // sprite_image_cache::sprite_image_cache()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an image.
 * \param s The sprite of which we want the image.
 * \return get_image( s, s.get_clip_width(), s.get_clip_height() )
 */
std::pair<wxBitmap, wxPoint>
bf::sprite_image_cache::get_image( const sprite& s )
{
  return get_image( s, s.get_clip_width(), s.get_clip_height() );
} // sprite_image_cache::get_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an image.
 * \param s The sprite.
 * \param w The width of the resulting image.
 * \param h The height of the resulting image.
 * \return A pair containing the resulting image as the first member, and a
 *         delta to apply to the position of the image to keep the new image
 *         centered on the initial one.
 */
std::pair<wxBitmap, wxPoint> bf::sprite_image_cache::get_image
( const sprite& s, unsigned int w, unsigned int h )
{
  std::pair<wxBitmap, wxPoint> result;

  key_type k(s, w, h);
  std::map<key_type, value_type>::iterator it=m_cache.find(k);

  if ( it == m_cache.end() )
    result = add_image(k);
  else
    {
      ++it->second.usage_count;
      result = it->second.resulting_image;
    }

  return result;
} // sprite_image_cache::get_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the images.
 */
void bf::sprite_image_cache::clear()
{
  m_cache.clear();
} // sprite_image_cache::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new image in the cache.
 * \param k The attributes of the image.
 * \return A pair containing the resulting image as the first member, and a
 *         delta to apply to the position of the image to keep the new image
 *         centered on the initial one.
 */
std::pair<wxBitmap, wxPoint>
bf::sprite_image_cache::add_image( const key_type& k )
{
  value_type v;
  wxString name( std_to_wx_string(k.get_sprite().get_image_name()) );

  v.usage_count = 1;
  v.resulting_image.second = wxPoint(0, 0);

  const wxRect sub_bmp
    ( k.get_sprite().get_left(), k.get_sprite().get_top(),
      k.get_sprite().get_clip_width(), k.get_sprite().get_clip_height() );

  v.resulting_image.first =
    m_workspace->get_image_pool().get_image(name).GetSubBitmap(sub_bmp);
  
  if ( trinary_logic::to_bool(k.get_sprite().get_mirrored_status()) ||
       trinary_logic::to_bool(k.get_sprite().get_flipped_status())
       || (k.get_sprite().get_clip_width() != k.get_width())
       || (k.get_sprite().get_clip_height() != k.get_height())
       || (k.get_sprite().get_angle() != 0)
       || (k.get_sprite().get_color().get_red_intensity() != 1)
       || (k.get_sprite().get_color().get_green_intensity() != 1)
       || (k.get_sprite().get_color().get_blue_intensity() != 1)
       || (k.get_sprite().get_color().get_opacity() != 1) )
    v.resulting_image = apply_effects
      ( k, v.resulting_image.first.ConvertToImage() );

  m_cache[k] = v;

  return v.resulting_image;
} // sprite_image_cache::add_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the effects of a sprite to an image.
 * \param k The attributes of the image.
 * \param bmp The initial image.
 * \return A pair containing the resulting image as the first member, and a
 *         delta to apply to the position of the image to keep the new image
 *         centered on the initial one.
 */
std::pair<wxBitmap, wxPoint>
bf::sprite_image_cache::apply_effects( const key_type& k, wxBitmap bmp ) const
{
  wxImage img( bmp.ConvertToImage() );
  wxPoint pos(0, 0);

  if ( trinary_logic::to_bool(k.get_sprite().get_mirrored_status()) )
    img = img.Mirror(true);

  if ( trinary_logic::to_bool(k.get_sprite().get_flipped_status()) )
    img = img.Mirror(false);

  if ( (k.get_sprite().get_clip_width() != k.get_width())
       || (k.get_sprite().get_clip_height() != k.get_height()) )
    img.Rescale( k.get_width(), k.get_height() );

  if ( k.get_sprite().get_angle() != 0 )
    {
      wxPoint center(k.get_width() / 2, k.get_height() / 2);
      img = img.Rotate(k.get_sprite().get_angle(), center);

      pos = wxPoint
        ( center.x - img.GetWidth() / 2, center.y - img.GetHeight() / 2 );
    }

  if ( (k.get_sprite().get_color().get_red_intensity() != 1)
       || (k.get_sprite().get_color().get_green_intensity() != 1)
       || (k.get_sprite().get_color().get_blue_intensity() != 1) )
    {
      unsigned char* data = img.GetData();
      const unsigned char* const end =
        data + 3 * img.GetWidth() * img.GetHeight();

      for( ; data!=end; data+=3 )
        {
          data[0] = 
            ( (double)data[0] * 
              k.get_sprite().get_color().get_red_intensity() );
          data[1] = 
            ( (double)data[1] * 
              k.get_sprite().get_color().get_green_intensity() );
          data[2] = 
            ( (double)data[2] * 
              k.get_sprite().get_color().get_blue_intensity() );
        }
    }

  if ( k.get_sprite().get_color().get_opacity() != 1 )
    {
      if ( !img.HasAlpha() )
        img.SetAlpha();

      unsigned char* data = img.GetAlpha();
      const unsigned char* const end = data + img.GetWidth() * img.GetHeight();

      for( ; data!=end; ++data )
        *data = ( (double)*data * k.get_sprite().get_color().get_opacity() );
    }

  return std::pair<wxBitmap, wxPoint>(wxBitmap(img), pos);
} // sprite_image_cache::apply_effects()

