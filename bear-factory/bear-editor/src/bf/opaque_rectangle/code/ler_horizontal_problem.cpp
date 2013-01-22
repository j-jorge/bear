/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the ler_horizontal_problem class.
 * \author Sebastien Angibaud
 */
#include "bf/opaque_rectangle/ler_horizontal_problem.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::ler_horizontal_problem::ler_horizontal_problem
( const_iterator first_point, const_iterator end_point, unsigned int min_bound )
  : ler_base_problem(min_bound), m_first_point(first_point), 
    m_end_point(end_point)
{
} // ler_horizontal_problem::ler_horizontal_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there exists forbidden points.
 */
bool bf::ler_horizontal_problem::has_forbidden_points() const
{
  return m_first_point != m_end_point;
} // ler_horizontal_problem::has_forbidden_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of forbidden points.
 */
unsigned int bf::ler_horizontal_problem::get_nb_points() const
{
  return std::distance(m_first_point, m_end_point);
} // ler_horizontal_problem::get_nb_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on first point.
 */
bf::ler_base_problem::const_iterator
bf::ler_horizontal_problem::first_point() const
{
  return m_first_point;
} // ler_horizontal_problem::first_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the ending point.
 */
bf::ler_base_problem::const_iterator
bf::ler_horizontal_problem::end_point() const
{
  return m_end_point;
} // ler_horizontal_problem::end_point()
