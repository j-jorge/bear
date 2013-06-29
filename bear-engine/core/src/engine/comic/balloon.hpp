/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A balloon in which we display what a speaker_item says.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_BALLOON_HPP__
#define __BEAR_BALLOON_HPP__

#include "gui/static_text.hpp"
#include "universe/types.hpp"

#include "engine/class_export.hpp"

#include <string>
#include <list>
#include <vector>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A balloon in which we display what a speaker_item says.
     * \author Sebastien Angibaud
     */
    class ENGINE_EXPORT balloon
    {
    public:
      balloon();

      void progress( universe::time_type elapsed_time );
      void render( std::list<visual::scene_element>& e );

      void set_spike_sprite( visual::sprite spr );
      void set_corner_sprite( visual::sprite spr );
      void set_horizontal_border_sprite( visual::sprite spr );
      void set_vertical_border_sprite( visual::sprite spr );
      void set_font( visual::font f );

      bool is_finished() const;
      visual::size_box_type get_size() const;
      visual::size_box_type get_final_size() const;
      void set_position
      ( const visual::position_type& pos, bool on_top, bool on_right );
      visual::position_type get_position() const;

      bool is_on_top() const;
      bool is_on_right() const;

      void set_speeches( const std::list<std::string>& speeches );
      void close();

    private:
      void render_bottom_left_corner
      ( std::list<visual::scene_element>& e,
        visual::sprite s ) const;
      void render_bottom_right_corner
      ( std::list<visual::scene_element>& e,
        visual::sprite s ) const;
      void render_top_left_corner
      ( std::list<visual::scene_element>& e,
        visual::sprite s ) const;
      void render_top_right_corner
      ( std::list<visual::scene_element>& e,
        visual::sprite s ) const;

      void increase( universe::time_type elapsed_time );
      void decrease( universe::time_type elapsed_time );
      void set_content_size( const visual::size_box_type& s );

      void write_text();

    private:
      /** \brief The speech. */
      std::list<std::string> m_speeches;

      /** \brief The spike, going out of the mouth of the speaker. */
      visual::sprite m_spike;

      /** \brief The corner of the balloon. */
      visual::sprite m_corner;

      /** \brief The horizontal border of the balloon. */
      visual::sprite m_horizontal_border;

      /** \brief The vertical border of the balloon. */
      visual::sprite m_vertical_border;

      /** \brief The size of the frame. */
      gui::size_box_type m_size_frame;

      /** \brief Indicates if the balloon is at its maximum size. */
      bool m_has_started;

      /** \brief The component in which we show the text. */
      gui::static_text m_text;

      /** \brief Elapsed time since the creation. */
      universe::time_type m_time;

      /** \brief Indicates how long the text is visible. */
      universe::time_type m_play_time;

      /** \brief Indicates if the balloon is on the top of the item. */
      bool m_on_top;

      /** \brief Indicates if the balloon is on the right of the item. */
      bool m_on_right;

      /** \brief Indicates if the balloon is currently showing something, or
          about to show something. */
      bool m_active;

      /** \brief Indicates increasing duration. */
      universe::time_type m_increasing_duration;

    }; // class balloon

  } // namespace engine
} // namespace bear

#endif // __BEAR_BALLOON_HPP__
