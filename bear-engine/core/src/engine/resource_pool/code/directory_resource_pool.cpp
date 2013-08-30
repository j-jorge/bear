/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::directory_resource_pool class.
 * \author Julien Jorge
 */
#include "engine/resource_pool/directory_resource_pool.hpp"

#include <fstream>
#include <claw/exception.hpp>
#include <boost/filesystem/convenience.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the instance to search in a given director.
 * \param path The path where the resources are searched.
 */
bear::engine::directory_resource_pool::directory_resource_pool
( const std::string& path )
  : m_path( path )
{

} // directory_resource_pool::directory_resource_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a file.
 * \param name The path of the file to get.
 * \param os Where we must write the content of the file.
 */
void bear::engine::directory_resource_pool::get_file
( const std::string& name, std::ostream& os )
{
  std::ifstream f;

  if ( find_file(name, f) )
    {
      f >> os.rdbuf();
      f.close();
    }
  else
    throw claw::exception( "Can't find file '" + name + "'" );
} // directory_resource_pool::get_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Checks if we know a file with a given name.
 * \param name The name of the file to find.
 */
bool
bear::engine::directory_resource_pool::exists( const std::string& name ) const
{
  bool result = false;

  std::ifstream f;

  if ( find_file(name, f) )
    {
      f.close();
      result = true;
    }

  return result;
} // directory_resource_pool::exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds a file in the path.
 * \param name The name of the file to find.
 * \param f We will open the file with this variable.
 * \return True if we found the file (f is opened); false otherwise.
 */
bool bear::engine::directory_resource_pool::find_file
( const std::string& name, std::ifstream& f ) const
{
  std::string n(name);

  if ( find_file_name_straight(n) )
    {
      f.open( n.c_str(), std::ios::binary );
      return true;
    }
  else
    return false;
} // directory_resource_pool::find_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a file in the paths.
 * \param name The name of the file to find.
 * \return True if we found the file; false otherwise.
 */
bool bear::engine::directory_resource_pool::find_file_name_straight
( std::string& name ) const
{
  bool result(false);

  const boost::filesystem::path path( boost::filesystem::path(m_path) / name );

  if ( boost::filesystem::exists( path ) )
    if ( !boost::filesystem::is_directory( path ) )
      {
        result = true;
        name = path.string();
      }

  return result;
} // directory_resource_pool::find_file_name_straight()
