/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the ler_solver class.
 * \author Sebastien Angibaud
 */
#include "bf/opaque_rectangle/ler_base_problem.hpp"
#include "bf/opaque_rectangle/ler_solver.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pb The problem to solve.
 */
bf::ler_solver::ler_solver( ler_base_problem& pb )
  : m_problem(pb), m_solved(false)
{
  
} // ler_solver::ler_solver()
 
/*----------------------------------------------------------------------------*/
/**
 * \brief Solve the problem.
 */
void bf::ler_solver::resolve()
{
  if ( m_problem.is_terminal() )
    {
      // terminal state
      if ( m_problem.is_a_solution() ) 
	{
	  m_solved = true;
	  m_solution = m_problem.get_bounding_rectangle();
	  m_solution_case = 0;
	  m_problem.set_min_bound(area(m_problem.get_bounding_rectangle()));
	}
    }
  else
    {
      if ( ( m_problem.get_bounding_rectangle().height() == 0 ) ||
	   ( m_problem.get_bounding_rectangle().width() == 0 ) )
	search_on_line();
      else
	{
	  // part 1 and 2
	  divide_on_x_y();
	  
	  // part 3
	  search_rectangle_in_quadrants();
	}
    }
} // ler_solver::resolve()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get if the problem has a solution.
 */
bool bf::ler_solver::is_solved() const
{
  return m_solved;
} // ler_solver::is_solved()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the solution.
 */
const bf::ler_solver::rectangle& bf::ler_solver::get_solution() const
{
  return m_solution;
} // ler_solver::get_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get case in which we found the solution.
 */
unsigned int bf::ler_solver::get_case_solution() const
{
  return m_solution_case;
} // ler_solver::get_case_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide the problem on two sub-problems on x-coordinate.
 */
void bf::ler_solver::divide_on_x_y()
{
  std::vector<ler_base_problem*> pbs;
  pbs.resize(4);

  m_problem.divide_on_x(pbs[0],pbs[1]);
  m_problem.divide_on_y(pbs[2],pbs[3]);
  
  resolve_in_order(pbs);

  for ( unsigned int i = 0; i < 4; ++i )
    delete pbs[i];
} // ler_solver::divide_on_x_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resolve in order sub-problems.
 * \param pbs The problems.
 */
void bf::ler_solver::resolve_in_order( std::vector<ler_base_problem*>& pbs)
{
  std::vector<bool> done(4,false);
  
  for ( unsigned int i=0; i != 4; ++i) 
    if ( pbs[i]->is_terminal() )
      {
	 // terminal state
	if ( pbs[i]->is_a_solution() && 
	     ( ( area(m_solution) < area(pbs[i]->get_bounding_rectangle()) || 
		 ! m_solved ) ) ) 
	  {
	    m_solved = true;
	    m_solution = pbs[i]->get_bounding_rectangle();
	    m_solution_case = 0;
	    m_problem.set_min_bound(area(m_solution));
	  }

	done[i] = true;
      }
  
  for ( unsigned int i=0; i != 4; i++) 
    if ( !done[i] )
      {
	resolve_sub_problem(pbs[i]);
	done[i] = true;
      }
} // ler_solver::resolve_in_order()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search a rectangle that is in at least three quadrants.
 */
void bf::ler_solver::search_rectangle_in_quadrants()
{
  place_points_in_quadrants();
  sort_quadrants();
  keep_maximum_points();
  compute_corners();

  compute_case_1();
  compute_case_2();
  compute_case_3();
  compute_case_4();
  compute_case_5();
  compute_case_6();
  compute_case_7();
  compute_case_8();
  compute_case_9();
  compute_case_10();
  compute_case_11();
  compute_case_12();
  compute_case_13();
  compute_case_14();
  compute_case_15();
  compute_case_16();
} // ler_solver::search_rectangle_in_quadrants()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resolve a sub-problem.
 */
void bf::ler_solver::resolve_sub_problem(ler_base_problem* pb)
{
  pb->set_min_bound( m_problem.get_min_bound() );

  ler_solver solver(*pb);
  
  solver.resolve();
  
  if ( solver.is_solved() )
    update_solution(solver.get_solution(), solver.get_case_solution());
} // ler_solver::resolve_sub_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update solution.
 * \param r A new candidate for the best solution.
 * \param solution_case The case in which the solution was found.
 */
void bf::ler_solver::update_solution
(const rectangle& r, unsigned int solution_case)
{
  if ( ( !m_solved || ( area(r) > area(m_solution) ) ) &&
       ( area(r) > m_problem.get_min_bound() ) )
    {
      m_solution = r;
      m_solution_case = solution_case;
      m_solved = true;
      m_problem.set_min_bound(area(r));
    }
} // ler_solver::update_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the area of a rectangle.
 * \param r The rectangle in witch we compute the area.
 */
unsigned int bf::ler_solver::area(const rectangle& r)
{
  return ( r.width() + 1 ) * (r.height() + 1);
} // ler_solver::area()

/*----------------------------------------------------------------------------*/
/**
 * \brief Place each forbidden point into the corresponding quadrant.
 */
void bf::ler_solver::place_points_in_quadrants()
{
  unsigned int l_v; // vertical_line to separe left and right area
  l_v = m_problem.get_vertical_line();

  unsigned int l_h; // horizontal_line to separe bottom and top area
  l_h = m_problem.get_horizontal_line();

  ler_base_problem::const_iterator it;
  // separe forbidden points in for groups
  for ( it = m_problem.first_point(); it != m_problem.end_point(); ++it)
    {
      if ( it->x <= l_v ) 
	{
	  if ( it->y <= l_h ) 
	    m_s1.push_back(*it);
	  else
	    m_s2.push_back(*it);
	}
      else 
	{
	  if ( it->y <= l_h ) 
	    m_s4.push_back(*it);
	  else
	    m_s3.push_back(*it);
	}     
    }
} // ler_solver::place_points_in_quadrants()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort quadrants.
 */
void bf::ler_solver::sort_quadrants()
{
  m_s1.sort(ler_base_problem::point_compare_x_max_y_max());
  m_s2.sort(ler_base_problem::point_compare_x_max_y_min());
  m_s3.sort(ler_base_problem::point_compare_x_min_y_min());
  m_s4.sort(ler_base_problem::point_compare_x_min_y_max());
} // ler_solver::sort_quandrants()

/*----------------------------------------------------------------------------*/
/**
 * \brief Keep only maximum points in each quadrant.
 */
void bf::ler_solver::keep_maximum_points()
{
  keep_maximum_points_in_quadrant(m_s1,true);
  keep_maximum_points_in_quadrant(m_s2,false);
  keep_maximum_points_in_quadrant(m_s3,false);
  keep_maximum_points_in_quadrant(m_s4,true);
} // keep_maximum_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief Keep only maximum points in a given quadrant.
 * \param p The list of points.
 * \param max Indicates if we must takes maximum y.
 */
void bf::ler_solver::keep_maximum_points_in_quadrant(point_list& p, bool max)
{
  if ( ! p.empty() )
    {
      point_list result;

      point_list::const_iterator it = p.begin();
      unsigned int y_ref = it->y;
      result.push_back(*it);

      for ( ; it != p.end(); ++it )
	if ( ( ( it->y > y_ref ) && max ) ||
	     ( ( it->y < y_ref ) && !max ) )
	  {
	    result.push_back(*it);
	    y_ref = it->y;
	  }

      swap(p,result);
    } 
} // keep_maximum_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute conrers points in each quadrant.
 */
void bf::ler_solver::compute_corners()
{
  compute_corners_in_quadrant(m_s1,m_s1_corners);
  compute_corners_in_quadrant(m_s2,m_s2_corners);
  compute_corners_in_quadrant(m_s3,m_s3_corners);
  compute_corners_in_quadrant(m_s4,m_s4_corners);
} // ler_solver::compute_corners()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute corners points in each quadrant.
 * \param p The list of points.
 * \param corners The list of resulting points.
 */
void bf::ler_solver::compute_corners_in_quadrant
(const point_list &p, point_list& corners)
{
  if ( ! p.empty() )
    {
      point_list::const_iterator it = p.begin();
      point_list::const_iterator previous = p.begin();
      ++it;
      if ( it != p.end() )
	for ( ; it != p.end(); ++it, previous++ )
	  corners.push_back(point(it->x,previous->y));
    }
} // ler_solver::compute_corners_in_quadrant()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the largest rectangle on a line.
 */
void bf::ler_solver::search_on_line()
{
  rectangle r;
  
  if ( m_problem.largest_rectangle_in_line(r) )
    update_solution(r, 17); 
} // ler_solver::search_on_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 1.
 */
void bf::ler_solver::compute_case_1()
{
  // type B
  point_list::iterator it;

  for ( it = m_s4_corners.begin(); it != m_s4_corners.end(); ++it )
    {
      supported_point p3 = 
	get_point_end_max_x
	(m_s3,it->x,m_problem.get_bounding_rectangle().top());
      supported_point p2 = 
	get_point_max_y
	(m_s2,p3.first.y,m_problem.get_bounding_rectangle().left());
      
      point_list c_p;
      compute_c_p_min_min(m_s1,p2.first.x,it->y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2;	  
	  
	  if ( p2.second )
	    q1.x = p2.first.x + 1;
	  else
	    q1.x = p2.first.x;
	  q1.y = it->y + 1;

	  q2.x = it->x - 1;
	  if ( p3.second )
	    q2.y = p3.first.y - 1;
	  else
	    q2.y = p3.first.y;
	  
	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),1);
	}
    }
} // ler_solver::compute_case_1()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 2.
 */
void bf::ler_solver::compute_case_2()
{
  // type B
  point_list::iterator it;

  for ( it = m_s3_corners.begin(); it != m_s3_corners.end(); ++it )
    {
      supported_point p2 = 
	get_point_max_y
	(m_s2,it->y,m_problem.get_bounding_rectangle().left());
      supported_point p4 = 
	get_point_end_max_x
	(m_s4,it->x,m_problem.get_bounding_rectangle().bottom());

      point_list c_p;
      compute_c_p_min_min(m_s1,p2.first.x,p4.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2(it->x - 1, it->y - 1);	  
	  
	  if ( p2.second )
	    q1.x = p2.first.x + 1;
	  else
	    q1.x = p2.first.x;

	  if ( p4.second )
	    q1.y = p4.first.y + 1;
	  else
	    q1.y = p4.first.y;
	  
	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),2);
	}
    }
} // ler_solver::compute_case_2()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 3.
 */
void bf::ler_solver::compute_case_3()
{
  // type A
  point_list::iterator it;

  for ( it = m_s2_corners.begin(); it != m_s2_corners.end(); ++it )
    {
      supported_point p1 = 
	get_point_end_min_x
	(m_s1,it->x,m_problem.get_bounding_rectangle().bottom());
      supported_point p3 = 
	get_point_max_y(m_s3,it->y,m_problem.get_bounding_rectangle().right());

      point_list c_p;
      compute_c_p_max_min(m_s4,p3.first.x,p1.first.y,c_p);
      
      if ( ! c_p.empty() )
	{
	  point_list::iterator it2;
	  for ( it2 = c_p.begin(); it2 != c_p.end(); ++it2 )
	    {
	      point p1(it->x + 1, it2->y + 1);
	      point p2(it2->x - 1, it->y + 1);
	      update_solution(rectangle(p1,p2),3);
	    }
	}
    }
} // ler_solver::compute_case_3()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 4.
 */
void bf::ler_solver::compute_case_4()
{
  // type B
  point_list::iterator it;

  for ( it = m_s2_corners.begin(); it != m_s2_corners.end(); ++it )
    {
      supported_point p3 = 
	get_point_max_y(m_s3,it->y,m_problem.get_bounding_rectangle().right());
      supported_point p4 = 
	get_point_end_max_x
	(m_s4,p3.first.x,m_problem.get_bounding_rectangle().bottom());
      
      point_list c_p;
      compute_c_p_min_min(m_s1,it->x,p4.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2;
	  
	  q1.x = it->x + 1;
	  if ( p4.second )
	    q1.y = p4.first.y + 1;
	  else
	    q1.y = p4.first.y;
	      
	  if ( p3.second )
	    q2.x = p3.first.x - 1;
	  else
	    q2.x = p3.first.x;
	  q2.y = it->y - 1;

	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),4);
	}
    }
} // ler_solver::compute_case_4()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 5.
 */
void bf::ler_solver::compute_case_5()
{
  // type B
  point_list::iterator it;

  for ( it = m_s4_corners.begin(); it != m_s4_corners.end(); ++it )
    {
      supported_point p1 = 
	get_point_min_y
	(m_s1,it->y,m_problem.get_bounding_rectangle().left());
      supported_point p3 = 
	get_point_end_max_x
	(m_s3,it->x,m_problem.get_bounding_rectangle().top());

      point_list c_p;
      compute_c_p_min_max(m_s2,p1.first.x,p3.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2;	  
	  
	  if ( p1.second )
	    q1.x = p1.first.x + 1;
	  else
	    q1.x = p1.first.x;
	  q1.y = it->y + 1;

	  q2.x = it->x;
	  if ( p3.second  )
	    q2.y = p3.first.y - 1;
	  else
	    q2.y = p3.first.y - 1;
	  
	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),5);
	}
    }
} // ler_solver::compute_case_5()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 6.
 */
void bf::ler_solver::compute_case_6()
{
  // type B
  point_list::iterator it;

  for ( it = m_s3_corners.begin(); it != m_s3_corners.end(); ++it )
    {
      supported_point p4 = 
	get_point_end_max_x
	(m_s4,it->x,m_problem.get_bounding_rectangle().bottom());
      supported_point p1 = 
	get_point_min_y
	(m_s1,p4.first.y,m_problem.get_bounding_rectangle().left());
      
      point_list c_p;
      compute_c_p_min_max(m_s2,p1.first.x,it->y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2(it->x - 1, it->y - 1);	  
	  
	  if ( p1.second )
	    q1.x = p1.first.x + 1;
	  else
	    q1.x = p1.first.x;

	  if ( p4.second )
	    q1.y = p4.first.y + 1;
	  else
	    q1.y = p4.first.y;
	  
	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),6);
	}
    }
} // ler_solver::compute_case_6()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 7.
 */
void bf::ler_solver::compute_case_7()
{
  // type B
point_list::iterator it;

  for ( it = m_s4_corners.begin(); it != m_s4_corners.end(); ++it )
    {
      supported_point p1 = 
	get_point_min_y
	(m_s1,it->y,m_problem.get_bounding_rectangle().left());
      supported_point p2 = 
	get_point_end_min_x
	(m_s2,p1.first.x,m_problem.get_bounding_rectangle().top());

      point_list c_p;
      compute_c_p_max_max(m_s3,it->x,p2.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2;	  
	  
	  if ( p1.second )
	    q1.x = p1.first.x + 1;
	  else
	    q1.x = p1.first.x;
	  q1.y = it->y + 1;

	  q2.x = it->x - 1;
	  if ( p2.second )
	    q2.y = p2.first.y - 1;
	  else
	    q2.y = p2.first.y;
	  
	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),7);
	}
    }
} // ler_solver::compute_case_7()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 8.
 */
void bf::ler_solver::compute_case_8()
{
  // type C
  point_list::iterator it;

  for ( it = m_s1.begin(); it != m_s1.end(); ++it )
    compute_case_8_unit(*it);

  point p(0,m_problem.get_horizontal_line());
  compute_case_8_unit(p);
} // ler_solver::compute_case_8()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 8.
 */
void bf::ler_solver::compute_case_8_unit(point & p)
{
  supported_point p2 = 
    get_point_end_min_x
    (m_s2,p.x,m_problem.get_bounding_rectangle().top());
  supported_point p3 = 
    get_point_max_y
    (m_s3,p2.first.y,m_problem.get_bounding_rectangle().right());
  supported_point p4 = 
    get_point_end_max_x
    (m_s4,p3.first.x,m_problem.get_bounding_rectangle().bottom());
  supported_point p1 = 
    get_point_min_y
    (m_s1,p4.first.y,m_problem.get_bounding_rectangle().left());
  
  if ( p.x == p1.first.x )
    {
      point q1;
      point q2;
      
      if ( p1.second )
	q1.x = p1.first.x + 1;
      else
	q1.x = p1.first.x;
      
      if (  p4.second )
	q1.y = p4.first.y + 1;
      else
	q1.y = p4.first.y;
      
      if (  p3.second )
	q2.x = p3.first.x - 1;
      else
	q2.x = p3.first.x;
      
      if (  p2.second )
	q2.y = p2.first.y - 1;
      else
	q2.y = p2.first.y;
      
       if ( (q1.x <= q2.x) && (q1.y <= q2.y) ) 
	 update_solution(rectangle(q1,q2),8);
    }
} // ler_solver::compute_case_8_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 9.
 */
void bf::ler_solver::compute_case_9()
{
  // type C
  point_list::iterator it;

  for ( it = m_s1.begin(); it != m_s1.end(); ++it )
    compute_case_9_unit(*it);

  point p(m_problem.get_vertical_line(),0);
  compute_case_9_unit(p);
} // ler_solver::compute_case_9()
/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 9.
 */
void bf::ler_solver::compute_case_9_unit(point & p)
{
  supported_point p4 = 
    get_point_min_y
    (m_s4,p.y,m_problem.get_bounding_rectangle().right());
  supported_point p3 = 
    get_point_end_max_x
    (m_s3,p4.first.x,m_problem.get_bounding_rectangle().top());
  supported_point p2 = 
    get_point_max_y
    (m_s2,p3.first.y,m_problem.get_bounding_rectangle().left());
  supported_point p1 = 
    get_point_end_min_x
    (m_s1,p2.first.x,m_problem.get_bounding_rectangle().bottom());
  
  if ( p.y == p1.first.y )
    {
      point q1;
      point q2;
      
      if ( p2.second )
	q1.x = p2.first.x + 1;
      else
	q1.x = p2.first.x;
      
      if ( p1.second )
	q1.y = p1.first.y + 1;
      else
	q1.y = p1.first.y;
      
      if ( p4.second )
	q2.x = p4.first.x - 1;
      else
	q2.x = p4.first.x;
      
      if ( p3.second )
	q2.y = p3.first.y - 1;
      else
	q2.y = p3.first.y;
      
      if ( (q1.x <= q2.x) && (q1.y <= q2.y) ) 
	update_solution(rectangle(q1,q2),9);
    }
} // ler_solver::compute_case_9()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 10.
 */
void bf::ler_solver::compute_case_10()
{
  // type B
  point_list::iterator it;

  for ( it = m_s3_corners.begin(); it != m_s3_corners.end(); ++it )
    {
      supported_point p2 = 
	get_point_max_y
	(m_s2,it->y,m_problem.get_bounding_rectangle().left());
      supported_point p1 = 
	get_point_end_min_x
	(m_s1,p2.first.x,m_problem.get_bounding_rectangle().bottom());

      point_list c_p;
      compute_c_p_max_min(m_s4,it->x,p1.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2(it->x - 1, it->y - 1);	  
	  
	  if ( p2.second )
	    q1.x = p2.first.x + 1;
	  else
	    q1.x = p2.first.x;

	  if ( p1.second )
	    q1.y = p1.first.y + 1;
	  else
	    q1.y = p1.first.y;
	  
	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),10);
	}
    }
} // ler_solver::compute_case_10()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 11.
 */
void bf::ler_solver::compute_case_11()
{
  // type B
  point_list::iterator it;

  for ( it = m_s2_corners.begin(); it != m_s2_corners.end(); ++it )
    {
      supported_point p1 = 
	get_point_end_min_x
	(m_s1,it->x,m_problem.get_bounding_rectangle().bottom());
      supported_point p4 = 
	get_point_min_y
	(m_s4,p1.first.y,m_problem.get_bounding_rectangle().right());

      point_list c_p;
      compute_c_p_max_max(m_s3,p4.first.x,it->y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2;
	  
	  q1.x = it->x + 1;
	  if ( p1.second )
	    q1.y = p1.first.y + 1;
	  else
	    q1.y = p1.first.y;
	      
	  if ( p4.second )
	    q2.x = p4.first.x - 1;
	  else
	    q2.x = p4.first.x;
	  q2.y = it->y - 1;

	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),11);
	}
    }
} // ler_solver::compute_case_11()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 12.
 */
void bf::ler_solver::compute_case_12()
{
  // type B
  point_list::iterator it;

  for ( it = m_s2_corners.begin(); it != m_s2_corners.end(); ++it )
    {
      supported_point p1 = 
	get_point_end_min_x
	(m_s1,it->x,m_problem.get_bounding_rectangle().bottom());
      supported_point p3 = 
	get_point_max_y(m_s3,it->y,m_problem.get_bounding_rectangle().right());

      point_list c_p;
      compute_c_p_max_min(m_s4,p3.first.x,p1.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1;
	  point q2;
	  
	  q1.x = it->x + 1;
	  if ( p1.second )
	    q1.y = p1.first.y + 1;
	  else
	    q1.y = p1.first.y;
	      
	  if ( p3.second )
	    q2.x = p3.first.x - 1;
	  else
	    q2.x = p3.first.x;
	  q2.y = it->y - 1;

	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),12);
	}
    }
} // ler_solver::compute_case_12()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 13.
 */
void bf::ler_solver::compute_case_13()
{
  // type B
  point_list::iterator it;

  for ( it = m_s1_corners.begin(); it != m_s1_corners.end(); ++it )
    {
      supported_point p4 = 
	get_point_min_y(m_s4,it->y,m_problem.get_bounding_rectangle().right());
      supported_point p3 = 
	get_point_end_max_x
	(m_s3,p4.first.x,m_problem.get_bounding_rectangle().top());
      
      point_list c_p;
      compute_c_p_min_max(m_s2,it->x,p3.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1(it->x + 1, it->y + 1);
	  point q2;

	  if ( p4.second )
	    q2.x = p4.first.x - 1;
	  else
	    q2.x = p4.first.x;
	      
	  if ( p3.second )
	    q2.y = p3.first.y - 1;
	  else
	    q2.y = p3.first.y;

	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),13);
	}
    }
} // ler_solver::compute_case_13()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 14.
 */
void bf::ler_solver::compute_case_14()
{
  // type A
  point_list::iterator it;

  for ( it = m_s1_corners.begin(); it != m_s1_corners.end(); ++it )
    {
      supported_point p2 = 
	get_point_end_min_x
	(m_s2,it->x,m_problem.get_bounding_rectangle().top());
      supported_point p4 = 
	get_point_min_y(m_s4,it->y,m_problem.get_bounding_rectangle().right());

      point_list c_p;
      compute_c_p_max_max(m_s3,p4.first.x,p2.first.y,c_p);
      
      if ( ! c_p.empty() )
	{
	  point_list::iterator it2;
	  for ( it2 = c_p.begin(); it2 != c_p.end(); ++it2 )
	    {
	      point p1(it->x + 1, it->y + 1);
	      point p2(it2->x - 1, it2->y - 1);
	      if ( (p1.x <= p2.x) && (p1.y <= p2.y) )
		update_solution(rectangle(p1,p2),14);
	    }
	}
    }
} // ler_solver::compute_case_14()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 15.
 */
void bf::ler_solver::compute_case_15()
{
  // type B
  point_list::iterator it;

  for ( it = m_s1_corners.begin(); it != m_s1_corners.end(); ++it )
    {
      supported_point p2 = 
	get_point_end_min_x
	(m_s2,it->x,m_problem.get_bounding_rectangle().top());
      supported_point p4 = 
	get_point_min_y(m_s4,it->y,m_problem.get_bounding_rectangle().right());

      point_list c_p;
      compute_c_p_max_max(m_s3,p4.first.x,p2.first.y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1(it->x + 1, it->y + 1);
	  point q2;

	  if ( p4.second )
	    q2.x = p4.first.x - 1;
	  else
	    q2.x = p4.first.x;
	      
	  if ( p2.second )
	    q2.y = p2.first.y - 1;
	  else
	    q2.y = p2.first.y;

	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),15);
	}
    }
} // ler_solver::compute_case_15()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle in case 16.
 */
void bf::ler_solver::compute_case_16()
{
  // type B
  point_list::iterator it;

  for ( it = m_s1_corners.begin(); it != m_s1_corners.end(); ++it )
    {
      supported_point p2 = 
	get_point_end_min_x
	(m_s2,it->x,m_problem.get_bounding_rectangle().top());
      supported_point p3 = 
	get_point_max_y
	(m_s3,p2.first.y,m_problem.get_bounding_rectangle().right());

      point_list c_p;
      compute_c_p_max_min(m_s4,p3.first.x,it->y,c_p);
      
      if ( c_p.empty() )
	{
	  point q1(it->x + 1, it->y + 1);
	  point q2;

	  if ( p3.second )
	    q2.x = p3.first.x - 1;
	  else
	    q2.x = p3.first.x;
	      
	  if ( p2.second )
	    q2.y = p2.first.y - 1;
	  else
	    q2.y = p2.first.y;

	  if ( (q1.x <= q2.x) && (q1.y <= q2.y) )
	    update_solution(rectangle(q1,q2),16);
	}
    }
} // ler_solver::compute_case_16()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle of type C.
 */
void bf::ler_solver::compute_c_p_max_max
(const point_list& p, unsigned int max_x, unsigned int max_y, point_list& c_p)
{
  point_list::const_iterator it;

  for ( it = p.begin(); it != p.end(); ++it )
    if ( it->x < max_x && it->y < max_y)
      c_p.push_back(*it);
} // ler_solver::compute_c_p_max_max()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle of type C.
 */
void bf::ler_solver::compute_c_p_max_min
(const point_list& p, unsigned int max_x, unsigned int min_y, point_list& c_p)
{
  point_list::const_iterator it;

  for ( it = p.begin(); it != p.end(); ++it )
    if ( it->x < max_x && it->y > min_y)
      c_p.push_back(*it);
} // ler_solver::compute_c_p_max_min()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle of type C.
 */
void bf::ler_solver::compute_c_p_min_max
(const point_list& p, unsigned int min_x, unsigned int max_y, point_list& c_p)
{
  point_list::const_iterator it;

  for ( it = p.begin(); it != p.end(); ++it )
    if ( it->x > min_x && it->y < max_y)
      c_p.push_back(*it);
} // ler_solver::compute_c_p_min_max()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute rectangle of type C.
 */
void bf::ler_solver::compute_c_p_min_min
(const point_list& p, unsigned int min_x, unsigned int min_y, point_list& c_p)
{
  point_list::const_iterator it;

  for ( it = p.begin(); it != p.end(); ++it )
    if ( it->x > min_x && it->y > min_y)
      c_p.push_back(*it);
} // ler_solver::compute_c_p_min_min()


bf::ler_solver::supported_point bf::ler_solver::get_point_min_x
(const point_list& p, unsigned int min_x, unsigned int default_y) const
{
  supported_point result;
  result.first.x = min_x;
  result.first.y = default_y;
  result.second = false;

  point_list::const_iterator it;
  bool found = false;

  for ( it = p.begin(); (it != p.end()) && (! found); ++it )
    {
      if ( it->x > min_x )
	{
	  found = true;
	  result.first = *it;
	  result.second = true;
	}
    }

  return result;
} // ler_solver::get_point_min_x()

bf::ler_solver::supported_point bf::ler_solver::get_point_max_x
(const point_list& p, unsigned int max_x, unsigned int default_y) const
{
  supported_point result;
  result.first.x = max_x;
  result.first.y = default_y;
  result.second = false;

  point_list::const_iterator it;
  bool found = false;

  for ( it = p.begin(); (it != p.end()) && (! found); ++it )
    {
      if ( it->x < max_x )
	{
	  found = true;
	  result.first = *it;
	  result.second = true;
	}
    }

  return result;
} // ler_solver::get_point_max_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
bf::ler_solver::supported_point bf::ler_solver::get_point_min_y
(const point_list& p, unsigned int min_y, unsigned int default_x) const
{
  supported_point result;
  result.first.x = default_x;
  result.first.y = min_y;
  result.second = false;

  point_list::const_iterator it;
  bool found = false;

  for ( it = p.begin(); (it != p.end()) && (! found); ++it )
    {
      if ( it->y > min_y )
	{
	  found = true;
	  result.first = *it;
	  result.second = true;
	}
    }

  return result;
} // ler_solver::get_point_min_y()

bf::ler_solver::supported_point bf::ler_solver::get_point_max_y
(const point_list& p, unsigned int max_y, unsigned int default_x) const
{
  supported_point result;
  result.first.x = default_x;
  result.first.y = max_y;
  result.second = false;

  point_list::const_iterator it;
  bool found = false;

  for ( it = p.begin(); (it != p.end()) && (! found); ++it )
    {
      if ( it->y < max_y )
	{
	  found = true;
	  result.first = *it;
	  result.second = true;
	}
    }

  return result;
} // ler_solver::get_point_max_y()

bf::ler_solver::supported_point bf::ler_solver::get_point_end_min_x
(const point_list& p, unsigned int min_x, unsigned int default_y) const
{
  supported_point result;
  result.first.x = min_x;
  result.first.y = default_y;
  result.second = false;
  
  if ( ! p.empty() ) 
    {
      bool found = false;
      point_list::const_iterator it = p.end();
      do
	{
	  --it;
	  if ( it->x > min_x )
	    {
	      found = true;
	      result.first = *it;
	      result.second = true;
	    }
	}
      while ( !found && (it != p.begin()) );
    }

  return result;
} // ler_solver::get_point_end_min_x()

bf::ler_solver::supported_point bf::ler_solver::get_point_end_max_x
(const point_list& p, unsigned int max_x, unsigned int default_y) const
{
  supported_point result;
  result.first.x = max_x;
  result.first.y = default_y;
  result.second = false;
  
  if ( ! p.empty() ) 
    {
      bool found = false;
      point_list::const_iterator it = p.end();
      do
	{
	  --it;
	  if ( it->x < max_x )
	    {
	      found = true;
	      result.first = *it;
	      result.second = true;
	    }
	}
      while ( !found && (it != p.begin()) );
    }

  return result;
} // ler_solver::get_point_end_max_x()

bf::ler_solver::supported_point bf::ler_solver::get_point_end_min_y
(const point_list& p, unsigned int min_y, unsigned int default_x) const
{
  supported_point result;
  result.first.x = default_x;
  result.first.y = min_y;
  result.second = false;
  
  if ( ! p.empty() ) 
    {
      bool found = false;
      point_list::const_iterator it = p.end();
      do
	{
	  --it;
	  if ( it->y > min_y )
	    {
	      found = true;
	      result.first = *it;
	      result.second = true;
	    }
	}
      while ( !found && (it != p.begin()) );
    }

  return result;
} // ler_solver::get_point_end_min_x()

bf::ler_solver::supported_point bf::ler_solver::get_point_end_max_y
(const point_list& p, unsigned int max_y, unsigned int default_x) const
{
  supported_point result;
  result.first.x = default_x;
  result.first.y = max_y;
  result.second = false;
  
  if ( ! p.empty() ) 
    {
      bool found = false;
      point_list::const_iterator it = p.end();
      do
	{
	  --it;
	  if ( it->y < max_y )
	    {
	      found = true;
	      result.first = *it;
	      result.second = true;
	    }
	}
      while ( !found && (it != p.begin()) );
    }

  return result;
} // ler_solver::get_point_end_max_x()


