/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A cache of resized or rotated sprites.
 * \author Julien Jorge
 */
#ifndef __BF_SPRITE_IMAGE_CACHE_HPP__
#define __BF_SPRITE_IMAGE_CACHE_HPP__

#include "bf/sprite.hpp"
#include "bf/libeditor_export.hpp"

#include <map>
#include <wx/bitmap.h>

namespace bf
{
  /**
   * \brief A cache of resized or rotated sprites.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT sprite_image_cache
  {
  private:
    /**
     * \brief The key of the sprite map.
     */
    class key_type
    {
    public:
      key_type( const sprite& s, unsigned int w, unsigned int h );

      bool operator<( const key_type& k ) const;

      const sprite& get_sprite() const;
      unsigned int get_width() const;
      unsigned int get_height() const;

    private:
      /** \brief The sprite. */
      sprite m_sprite;

      /** \brief The width of the resulting image. */
      unsigned int m_width;

      /** \brief The height of the resulting image. */
      unsigned int m_height;

      /** \brief String representation of the string. */
      std::string m_key_string;

    }; // class key_type

    /**
     * \brief The values stored in the cache.
     */
    struct value_type
    {
      /** \brief The resulting image and the distance between the top left
          corner of the original image and the one of the rotated image. */
      std::pair<wxBitmap, wxPoint> resulting_image;

      /** \brief Evaluation of the usefulness of this cache entry. */
      int usage_count;

    }; // struct value_type

  public:
    std::pair<wxBitmap, wxPoint> get_image( const sprite& s );
    std::pair<wxBitmap, wxPoint> get_image
    ( const sprite& s, unsigned int w, unsigned int h );

    void clear();

  private:
    std::pair<wxBitmap, wxPoint> add_image( const key_type& k );
    std::pair<wxBitmap, wxPoint>
    apply_effects( const key_type& k, wxBitmap bmp ) const;

  private:
    /** \brief The cache of images. */
    std::map<key_type, value_type> m_cache;

  }; // class sprite_image_cache
} // namespace bf

#endif // __BF_SPRITE_IMAGE_CACHE_HPP__
