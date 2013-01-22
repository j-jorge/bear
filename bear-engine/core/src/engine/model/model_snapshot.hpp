/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A snapshot in an model_action of a model_actor.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_SNAPSHOT_HPP__
#define __ENGINE_MODEL_SNAPSHOT_HPP__

#include "engine/model/model_mark_placement.hpp"
#include "engine/class_export.hpp"

#include <vector>
#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A snapshot in an model_action of a model_actor.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_snapshot
    {
    public:
      typedef std::vector<model_mark_placement>::const_iterator
      const_mark_placement_iterator;

      /** \brief How to align the action, horizontally, with the previous
          action. */
      struct horizontal_alignment
      {
        enum value
          {
            align_left,
            align_right,
            align_center
          }; // enum value

        static value from_string( const std::string& str );
      }; // horizontal_alignment

      /** \brief How to align the action, vertically, with the previous
          action. */
      struct vertical_alignment
      {
        enum value
          {
            align_top,
            align_bottom,
            align_center
          }; // enum value

        static value from_string( const std::string& str );
      }; // vertical_alignment

    public:
      model_snapshot
      ( universe::time_type d, std::size_t n, std::string func,
        std::vector<std::string> sounds, bool glob );

      universe::time_type get_date() const;

      void set_mark_placement( const model_mark_placement& m );
      const model_mark_placement& get_mark_placement( std::size_t i ) const;
      std::size_t get_mark_placements_count() const;

      std::string get_function() const;
      std::string get_random_sound_name() const;
      bool sound_is_global() const;

      const_mark_placement_iterator mark_placement_begin() const;
      const_mark_placement_iterator mark_placement_end() const;

      void set_size( double width, double height );
      double get_width() const;
      double get_height() const;

      void set_x_alignment(horizontal_alignment::value a);
      horizontal_alignment::value get_x_alignment() const;
      void set_x_alignment_value(universe::coordinate_type value);
      universe::coordinate_type get_x_alignment_value() const;

      void set_y_alignment(const vertical_alignment::value a);
      vertical_alignment::value get_y_alignment() const;
      void set_y_alignment_value(universe::coordinate_type value);
      universe::coordinate_type get_y_alignment_value() const;

    private:
      /** \brief The date of this snapshot in the action. */
      const universe::time_type m_date;

      /** \brief The placement of the marks in this snapshot. */
      std::vector<model_mark_placement> m_placement;

      /** \brief The name of a function to call when passing on this
          snapshot. */
      std::string m_function;

      /**
       * \brief The name of the sound to play when passing on this
       *        snapshot. One of them will be randomly picked.
       */
      std::vector<std::string> m_sound_name;

      /** \brief Tell if the sound is played globally. */
      bool m_sound_is_global;

      /** \brief The width of the snapshot. */
      double m_width;

      /** \brief The height of the snaphot. */
      double m_height;

      /** \brief The alignment on x-coordinate. */
      horizontal_alignment::value m_x_alignment;

      /** \brief The alignment on x-coordinate. */
      universe::coordinate_type m_x_alignment_value;

      /** \brief The alignment on y-coordinate. */
      vertical_alignment::value m_y_alignment;

      /** \brief The alignment on y-coordinate. */
      universe::coordinate_type m_y_alignment_value;

    }; // class model_snapshot
  } // namespace engine
} // namespace bear

#endif // __ENGINE_MODEL_SNAPSHOT_HPP__
