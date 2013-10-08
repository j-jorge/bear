/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the workspace_environment class.
 * \author Sebastien Angibaud
 */
#include "bf/workspace_environment.hpp"

#include "bf/image_pool.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::workspace_environment::workspace_environment()
  : name(""), pool("")
{
} // workspace_environment::workspace_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The name of the workspace.
 */
bf::workspace_environment::workspace_environment( const std::string& n )
  : pool(n)
{
  name = n;
} // workspace_environment::workspace_environment()
