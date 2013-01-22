/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The value of a field of type "sprite".
 * \author Julien Jorge
 */
#ifndef __BF_SPRITE_HPP__
#define __BF_SPRITE_HPP__

#include "bf/bitmap_rendering_attributes.hpp"
#include "bf/libeditor_export.hpp"

#include <claw/rectangle.hpp>
#include <string>

namespace bf
{
  class compilation_context;

  /**
   * \brief Sprite type.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT sprite:
    public bitmap_rendering_attributes
  {
  public:
    sprite();

    void set_image_name( const std::string& name );
    void set_spritepos_entry( const std::string& name );
    void set_top( const unsigned int top );
    void set_left( const unsigned int left );
    void set_clip_width( const unsigned int width );
    void set_clip_height( const unsigned int height );
    void set_clip_rectangle( const claw::math::rectangle<unsigned int>& r );

    unsigned int get_left() const;
    unsigned int get_top() const;
    unsigned int get_clip_width() const;
    unsigned int get_clip_height() const;
    claw::math::rectangle<unsigned int> get_clip_rectangle() const;
    const std::string& get_image_name() const;
    const std::string& get_spritepos_entry() const;

    void compile( compiled_file& f, compilation_context& c ) const;

    bool operator==( const sprite& that ) const;
    bool operator!=( const sprite& that ) const;
    bool operator<( const sprite& that ) const;

  private:
    /** \brief The name of the image resource to use. */
    std::string m_image_name;

    /** \brief The name of the entry in the spritepos file. */
    std::string m_spritepos_entry;

    /** \brief Y-coordinate. */
    unsigned int m_top;

    /** \brief X-coordinate. */
    unsigned int m_left;

    /** \brief Width in the source image. */
    unsigned int m_clip_width;

    /** \brief Height in the source image. */
    unsigned int m_clip_height;

  }; // class sprite
} // namespace bf

#endif // __BF_SPRITE_HPP__
