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
#include "bf/opaque_rectangle/test.hpp"

#include "bf/opaque_rectangle/ler_problem.hpp"
#include "bf/opaque_rectangle/ler_solver.hpp"

#include <iostream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::test::test( )
{
  std::cout << "problem creation" << std::endl;
  ler_problem pb;
  ler_problem::rectangle r
    ( ler_problem::point(0, 0), ler_problem::point(6, 8) );
  
  pb.set_bounding_rectangle(r);

  // DEFAULT TEST
  // no forbidden point

  // TEST 2
  /*pb.add_forbidden_point(point(0,7));
  pb.add_forbidden_point(point(1,2));
  pb.add_forbidden_point(point(1,4));
  pb.add_forbidden_point(point(2,0));
  pb.add_forbidden_point(point(2,6));
  pb.add_forbidden_point(point(3,6));
  pb.add_forbidden_point(point(4,1));
  pb.add_forbidden_point(point(4,8));
  pb.add_forbidden_point(point(5,3));
  pb.add_forbidden_point(point(5,5));
  pb.add_forbidden_point(point(5,6));
  pb.add_forbidden_point(point(6,4));
  */
  // TEST 2
  // All forbidden point
  /*
    for ( unsigned int i = 0; i != 7; i++ )
    for ( unsigned int j = 0; j != 9; j++ )
    pb.add_forbidden_point(point(i,j));
  */

  std::cout << "solver creation" << std::endl;
  ler_solver solver(pb);

  std::cout << "resolution" << std::endl;
  solver.resolve();

  std::cout << "result" << std::endl;
  if ( solver.is_solved() )
    {
      ler_problem::rectangle solution = solver.get_solution();

      std::cout << "solution : " 
		<< solution.left() << " " << solution.bottom() << " " 
		<< solution.right() << " " << solution.top() << std::endl;
    }
  else
    std::cout << "No solution" << std::endl;
} // test::test()
