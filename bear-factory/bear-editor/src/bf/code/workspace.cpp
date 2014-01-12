/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::workspace class.
 * \author Sebastien Angibaud
 */
#include "bf/workspace.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::workspace::workspace()
{

} // workspace::workspace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the run path.
 * \param path The new run path.
 */
void bf::workspace::set_run_path(const std::string& path)
{
  m_run_path = path;
} // workspace::set_run_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the run path.
 */
std::string bf::workspace::get_run_path() const
{
  return m_run_path;
} // workspace::get_run_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a path on item class.
 * \param path The path to add.
 */
void bf::workspace::add_item_class_path( const std::string& path )
{
  m_item_class_path.push_back(path);
} // workspace::add_item_class_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear paths on item class.
 */
void bf::workspace::clear_item_class_path()
{
  m_item_class_path.clear();
} // workspace::clear_item_class_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a pth on data.
 * \param path The path to add.
 */
void bf::workspace::add_data_path( const std::string& path )
{
  m_data_path.push_back(path);
} // workspace::add_data_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear paths on data.
 */
void bf::workspace::clear_data_path()
{
  m_data_path.clear();
} // workspace::clear_data_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the workspace.
 */
void bf::workspace::clear()
{
  m_item_class_path.clear();
  m_data_path.clear();
} // workspace::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item class path.
 */
const bf::workspace::path_list& bf::workspace::get_item_class_path() const
{
  return m_item_class_path;
} // workspace::get_item_class_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the begin of item class list.
 */
bf::workspace::path_list_const_iterator bf::workspace::item_class_begin() const
{
  return m_item_class_path.begin();
} // workspace::item_class_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the begin of item class list.
 */
bf::workspace::path_list_const_iterator bf::workspace::item_class_end() const
{
  return m_item_class_path.end();
} // workspace::item_class_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the begin of item class list.
 */
bf::workspace::path_list_const_iterator bf::workspace::data_begin() const
{
  return m_data_path.begin();
} // workspace::data_begin() 

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the begin of item class list.
 */
bf::workspace::path_list_const_iterator bf::workspace::data_end() const
{
  return m_data_path.end();
} // workspace::data_end() 

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the begin of item class list (reverse iterator).
 */
bf::workspace::path_list_const_reverse_iterator 
bf::workspace::data_rbegin() const
{
  return m_data_path.rbegin();
} // workspace::data_rbegin() 

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the begin of item class list (reverse iterator).
 */
bf::workspace::path_list_const_reverse_iterator bf::workspace::data_rend() const
{
  return m_data_path.rend();
} // workspace::data_rend() 
