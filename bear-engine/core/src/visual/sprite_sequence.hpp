/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A sequence sprites.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SPRITE_SEQUENCE_HPP__
#define __VISUAL_SPRITE_SEQUENCE_HPP__

#include <vector>

#include <claw/coordinate_2d.hpp>
#include "visual/sprite.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A sequence of sprites.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT sprite_sequence:
      public bitmap_rendering_attributes
    {
    public:
      sprite_sequence();
      sprite_sequence( const std::vector<sprite>& images );

      void reset();
      void next();
      bool empty() const;
      sprite get_sprite() const;

      size_box_type get_max_size() const;

      unsigned int get_max_index() const;
      void set_current_index( unsigned int index );

      void set_loop_back( bool loop_back );
      void set_loops( unsigned int loops );

      void set_first_index(unsigned int index);
      void set_last_index(unsigned int index);

      bool is_finished() const;
      bool is_finite() const;

      bool is_valid() const;

    protected:
      explicit sprite_sequence( const sprite& s );
      unsigned int get_current_index() const;

    private:
      void next_forward();
      void next_backward();

    private:
      /** \brief Sequence's frames. */
      std::vector<sprite> m_sprites;

      /** \brief Current frame. */
      unsigned int m_index;

      /**
       * \brief How many times the sequence should play.
       * \remark 0 means non stop.
       */
      unsigned int m_loops;

      /**
       * \brief Should we loop back (true) or jump (false) to the first frame
       *        when the sprite_sequence ends ?
       */
      bool m_loop_back;

      /** \brief Are we playing in the normal order ? */
      bool m_forward;

      /** \brief How many full playing have we done ? */
      unsigned int m_play_count;

      /** \brief First index of the loop, included. */
      unsigned int m_first_index;

      /** \brief Last index of the loop, included. */
      unsigned int m_last_index;

    }; // class sprite_sequence

  } // namespace visual
} // namespace bear

#endif // __VISUAL_SPRITE_SEQUENCE_HPP__
