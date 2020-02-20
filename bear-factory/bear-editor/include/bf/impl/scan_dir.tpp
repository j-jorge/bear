/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::scan_dir class.
 * \author Julien Jorge
 */

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <queue>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read all item files from a given directory and in its subdirectories.
 * \param dir The path where the files are searched.
 * \param f A copyable function object called on each file found in the
 *        directory.
 * \param first_ext Iterator on the first valid extension.
 * \param last_ext Iterator just after the last valid extension.
 */
template<typename Func>
template<typename Iterator>
void bf::scan_dir<Func>::operator()
( const std::string& dir, Func& f, Iterator first_ext, Iterator last_ext )
{
  std::queue<boost::filesystem::path> pending;
  boost::filesystem::path path(dir);

  if ( !boost::filesystem::exists(path) )
    return;

  pending.push(path);

  while ( !pending.empty() )
    {
      path = pending.front();
      pending.pop();

      boost::filesystem::directory_iterator it(path);
      boost::filesystem::directory_iterator eit;

      for ( ; it!=eit; ++it)
        if ( boost::filesystem::is_directory(*it) )
          pending.push(*it);
        else
          {
            const std::string file_name( it->path().string() );

            if ( supported_extension( file_name, first_ext, last_ext ) )
              f( file_name );
          }
    }
} // scan_dir::operator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a path correspond to a supported extension.
 * \param path The path to check.
 * \param first_ext Iterator on the first valid extension.
 * \param last_ext Iterator just after the last valid extension.
 */
template<typename Func>
template<typename Iterator>
bool bf::scan_dir<Func>::supported_extension
( const std::string& path, Iterator first_ext, Iterator last_ext )
{
  // return true if no extension is given
  bool result( first_ext == last_ext );

  for ( ; !result && (first_ext!=last_ext); ++first_ext )
    if ( path.size() >= first_ext->size() )
      result = path.rfind(*first_ext) == path.size() - first_ext->size();

  return result;
} // scan_dir::supported_extension()
