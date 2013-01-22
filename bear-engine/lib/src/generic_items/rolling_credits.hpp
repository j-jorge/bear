/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This items displays rolling credits from a custom file.
 * \author Julien Jorge
 */
#ifndef __BEAR_ROLLING_CREDITS_HPP__
#define __BEAR_ROLLING_CREDITS_HPP__

#include "engine/base_item.hpp"

#include "visual/font/font.hpp"
#include "visual/scene_element.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief This items displays rolling credits, ie. a list of members of the
   *        software team. This list is taken from a custom file.
   *
   * The fields of this class are:
   * - \a credits (path): [required] the credits to display,
   * - \a movement_duration (real): the duration in seconds of the movement of
   *   the lines (default = 1),
   * - \a fading_duration (real): the duration in seconds of the fading effect
   *   when a line appears or disappears (default = movement_duration / 4).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT rolling_credits:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(rolling_credits);

  public:
    /** \brief The type of the parent class. */
    typedef base_item super;

  private:
    class credit_line
    {
    public:
      credit_line
      ( const std::string& text, const visual::font& font, double r, double g,
        double b, double o, const universe::position_type& pos );

      const visual::scene_element& create_scene_element() const;

      const universe::position_type& get_bottom_left() const;
      void set_bottom_left( const universe::position_type& p );

      universe::size_box_type get_size() const;

      bool is_on() const;
      void turn_on();

    private:
      /** \brief The visual displayed on the screen. */
      visual::scene_element m_visual;
      /** \brief Tell if the line has to be displayed. */
      bool m_is_on;

    }; // class credit_line

  public:
    rolling_credits();

    void build();
    void progress( universe::time_type elapsed_time );
    void get_visual( std::list<engine::scene_visual>& visuals ) const;

    bool set_string_field
      ( const std::string& name, const std::string& value );
    bool set_real_field( const std::string& name, double value );

    bool is_valid();

  private:
    /** \brief The path to the credit file. */
    std::string m_file;

    /** \brief The lines displayed on the screen. */
    std::list<credit_line> m_lines;

    /** \brief The total duration of the movement of the lines. */
    universe::time_type m_movement_duration;

    /** \brief The fraction of the movement duration used for the fading effect
        of the lines. */
    universe::time_type m_fading_frac;

    /** \brief Elapsed time since the beginning of the movement. */
    universe::time_type m_elapsed_time;

  }; // class rolling_credits
} // namespace bear

#endif // __BEAR_ROLLING_CREDITS_HPP__
