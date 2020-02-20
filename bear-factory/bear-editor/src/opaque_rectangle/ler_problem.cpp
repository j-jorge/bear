/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the ler_problem class.
 * \author Sebastien Angibaud
 */
#include "bf/opaque_rectangle/ler_problem.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::ler_problem::ler_problem(unsigned int min_bound)
  : ler_base_problem(min_bound)
{
} // ler_problem::ler_problem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a forbidden point.
 * \param p The forbidden point to add.
 */
void bf::ler_problem::add_forbidden_point(const point& p)
{
  m_forbidden_points.push_back(p);
} // ler_problem::add_forbidden_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there exists forbidden points.
 */
bool bf::ler_problem::has_forbidden_points() const
{
  return ! m_forbidden_points.empty();
} // ler_problem::has_forbidden_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of forbidden points.
 */
unsigned int bf::ler_problem::get_nb_points() const
{
  return m_forbidden_points.size();
} // ler_problem::get_nb_points()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on first point.
 */
bf::ler_base_problem::const_iterator bf::ler_problem::first_point() const
{
  return m_forbidden_points.begin();
} // ler_problem::first_point()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the ending point.
 */
bf::ler_base_problem::const_iterator bf::ler_problem::end_point() const
{
  return m_forbidden_points.end();
} // ler_problem::end_point()
