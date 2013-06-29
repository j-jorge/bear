/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::resource_pool class.
 * \author Julien Jorge
 */
#include "engine/resource_pool.hpp"

#include <fstream>
#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <claw/functional.hpp>
#include <claw/logger.hpp>
#include <algorithm>
#include <boost/filesystem/convenience.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the instance.
 */
bear::engine::resource_pool& bear::engine::resource_pool::get_instance()
{
  return super::get_instance();
} // resource_pool::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a path in which to seek resources.
 * \param path The path to add.
 */
void bear::engine::resource_pool::add_path( const std::string& path )
{
  m_path.push_front(path);
} // resource_pool::add_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a file.
 * \param name The path of the file to get.
 * \param os Where we must write the content of the file.
 */
void bear::engine::resource_pool::get_file
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

} // resource_pool::get_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if we know a file with a given name.
 * \param name The name of the file to find.
 */
bool bear::engine::resource_pool::exists( const std::string& name ) const
{
  bool result = false;

  std::ifstream f;

  if ( find_file(name, f) )
    {
      f.close();
      result = true;
    }

  return result;
} // resource_pool::exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a file in the paths.
 * \param name The name of the file to find.
 * \param f We will open the file with this variable.
 * \return True if we found the file (f is opened) ; false otherwise.
 */
bool bear::engine::resource_pool::find_file
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
} // resource_pool::find_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a file in the paths.
 * \param name The name of the file to find.
 * \return True if we found the file; false otherwise.
 */
bool bear::engine::resource_pool::find_file_name_straight
( std::string& name ) const
{
  std::list<std::string>::const_iterator it;
  bool result(false);

  for (it=m_path.begin(); (it!=m_path.end()) && !result; ++it)
    {
#if BOOST_VERSION / 100 % 1000 < 34
      const std::string filepath = *it + '/' + name;
#else
      const std::string filepath =
        *it + boost::filesystem::slash<boost::filesystem::path>::value + name;
#endif
      const boost::filesystem::path path( filepath, boost::filesystem::native );

      if ( boost::filesystem::exists( path ) )
        if ( !boost::filesystem::is_directory( path ) )
          {
            result = true;
            name = filepath;
          }
    }

  return result;
} // resource_pool::find_file_name_straight()
