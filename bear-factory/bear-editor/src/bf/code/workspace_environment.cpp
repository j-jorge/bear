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

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::workspace_environment::workspace_environment()
  : m_name(""), m_image_pool("")
{
} // workspace_environment::workspace_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param n The name of the workspace.
 */
bf::workspace_environment::workspace_environment( const std::string& name )
  : m_image_pool(name)
{
  m_name = name;
} // workspace_environment::workspace_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the workspace.
 */
const std::string& bf::workspace_environment::get_name() const
{
  return m_name;
} // workspace_environment::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the image pool.
 */
const bf::image_pool& bf::workspace_environment::get_image_pool() const
{
  return m_image_pool;
} // workspace_environment::get_image_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item class pool.
 */
const bf::item_class_pool& 
bf::workspace_environment::get_item_class_pool() const
{
  return m_item_class_pool;
} // workspace_environment::get_item_class_pool()
