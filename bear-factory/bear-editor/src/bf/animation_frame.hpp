/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A frame in an animation.
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_FRAME_HPP__
#define __BF_ANIMATION_FRAME_HPP__

#include "bf/sprite.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief A frame is a element of an animation (a sprite and a time).
   */
  class BEAR_EDITOR_EXPORT animation_frame
  {
  public:
    animation_frame();

    void set_sprite(const sprite& spr);
    void set_duration(double time);

    const bf::sprite& get_sprite() const;
    double get_duration() const;

    bool operator==( const animation_frame& that ) const;
    bool operator<( const animation_frame& that ) const;

  private:
    /** \brief The sprite. */
    sprite m_sprite;

    /** \brief The time during which the sprite is display. */
    double m_duration;

  }; // class animation_frame
} // namespace bf

#endif // __BF_ANIMATION_FRAME_HPP__
