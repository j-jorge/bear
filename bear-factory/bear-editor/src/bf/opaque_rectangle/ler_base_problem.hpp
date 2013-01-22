/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A largest empty rectangle problem.
 * \author Sebastien Angibaud
 */
#ifndef __BF_LER_BASE_PROBLEM_HPP__
#define __BF_LER_BASE_PROBLEM_HPP__

#include <list>
#include <claw/box_2d.hpp>

namespace bf
{
  /**
   * \brief A largest empty rectangle problem.
   *
   * \author Sebastien Angibaud
   */
  class ler_base_problem
  {
  public:
    /** \brief The type of the rectangles used in the problems. */
    typedef claw::math::box_2d<unsigned int> rectangle;

    /** \brief The type of the vectors used for representing coordinates. */
    typedef claw::math::coordinate_2d<unsigned int> point;

    /** \brief Compare two points on their coordinates. */
    struct point_compare_x_min_y_min
    {
      bool operator()
	( const point& pe1,
	  const point& pe2 ) const;
    }; // struct point_compare_x_min_y_min

    /** \brief Compare two points on their coordinates. */
    struct point_compare_x_min_y_max
    {
      bool operator()
	( const point& pe1,
	  const point& pe2 ) const;
    }; // struct point_compare_x_min_y_max

    /** \brief Compare two points on their coordinates. */
    struct point_compare_x_max_y_min
    {
      bool operator()
	( const point& pe1,
	  const point& pe2 ) const;
    }; // struct point_compare_x_max_y_min

    /** \brief Compare two points on their coordinates. */
    struct point_compare_x_max_y_max
    {
      bool operator()
	( const point& pe1,
	  const point& pe2 ) const;
    }; // struct point_compare_x_max_y_max
    
    /** \brief Compare two points on their y-coordinates. */
    struct point_compare_y_min_x_min
    {
      bool operator()
	( const point& pe1,
	  const point& pe2 ) const;
    }; // struct point_compare_y_min_x_min

    /** \brief The type of the iterators. */
    typedef std::list<point>::const_iterator const_iterator;

  public:
    ler_base_problem(unsigned int min_bound = 0);
    virtual ~ler_base_problem();
    
    bool is_terminal() const;
    bool is_a_solution() const;
    void set_min_bound(unsigned int min_bound);
    unsigned int get_min_bound() const;
    const rectangle& get_bounding_rectangle() const;  
    void set_bounding_rectangle(const rectangle& bounding_rectangle);
    
    void divide_on_x(ler_base_problem*& left_pb, ler_base_problem*& right_pb);
    void divide_on_y(ler_base_problem*& bottom_pb, ler_base_problem*& top_pb);
    bool largest_rectangle_in_line(rectangle& result);
    
    unsigned int get_vertical_line() const;
    unsigned int get_horizontal_line() const;
    void compute_min_bound();
    
    virtual bool has_forbidden_points() const = 0;
    virtual unsigned int get_nb_points() const = 0;
    virtual const_iterator first_point() const = 0;
    virtual const_iterator end_point() const = 0;

  private:
    bool largest_rectangle_in_horizontal_line(rectangle& result);
    bool largest_rectangle_in_vertical_line(rectangle& result);

  protected:
    /** \brief The rectangle in which we search the largest empty rectangle.*/
    rectangle m_bounding_rectangle;

    /** \brief The minimal bound.*/
    unsigned int m_min_bound;

  }; // ler_base_problem
} // namespace bf

#endif // __BF_LER_BASE_PROBLEM_HPP__
