/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A sprite and a position.
 * \author Julien Jorge
 */
#ifndef __VISUAL_PLACED_SPRITE_HPP__
#define __VISUAL_PLACED_SPRITE_HPP__

#include "bear/visual/sprite.hpp"
#include "bear/visual/types.hpp"

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A sprite and a position.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT placed_sprite
    {
    public:
      placed_sprite();
      placed_sprite( coordinate_type x, coordinate_type y, const sprite& s );
      placed_sprite( const position_type& p, const sprite& s );

      const sprite& get_sprite() const;
      sprite& get_sprite();
      void set_sprite( const sprite& spr );

      const position_type& get_position() const;
      position_type& get_position();
      void set_position( const position_type& p );

    private:
      /** \brief The sprite. */
      sprite m_sprite;

      /** \brief The position. */
      position_type m_position;

    }; // class placed_sprite
  } // namespace visual
} // namespace bear

#endif // __VISUAL_PLACED_SPRITE_HPP__
