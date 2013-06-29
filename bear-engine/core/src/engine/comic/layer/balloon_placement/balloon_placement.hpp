/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that tries to place the balloons such that they don't overlap.
 * \author Julien Jorge
 */
#ifndef __BEAR_BALLOON_PLACEMENT_HPP__
#define __BEAR_BALLOON_PLACEMENT_HPP__

#include "universe/types.hpp"

#include "engine/class_export.hpp"

#include <list>

namespace bear
{
  namespace engine
  {

    class speaker_item;

    /**
     * \brief A class that tries to place the balloons such that they don't
     *        overlap.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT balloon_placement
    {
    private:
      /** \brief A character speaking in the scene. */
      struct scene_character
      {
      public:
        scene_character
        ( speaker_item& i, const universe::rectangle_type& r, bool v );

        universe::size_box_type get_balloon_size() const;

        /** \brief The bounding box of the character. */
        const universe::rectangle_type box;

        /** \brief The speaking item. */
        speaker_item& item;

        /** \brief Tell if the speaker is visible. */
        bool visible;

      }; // struct scene_character

      class candidate;
      typedef std::list<candidate*> candidate_group;

      /** \brief A candidate position for a balloon. */
      class candidate
      {
      public:
        /** \brief This function object compares two candidates in decreasing
            order of their quality. */
        struct increasing_conflicts
        {
          bool operator()( const candidate* a, const candidate* b ) const;
        }; // struct increasing_conflicts

      public:
        candidate
        ( const universe::rectangle_type& r, const scene_character& s,
          int score );

        void add_covered_area( double percent );
        void set_in_conflict_with( candidate* c );
        std::size_t get_conclicts_count() const;

        /** \brief Get the candidates conflicting with this one. */
        const candidate_group& get_conflicts() const { return m_conflicts; }

        bool is_valid() const;
        void invalidate();

        int eval() const;

        void print_formatted( std::ostream& os, bool top, bool right ) const;

      public:
        /** \brief The rectangle where the balloon would be. */
        const universe::rectangle_type rect;

        /** \brief The character owning the balloon. */
        const scene_character& speaker;

      private:
        /** \brief The candidates in conflicts with this one. */
        candidate_group m_conflicts;

        /** \brief The size of m_conflicts. */
        int m_conflicts_count;

        /** \brief Tell if this candidate is still usable. */
        bool m_is_valid;

        /** \brief The score of this candidate. */
        int m_score;

        /** \brief Percentage of the area of the balloon covered by something
            else. */
        double m_covered_area;

      }; // class candidate

      /** \brief The list of candidates, where each group concerns the same
          speaker. */
      typedef std::list<candidate_group> candidate_group_list;

      /** \brief This function object compares two groups in decreasing order of
          the quality of their first candidate. */
      struct group_ordering
      {
        bool operator()
        ( const candidate_group& a, const candidate_group& b ) const;
      }; // struct group_ordering

      /** \brief The type of the list in which we store the speakers. */
      typedef std::list<scene_character> character_list_type;

    public:
      balloon_placement
      ( universe::size_type w, universe::size_type h );

      void add_speaker
      ( speaker_item& c, const universe::rectangle_type& rect );

      void place_balloons() const;

    private:
      void sort_candidates( candidate_group_list& c ) const;

      void create_candidates( candidate_group_list& c ) const;
      void
      check_conflicts( candidate_group& g, candidate_group_list& result ) const;

      void new_candidate
      ( const scene_character& c, candidate_group& result,
        universe::coordinate_type left,
        universe::coordinate_type bottom, int score ) const;
      void create_candidate_visible
      ( const scene_character& c, candidate_group& result ) const;
      void create_candidate_not_visible
      ( const scene_character& c, candidate_group& result ) const;

      void repeat_candidate_horizontally
      ( const scene_character& c, candidate_group& result,
        universe::coordinate_type first,
        universe::coordinate_type last,
        universe::coordinate_type y ) const;
      void repeat_candidate_vertically
      ( const scene_character& c, candidate_group& result,
        universe::coordinate_type first,
        universe::coordinate_type last,
        universe::coordinate_type x ) const;
      void repeat_candidate_placed_horizontally
      ( const scene_character& c, candidate_group& result,
        universe::coordinate_type y ) const;
      void repeat_candidate_placed_vertically
      ( const scene_character& c, candidate_group& result,
        universe::coordinate_type x ) const;

      bool check_on_top( const candidate& c ) const;
      bool check_on_right( const candidate& c ) const;

    private:
      /** \brief The size of the view where the speakers are visible. */
      universe::rectangle_type m_view;

      /** \brief The list of all speakers. */
      character_list_type m_characters;

    }; // class balloon_placement

  } // namespace engine
} // namespace bear

#endif // __BEAR_BALLOON_PLACEMENT_HPP__
