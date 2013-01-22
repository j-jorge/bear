/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::variable_eraser class.
 * \author Julien Jorge
 */
#include "engine/variable/variable_eraser.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The map from which the variable is erased.
 * \param pattern A regular expression that must be verified by the variable's
 *        name to be erased.
 */
bear::engine::variable_eraser::variable_eraser
( var_map& m, boost::regex pattern )
  : m_map(m), m_pattern(pattern)
{

} // variable_eraser::variable_eraser()
