/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The placement of a mark (position, angle, depth).
 * \author Julien Jorge
 */
#ifndef __BF_MARK_PLACEMENT_HPP__
#define __BF_MARK_PLACEMENT_HPP__

#include <string>

#include "bear/easing.hpp"

namespace bf
{
  class compiled_file;
  class mark;

  /**
   * \brief The placement of a mark (position, angle, depth).
   * \author Julien Jorge
   */
  class mark_placement
  {
  public:
    explicit mark_placement( const mark* m = NULL );

    void copy( const mark_placement& that );

    void set_x_position( double x );
    void set_y_position( double y );
    void set_position( double x, double y );
    double get_x_position() const;
    double get_y_position() const;

    void set_width( double w );
    void set_height( double h );
    void set_size( double w, double h );
    double get_width() const;
    double get_height() const;

    void set_collision_function( const std::string& f );
    const std::string& get_collision_function() const;

    void set_visibility( bool v );
    bool is_visible() const;

    void set_depth_position( int p );
    int get_depth_position() const;

    void set_angle( double a );
    double get_angle() const;

    const mark* get_mark() const;

    void set_x_easing( const bear::easing& e );
    const bear::easing& get_x_easing() const;

    void set_y_easing( const bear::easing& e );
    const bear::easing& get_y_easing() const;

    void set_width_easing( const bear::easing& e );
    const bear::easing& get_width_easing() const;

    void set_height_easing( const bear::easing& e );
    const bear::easing& get_height_easing() const;

    void set_angle_easing( const bear::easing& e );
    const bear::easing& get_angle_easing() const;

    void compile( compiled_file& f ) const;

  private:
    /** \brief The mark concerned by this placement. */
    const mark* m_mark;

    /** \brief The X-position of the mark. */
    double m_x;

    /** \brief The Y-position of the mark. */
    double m_y;

    /** \brief The width of the box around the mark. */
    double m_width;

    /** \brief The height of the box around the mark. */
    double m_height;

    /** \brief The depth-position of the mark. */
    int m_depth;

    /** \brief The angle of the mark. */
    double m_angle;

    /** \brief Tell if the mark is displayed or not. */
    bool m_visible;

    /** \brief The function called when a collision occurs with this mark. */
    std::string m_collision_function;

    /** \brief The function to use for the easing of the x-position. */
    bear::easing m_x_easing;

    /** \brief The function to use for the easing of the y-position. */
    bear::easing m_y_easing;

    /** \brief The function to use for the easing of the width. */
    bear::easing m_width_easing;

    /** \brief The function to use for the easing of the height. */
    bear::easing m_height_easing;

    /** \brief The function to use for the easing of the angle. */
    bear::easing m_angle_easing;

  }; // class mark_placement
} // namespace bf

#endif // __BF_MARK_PLACEMENT_HPP__
