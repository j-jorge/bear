/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the ler_base_problem class.
 * \author Sebastien Angibaud
 */
#include "bf/opaque_rectangle/ler_base_problem.hpp"
#include "bf/opaque_rectangle/ler_problem.hpp"
#include "bf/opaque_rectangle/ler_horizontal_problem.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two points on their coordinates.
 */
bool bf::ler_base_problem::point_compare_x_min_y_min::operator()
  (  const point& p1, const point& p2 ) const
{
  if ( p1.x < p2.x )
    return true;
  else if ( p1.x == p2.x )
    return p1.y < p2.y;
  else
    return false;
} // point_compare_x_min_y_min::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two points on their coordinates.
 */
bool bf::ler_base_problem::point_compare_x_min_y_max::operator()
  (  const point& p1, const point& p2 ) const
{
  if ( p1.x < p2.x )
    return true;
  else if ( p1.x == p2.x )
    return p1.y > p2.y;
  else
    return false;
} // point_compare_x_min_y_max::operator()()


/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two points on their coordinates.
 */
bool bf::ler_base_problem::point_compare_x_max_y_min::operator()
  (  const point& p1, const point& p2 ) const
{
  if ( p1.x > p2.x )
    return true;
  else if ( p1.x == p2.x )
    return p1.y < p2.y;
  else
    return false;
} // point_compare_x_max_y_min::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two points on their coordinates.
 */
bool bf::ler_base_problem::point_compare_x_max_y_max::operator()
  (  const point& p1, const point& p2 ) const
{
  if ( p1.x > p2.x )
    return true;
  else if ( p1.x == p2.x )
    return p1.y > p2.y;
  else
    return false;
} // point_compare_x_max_y_max::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two point on their y-coordinates.
 */
bool bf::ler_base_problem::point_compare_y_min_x_min::operator()
  (  const point& p1, const point& p2 ) const
{
  if ( p1.y < p2.y )
    return true;
  else if ( p1.y == p2.y )
    return p1.x < p2.x;
  else
    return false;
} // point_compare_y_min_x_min::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::ler_base_problem::ler_base_problem(unsigned int min_bound)
  : m_min_bound(min_bound)
{

} // ler_base_problem::ler_base_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::ler_base_problem::~ler_base_problem()
{

} // ler_base_problem::~ler_base_problem()
 
/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the problem is in a terminal state.
 */
bool bf::ler_base_problem::is_terminal() const
{
  return ( ! has_forbidden_points() ) || 
    ( (m_bounding_rectangle.width() == 0) && 
      (m_bounding_rectangle.height() == 0) ) || 
    ( get_nb_points() == 
      (m_bounding_rectangle.width()+1) * (m_bounding_rectangle.height()+1) ) ||
    ( (m_bounding_rectangle.width()+1) * (m_bounding_rectangle.height()+1) < 
      m_min_bound ) ;
} // ler_base_problem::is_terminal()
 
/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the problem is a solution.
 */
bool bf::ler_base_problem::is_a_solution() const
{
  return ( ! has_forbidden_points() ) &&
    ( (m_bounding_rectangle.width()+1) * (m_bounding_rectangle.height()+1) >= 
      m_min_bound )  ;
} // ler_base_problem::is_a_solution()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the bounding rectangle.
 */
const bf::ler_base_problem::rectangle&
bf::ler_base_problem::get_bounding_rectangle() const
{
  return m_bounding_rectangle;
} // ler_base_problem::get_bounding_rectangle() 

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bounding rectangle.
 * \param bounding_rectangle The new bounding rectangle.
 */
void bf::ler_base_problem::set_bounding_rectangle
(const rectangle& bounding_rectangle)
{
  m_bounding_rectangle = bounding_rectangle;
} // ler_base_problem::set_bounding_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the minimal bound.
 * \param min_bound The minimal bound.
 */
void bf::ler_base_problem::set_min_bound(unsigned int min_bound)
{
  m_min_bound = min_bound;
} // ler_base_problem::set_min_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the minimal bound.
 */
unsigned int bf::ler_base_problem::get_min_bound() const
{
  return m_min_bound;
} // ler_base_problem::get_min_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide the problem into two sub-problems (left and right problems).
 * \param left_pb The left problem to construct.
 * \param right_pb The right problem to construct.
 */
void bf::ler_base_problem::divide_on_x
( ler_base_problem*& left_pb, ler_base_problem*& right_pb )
{
  const_iterator it;
  const_iterator it_end = end_point();
  bool ok = true;

  unsigned int l_v; // vertical_line to separe right and left area
  l_v = get_vertical_line();

  // separe forbidden points in two groups
  for ( it = first_point(); (it != end_point()) && ok; ++it)
    {
      if ( it->x > l_v )
	{
	  ok = false;
	  it_end = it;
	}
    }

  left_pb = new ler_horizontal_problem(first_point(), it_end, get_min_bound());
  right_pb = new ler_horizontal_problem(it_end, end_point(), get_min_bound());
  
  rectangle left_rectangle
    (point(m_bounding_rectangle.left(),m_bounding_rectangle.bottom()),
     point(l_v,m_bounding_rectangle.top()));
  left_pb->set_bounding_rectangle(left_rectangle);

  rectangle right_rectangle
    (point(l_v+1,m_bounding_rectangle.bottom()),
     point(m_bounding_rectangle.right(),m_bounding_rectangle.top()));
  right_pb->set_bounding_rectangle(right_rectangle);
} // ler_base_problem::divide_on_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide the problem into two sub-problems (bottom and top problems).
 * \param bottom_pb The bottom problem to construct.
 * \param top_pb The top problem to construct.
 */
void bf::ler_base_problem::divide_on_y
(ler_base_problem*& bottom_pb, ler_base_problem*& top_pb)
{
  ler_problem* b_pb = new ler_problem(get_min_bound());
  ler_problem* t_pb = new ler_problem(get_min_bound());
  
  const_iterator it;
 
  unsigned int l_h; // horizontal_line to separe bottom and top area
  l_h = get_horizontal_line();

  // separe forbidden points in two groups
  for ( it = first_point(); it != end_point(); ++it)
    {
      if ( it->y <= l_h ) 
	b_pb->add_forbidden_point(*it);
      else 
	t_pb->add_forbidden_point(*it);
    }      

  bottom_pb = b_pb;
  top_pb = t_pb;

  rectangle bottom_rectangle
    (point(m_bounding_rectangle.left(),m_bounding_rectangle.bottom()),
     point(m_bounding_rectangle.right(),l_h));
  bottom_pb->set_bounding_rectangle(bottom_rectangle);

  rectangle top_rectangle
    (point(m_bounding_rectangle.left(),l_h+1),
     point(m_bounding_rectangle.right(),m_bounding_rectangle.top()));
  top_pb->set_bounding_rectangle(top_rectangle);
} // ler_base_problem::divide_on_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the largest empty rectangle in a line.
 * return true if a empty rectangle is found.
 * \param result The largest rectangle.
 */
bool bf::ler_base_problem::largest_rectangle_in_line(rectangle& result)
{
  if ( ( m_bounding_rectangle.width() != 0 ) && 
       ( m_bounding_rectangle.height() != 0 ) )
    // not a line
    return false;

  if ( ! has_forbidden_points() )
    {
      result = m_bounding_rectangle;
      return true;
    }
  else
    {
      if ( m_bounding_rectangle.width() == 0 )
	return largest_rectangle_in_vertical_line(result);
      else
	return largest_rectangle_in_horizontal_line(result);
    }
} // ler_base_problem::largest_rectangle_in_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the vertical line that separates problems into two sub-problems.
 */
unsigned int bf::ler_base_problem::get_vertical_line() const
{
  return m_bounding_rectangle.left() + m_bounding_rectangle.width() / 2;
} // ler_base_problem::get_vertical_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the horizontal line that separates problems into two 
 * sub-problems.
 */
unsigned int bf::ler_base_problem::get_horizontal_line() const
{
  return m_bounding_rectangle.bottom() + m_bounding_rectangle.height() / 2;
} // ler_base_problem::get_horizontal_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute a greedy minimal bound.
 */
void bf::ler_base_problem::compute_min_bound()
{
  m_min_bound = 0;
  const_iterator it = first_point();

  for ( ; it != end_point(); ++it)
    {
      std::list<point> maximal;
      point last_point(it->x,-1);
      const_iterator it2 = it;

      for ( ++it2; it2 != end_point(); ++it2)
	{
	  if ( ( it2->x > last_point.x ) && 
	       ( it2->y < last_point.y ) && 
	       ( it2->y > it->y ) )
	    {
	      last_point = *it2;
	      maximal.push_back(*it2);
	    }
	}

      std::list<point>::const_iterator it3;
      for ( it3 = maximal.begin(); it3 != maximal.end(); ++it3 )
	{
	  unsigned int area = (it3->x - it->x - 1) * (it3->y - it->y - 1);
	  
	  if ( area > m_min_bound ) 
	    m_min_bound = area;
	}
    }
} // ler_base_problem::compute_min_bound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the largest empty rectangle in a horizontal line.
 * return true if a empty rectangle is found.
 * \param result The largest rectangle.
 */
bool bf::ler_base_problem::largest_rectangle_in_horizontal_line
( rectangle& result )
{  
  // m_forbidden_points.sort(ler_base_problem::point_compare_x_min_y_min());
  
  const_iterator it = first_point();
  unsigned int size = it->x - m_bounding_rectangle.left();
  unsigned int last = it->x;
  unsigned int left = m_bounding_rectangle.left();

  for ( ++it ; it != end_point(); ++it)
    {
      if ( it->x - last - 1 > size) 
	{
	  left = last + 1;
	  size = it->x - last - 1;
	}
      last = it->x;
    }
  
  if ( m_bounding_rectangle.right() - last > size )
    {
      left = last + 1;
      size = m_bounding_rectangle.right() - last;
    }
  
  if ( size != 0) 
    result = 
      rectangle(point(left,m_bounding_rectangle.bottom()),
		point(left+size-1,m_bounding_rectangle.bottom()));
  
  return (size != 0); 
} // ler_base_problem::largest_rectangle_in_hozizontal_line()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the largest empty rectangle in a vertical line.
 * return true if a empty rectangle is found.
 * \param result The largest rectangle.
 */
bool bf::ler_base_problem::largest_rectangle_in_vertical_line(rectangle& result)
{  
  // m_forbidden_points.sort(ler_base_problem::point_compare_y_min_x_min());
  
  const_iterator it = first_point();
  unsigned int size = it->y - m_bounding_rectangle.bottom();
  unsigned int last = it->y;
  unsigned int bottom = m_bounding_rectangle.bottom();

  for ( ++it ; it != end_point(); ++it )
    {
      if ( it->y - last - 1 > size) 
	{
	  bottom = last + 1;
	  size = it->y - last - 1;
	}
      last = it->y;
    }
  
  if ( m_bounding_rectangle.top() - last > size )
    {
      bottom = last + 1;
      size = m_bounding_rectangle.top() - last;
    }
  
  if ( size != 0) 
    result = 
      rectangle(point(m_bounding_rectangle.left(), bottom),
		point(m_bounding_rectangle.left(), bottom+size-1));
  
  return (size != 0); 
} // ler_base_problem::largest_rectangle_in_vertical_line()
