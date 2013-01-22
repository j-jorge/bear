/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A snapshot is a set of one placement for each mark.
 * \author Julien Jorge
 */
#ifndef __BF_SNAPSHOT_HPP__
#define __BF_SNAPSHOT_HPP__

#include "bf/mark_placement.hpp"
#include "bf/sound_description.hpp"

#include <claw/box_2d.hpp>
#include <list>
#include <map>

namespace bf
{
  class compiled_file;

  /**
   * \brief A snapshot is a set of one placement for each mark.
   *
   * All snapshots in an action contain the same marks.
   *
   * \author Julien Jorge
   */
  class snapshot
  {
  private:
    /** \brief The type of the structure in which we store the placements of the
        marks. The key is the name of the mark. */
    typedef std::list<mark_placement> placement_list;

  public:
    /** \brief The type of iterator on list of placements. */
    typedef placement_list::const_iterator const_mark_placement_iterator;

    /** \brief The type used to store a rectangle. */
    typedef claw::math::box_2d<double> rectangle_type;

    /**
     * \brief Tell how the bounding box in this snapshot is aligned on the
     *        x-axis according to the previous snapshot.
     */
    struct horizontal_alignment
    {
      /** Tell to keep the left edge at the same position. */
      static const std::string align_left;

      /** Tell to keep the middle axis at the same position. */
      static const std::string align_center;

      /** Tell to keep the right edge at the same position. */
      static const std::string align_right;

    }; // struct horizontal_alignment

    /**
     * \brief Tell how the bounding box in this snapshot is aligned on the
     *        y-axis according to the previous snapshot.
     */
    struct vertical_alignment
    {
      /** Tell to keep the bottom edge at the same position. */
      static const std::string align_bottom;

      /** Tell to keep the middle axis at the same position. */
      static const std::string align_center;

      /** Tell to keep the top edge at the same position. */
      static const std::string align_top;

    }; // struct vertical_alignment()

  public:
    snapshot();
    snapshot
    ( const snapshot& that, const std::map<const mark*, const mark*>& marks );

    double get_date() const;
    void set_date(double date);

    bool has_mark( const std::string& mark_label ) const;
    bool has_mark( const mark* m ) const;
    mark_placement& get_placement( const mark* m );
    mark_placement& get_placement( const std::string& mark_label );
    const mark_placement& get_placement( const mark* m ) const;

    void set_function( const std::string& c );
    const std::string& get_function() const;

    void set_sound( const sound_description& s );
    sound_description& get_sound();
    const sound_description& get_sound() const;

    void add_mark( mark* m );
    void remove_mark( mark* m );

    void get_bounds
    ( double& min_x, double& min_y, double& max_x, double& max_y ) const;

    const_mark_placement_iterator mark_placement_begin() const;
    const_mark_placement_iterator mark_placement_end() const;

    void set_size( double width, double height );
    double get_width() const;
    double get_height() const;

    void set_x_alignment(const std::string& a);
    const std::string& get_x_alignment() const;
    void set_x_alignment_value(double value);
    double get_x_alignment_value() const;

    void set_y_alignment(const std::string& a);
    const std::string& get_y_alignment() const;
    void set_y_alignment_value(double value);
    double get_y_alignment_value() const;

    void compile
    ( compiled_file& f,
      const std::map<std::string, unsigned int>& label_to_int ) const;

  private:
    placement_list::iterator get_iterator( const mark* m );
    placement_list::const_iterator get_const_iterator( const mark* m ) const;
    placement_list::iterator get_iterator( const std::string& mark_label );
    placement_list::const_iterator
    get_const_iterator( const std::string& mark_label ) const;

  private:
    /** \brief The date of the placement. */
    double m_date;

    /** \brief The marks with their placement. */
    placement_list m_placement;

    /** \brief The name of a function to call when passing on this snapshot. */
    std::string m_function;

    /** \brief The sound played when passing on the snapshot. */
    sound_description m_sound;

    /** \brief The width of the snapshot. */
    double m_width;

    /** \brief The height of the snaphot. */
    double m_height;

     /** \brief The alignment on x-coordinate. */
    std::string m_x_alignment;

    /** \brief The alignment on x-coordinate. */
    double m_x_alignment_value;

    /** \brief The alignment on y-coordinate. */
    std::string m_y_alignment;

    /** \brief The alignment on y-coordinate. */
    double m_y_alignment_value;

  }; // class snapshot
} // namespace bf

#endif // __BF_SNAPSHOT_HPP__
