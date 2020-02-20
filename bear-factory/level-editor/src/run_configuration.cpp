/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::run_configuration class.
 * \author Julien Jorge
 */
#include "bf/run_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path to the program to run to test the level.
 */
const std::string& bf::run_configuration::get_program_path() const
{
  return m_program_path;
} // run_configuration::get_program_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the path to the program to run to test the level.
 * \param p The path.
 */
void bf::run_configuration::set_program_path( const std::string& p )
{
  m_program_path = p;
} // run_configuration::set_program_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an argument to pass to the program to run to test the level.
 * \param i The index of the argument on the command line.
 */
std::string bf::run_configuration::get_argument( std::size_t i ) const
{
  if ( i >= m_arguments.size() )
    return "";
  else
    {
      std::list<std::string>::const_iterator it( m_arguments.begin() );
      std::advance( it, i );
      return *it;
    }
} // run_configuration::get_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of arguments.
 */
std::size_t bf::run_configuration::get_arguments_count() const
{
  return m_arguments.size();
} // run_configuration::get_arguments_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an argument at the end of the command line.
 * \param arg The argument to add.
 */
void bf::run_configuration::append_argument( const std::string& arg )
{
  m_arguments.push_back( arg );
} // run_configuration::append_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an item to insert in the the level before running it.
 * \param i The index of the item.
 */
bf::item_instance bf::run_configuration::get_item( std::size_t i ) const
{
  CLAW_PRECOND( i < get_items_count() );

  std::list<item_instance>::const_iterator it( m_items.begin() );
  std::advance( it, i );
  return *it;
} // run_configuration::get_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of items.
 */
std::size_t bf::run_configuration::get_items_count() const
{
  return m_items.size();
} // run_configuration::get_items_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item to insert in the level before running it.
 * \param item The item to add.
 */
void bf::run_configuration::append_item( const bf::item_instance& item )
{
  m_items.push_back( item );
} // run_configuration::append_item()
