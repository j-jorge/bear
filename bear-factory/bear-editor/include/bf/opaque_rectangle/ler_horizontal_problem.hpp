/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A largest empty rectangle problem.
 * \author Sebastien Angibaud
 */
#ifndef __BF_LER_HORIZONTAL_PROBLEM_HPP__
#define __BF_LER_HORIZONTAL_PROBLEM_HPP__

#include "bf/opaque_rectangle/ler_base_problem.hpp"

namespace bf
{
  /**
   * \brief A largest empty rectangle problem.
   *
   * \author Sebastien Angibaud
   */
  class ler_horizontal_problem
    : public ler_base_problem
  {
  public:
    ler_horizontal_problem
    ( const_iterator first_point, const_iterator end_point,
      unsigned int min_bound = 0 );
    
    bool has_forbidden_points() const;
    unsigned int get_nb_points() const;
    const_iterator first_point() const;
    const_iterator end_point() const;

  private:
    /** \brief The itertaor on first point. */
    const_iterator m_first_point;

    /** \brief The iterator just after the last point. */
    const_iterator m_end_point;

  }; // ler_horizontal_problem
} // namespace bf

#endif // __BF_LER_HORIZONTAL_PROBLEM_HPP__
