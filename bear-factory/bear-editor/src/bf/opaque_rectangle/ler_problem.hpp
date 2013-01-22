/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A largest empty rectangle problem.
 * \author Sebastien Angibaud
 */
#ifndef __BF_LER_PROBLEM_HPP__
#define __BF_LER_PROBLEM_HPP__

#include "bf/opaque_rectangle/ler_base_problem.hpp"

namespace bf
{
  /**
   * \brief A largest empty rectangle problem.
   *
   * \author Sebastien Angibaud
   */
  class ler_problem
    : public ler_base_problem
  {
  public:
    ler_problem(unsigned int min_bound = 0);
    
    void add_forbidden_point(const point& p);
    
    bool has_forbidden_points() const;
    unsigned int get_nb_points() const;
    const_iterator first_point() const;
    const_iterator end_point() const;

  private:
    /** \brief The list of forbidden points. */
    std::list<point> m_forbidden_points;

  }; // ler_problem
} // namespace bf

#endif // __BF_LER_PROBLEM_HPP__
