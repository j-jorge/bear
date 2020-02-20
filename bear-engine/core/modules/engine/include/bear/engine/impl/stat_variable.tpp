/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::stat_variable structure.
 * \author Julien Jorge
 */
#include "bear/engine/stat_variable.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The name of the variable.
 * \param v The value of the variable.
 */
template<typename ValueType>
bear::engine::stat_variable::stat_variable( std::string n, ValueType v )
{
  name = n;

  std::ostringstream oss;
  oss << v;
  value = oss.str();
} // stat_variable::stat_variable()
