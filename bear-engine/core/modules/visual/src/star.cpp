/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::star class.
 * \author Julien Jorge
 */
#include "bear/visual/star.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param branches The number of branches in the star.
 * \param inside_ratio The ratio of the distance to the center of the inner
 *        vertices relatively to the distance of the outer ones.
 */
bear::visual::star::star( std::size_t branches, double inner_ratio )
{
  branches = std::max( (std::size_t)3, branches );
  inner_ratio = std::min( (double)1, std::max((double)0, inner_ratio) );

  compute_coordinates( branches, inner_ratio );
} // star::star()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ratio of the distance to the center of the inner vertices
 *        relatively to the distance of the outer ones.
 */
double bear::visual::star::get_ratio() const
{
  CLAW_PRECOND( m_coordinates.size() > 2 );

  return m_coordinates[1].distance( position_type(0, 0) );
} // star::get_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the ratio of the distance to the center of the inner vertices
 *        relatively to the distance of the outer ones.
 * \param r The ratio.
 */
void bear::visual::star::set_ratio( double r )
{
  compute_coordinates( get_branches(), std::max(0., std::min(1., r)) );
} // star::set_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of branches.
 */
std::size_t bear::visual::star::get_branches() const
{
  CLAW_PRECOND( m_coordinates.size() > 2 );
  CLAW_PRECOND( m_coordinates.size() % 2 == 0 );

  return m_coordinates.size() / 2;
} // star::get_branches()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the number of branches.
 * \param b The number of branches.
 */
void bear::visual::star::set_branches( std::size_t b )
{
  CLAW_PRECOND( b > 2 );

  compute_coordinates( b, get_ratio() );
} // star::set_branches()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the vertices.
 */
const std::vector<bear::visual::position_type>&
bear::visual::star::get_coordinates() const
{
  return m_coordinates;
} // star::get_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the coordinates of the branches.
 * \param branches The number of branches in the star.
 * \param inside_ratio The ratio of the distance to the center of the inner
 *        vertices relatively to the distance of the outer ones.
 */
void bear::visual::star::compute_coordinates
( std::size_t branches, double inside_ratio )
{
  CLAW_PRECOND( branches > 0 );

  const std::size_t count( 2 * branches );
  double a = 3.14159 / 2;
  const double a_step = 2 * 3.14159 / count;

  m_coordinates.resize( count );

  // outer vertices
  for ( std::size_t i=0; i!=branches; ++i )
    {
      const double angle = a + 2 * i * a_step;
      m_coordinates[2*i] = position_type( std::cos(angle), std::sin(angle) );
    }

  // inner vertices
  for ( std::size_t i=0; i!=branches; ++i )
    {
      const double angle = (a + (2 * i + 1) * a_step);
      m_coordinates[2*i+1] =
        inside_ratio * position_type( std::cos(angle), std::sin(angle) );
    }
} // star::compute_coordinates()
