/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The value of a field of type "animation".
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION__
#define __BF_ANIMATION__

#include <list>

#include "bf/animation_frame.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief Animation type.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation:
    public bitmap_rendering_attributes,
    public std::list<animation_frame>
  {
  public:
    typedef std::list<animation_frame> frame_list;

  public:
    animation();

    bool operator==(const animation& that) const;
    bool operator!=(const animation& that) const;
    bool operator<(const animation& that) const;

    void set_frames( const frame_list& frames );
    void set_loops( unsigned int loops );
    void set_loop_back( bool b );
    void set_first_index( unsigned int index );
    void set_last_index( unsigned int index );

    unsigned int get_loops() const;
    bool get_loop_back() const;
    unsigned int get_first_index() const;
    unsigned int get_last_index() const;

    animation_frame& add_frame();
    animation_frame& get_frame(unsigned int index);
    const animation_frame& get_frame(unsigned int index) const;
    void delete_frame(unsigned int index);
    void reverse_frames(const std::list<unsigned int>& indexes);
    void move_backward(unsigned int index);
    void move_forward(unsigned int index);

    sprite get_sprite(unsigned int index) const;

    claw::math::coordinate_2d<unsigned int> get_max_size() const;

    std::size_t frames_count() const;

    double get_duration() const;

    void compile( compiled_file& f, compilation_context& c ) const;

  private:
    /** \brief Number of loops. */
    unsigned int m_loops;

    /** \brief Play the animation backward. */
    bool m_loop_back;

    /** \brief The index of the first frame of the loops. */
    unsigned int m_first_index;

    /** \brief The index of the last frame of the loops. */
    unsigned int m_last_index;

  }; // class animation
} // namespace bf

#endif // __BF_ANIMATION__
