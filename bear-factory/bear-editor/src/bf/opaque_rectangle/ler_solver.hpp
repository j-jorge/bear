/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A solver for largest empty rectangle problem.
 * \author Sebastien Angibaud
 */
#ifndef __BF_LER_SOLVER_HPP__
#define __BF_LER_SOLVER_HPP__

#include <vector>

namespace bf
{
  class ler_base_problem;

  /**
   * \brief A solver for largest empty rectangle problem.
   *
   * \author Sebastien Angibaud
   */
  class ler_solver
  {
  public:
    typedef ler_base_problem::point point;

    typedef ler_base_problem::rectangle rectangle;

    typedef std::pair<point, bool> supported_point;

    typedef std::list<point> point_list;

  public:
    ler_solver(ler_base_problem& pb);

    void resolve();
    bool is_solved() const;
    const rectangle& get_solution() const;
    unsigned int get_case_solution() const;

  private:
    void divide_on_x_y();
    void resolve_in_order( std::vector<ler_base_problem*>& pbs);
    void search_rectangle_in_quadrants();
    void resolve_sub_problem(ler_base_problem* pb);
    unsigned int area(const rectangle& r);
    void update_solution(const rectangle& r, unsigned int solution_case);
    void place_points_in_quadrants();
    void sort_quadrants();
    void keep_maximum_points();
    void keep_maximum_points_in_quadrant(point_list& p, bool max);
    void compute_corners();
    void compute_corners_in_quadrant(const point_list &p, point_list& corners);
    void search_on_line();    
    
    void compute_case_1();
    void compute_case_2();
    void compute_case_3();
    void compute_case_4();
    void compute_case_5();
    void compute_case_6();
    void compute_case_7();
    void compute_case_8();
    void compute_case_8_unit(point & p);
    void compute_case_9();
    void compute_case_9_unit(point & p);
    void compute_case_10();
    void compute_case_11();
    void compute_case_12();
    void compute_case_13();
    void compute_case_14();
    void compute_case_15();
    void compute_case_16();

    void compute_c_p_max_max
    ( const point_list& p, unsigned int max_x, unsigned int max_y,
      point_list& c_p );
    void compute_c_p_max_min
    ( const point_list& p, unsigned int max_x, unsigned int min_y,
      point_list& c_p );
    void compute_c_p_min_max
    ( const point_list& p, unsigned int min_x, unsigned int max_y,
      point_list& c_p );
    void compute_c_p_min_min
    ( const point_list& p, unsigned int min_x, unsigned int min_y,
      point_list& c_p );

    supported_point get_point_min_x
    (const point_list & p, unsigned int min_x, unsigned int default_y) const;
    supported_point get_point_max_x
    (const point_list & p, unsigned int max_x, unsigned int default_y) const;
    supported_point get_point_min_y
    (const point_list & p, unsigned int min_y, unsigned int default_x) const;
    supported_point get_point_max_y
    (const point_list & p, unsigned int max_y, unsigned int default_x) const;
    supported_point get_point_end_min_x
    (const point_list & p, unsigned int min_x, unsigned int default_y) const;
    supported_point get_point_end_max_x
    (const point_list & p, unsigned int min_x, unsigned int default_y) const;
    supported_point get_point_end_min_y
    (const point_list & p, unsigned int min_y, unsigned int default_x) const;
    supported_point get_point_end_max_y
    (const point_list & p, unsigned int max_y, unsigned int default_x) const;
    
  private:
    /* \brief The problem to solve. */
    ler_base_problem& m_problem;

    /* \brief The solution. */
    rectangle m_solution;

    /* \brief The case in which we found the best solution. */
    unsigned int m_solution_case;

    /* \brief Indicates if a solution has been found. */
    bool m_solved;

    /* Notation of quadrants :
       
    S2  |  S3
    ----------
    S1  |  S4

    */

    /* \brief List of forbidden points in S1 quadrant. */
    point_list m_s1;

    /* \brief List of forbidden points in S2 quadrant. */
    point_list m_s2;
    
    /* \brief List of forbidden points in S3 quadrant. */
    point_list m_s3;

    /* \brief List of forbidden points in S4 quadrant. */
    point_list m_s4;

    /* \brief List of corner points in S1 quadrant. */
    point_list m_s1_corners;

    /* \brief List of forbidden points in S2 quadrant. */
    point_list m_s2_corners;
    
    /* \brief List of forbidden points in S3 quadrant. */
    point_list m_s3_corners;

    /* \brief List of forbidden points in S4 quadrant. */
    point_list m_s4_corners;

  }; // ler_solver
} // namespace bf

#endif // __BF_LER_SOLVER_HPP__
