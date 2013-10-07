/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::path_configuration class.
 * \author Julien Jorge
 */
#include "bf/path_configuration.hpp"

#include <claw/assert.hpp>
#include <claw/glob.hpp>
#include <claw/system_info.hpp>
#include <claw/configuration_file.hpp>

#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <sstream>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The pattern used for the search.
 * \param n The maximum number of results asked.
 * \param c The files found in the data directory.
 */
bf::path_configuration::random_file_result::random_file_result
( const std::string p, std::size_t n, const std::list<std::string>& c )
  : pattern(p), count(n), candidates(c)
{

} // path_configuration::random_file_result::random_file_result()




/*----------------------------------------------------------------------------*/
const std::string bf::path_configuration::s_config_file_name = "config";
const std::string bf::path_configuration::s_config_directory = ".bear_factory";

const char bf::path_configuration::s_section_left = '[';
const char bf::path_configuration::s_section_right = ']';
const char bf::path_configuration::s_comment = '#';
const char bf::path_configuration::s_field_assign = '=';
const std::string
bf::path_configuration::s_items_directory_field = "items_directory";
const std::string
bf::path_configuration::s_data_directory_field = "data_directory";

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::path_configuration::path_configuration()
  : m_max_cached_files(24) // arbitrary number
{
  load();
} // path_configuration::configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the configuration.
 */
void bf::path_configuration::save() const
{
  if ( create_config_file() )
    {
      const std::string path( get_config_directory() + s_config_file_name );
      std::ofstream f( path.c_str() );

      if (f)
        {
          f << s_comment
            << " Path to the directory containing XML item class files\n";

          workspaces_const_iterator it_map;
          
          for ( it_map = m_workspaces.begin(); 
                it_map != m_workspaces.end(); ++it_map )
            {
              f << s_section_left << it_map->first << s_section_right << '\n';
              std::list<std::string>::const_iterator it;

              for ( it = it_map->second.item_class_begin(); 
                    it != it_map->second.item_class_end(); ++it )
                f << s_items_directory_field << ' ' << s_field_assign 
                  << ' ' << *it << '\n';

              f << '\n' << s_comment
                << " Path to the directory containing the data of the game\n";
              
              for ( it = it_map->second.data_begin(); 
                    it != it_map->second.data_end(); ++it )
                f << s_data_directory_field << ' ' << s_field_assign 
                  << ' ' << *it << '\n';
            }
        }
    }
} // path_configuration::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path to the directory where are saved the configuration files.
 */
std::string bf::path_configuration::get_config_directory() const
{
  return
    claw::system_info::get_user_directory() + '/' + s_config_directory + '/';
} // path_configuration::get_config_directory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a path relative to a data path into a full path.
 * \param p (in) The relative path. (out) The full path.
 * \param w The workspace used.
 *
 * \a p is unchanged if no data path is found to make it a full path.
 */
bool bf::path_configuration::get_full_path
( std::string& p, const std::string& w ) const
{
  boost::filesystem::path path( p );
  bool result = boost::filesystem::exists( path );
  workspace::path_list_const_reverse_iterator it;
  workspaces_const_iterator it_map;

  it_map = m_workspaces.find( w );
  if ( it_map != m_workspaces.end() )
    for ( it = it_map->second.data_rbegin(); 
          !result && (it != it_map->second.data_rend()); ++it)
      {
        path = *it;
        path /= p;
        
        if ( boost::filesystem::exists( path ) )
          {
            result = true;
            p = path.string();
          }
      }

  return result;
} // path_configuration::get_full_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the full path to a file. Find a random file if the parameter is a
 *        file pattern.
 * \param p (in) The relative path/pattern. (out) The full path.
 * \param w The workspace used.
 *
 * \a p is unchanged if no data path is found to make it a full path.
 */
bool bf::path_configuration::expand_file_name
( std::string& p, const std::string& w ) const
{
  return expand_file_name(p, std::numeric_limits<std::size_t>::max(), w);
} // path_configuration::expand_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the full path to a file. Find a random file if the parameter is a
 *        file pattern.
 * \param p (in) The relative path/pattern. (out) The full path.
 * \param m The maximum number of files to find.
 * \param w The workspace used.
 *
 * \a p is unchanged if no data path is found to make it a full path.
 */
bool
bf::path_configuration::expand_file_name
( std::string& p, std::size_t m, const std::string& w ) const
{
  if ( p.empty() )
    return false;
  else if ( p.find_first_of("#?*") == std::string::npos )
    return get_full_path(p,w);
  else
    return find_random_file_name(p, m, w);
} // path_configuration::expand_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a complete path to a path relative to a data path.
 * \param p (in) The full path. (out) The relative path.
 * \param w The workspace used.
 *
 * \a p is unchanged if no data path is found to make it a relative path.
 */
bool bf::path_configuration::get_relative_path
( std::string& p, const std::string& w ) const
{
  boost::filesystem::path path( p );
  bool result = false;
  workspace::path_list_const_reverse_iterator it;
  workspaces_const_iterator it_map;

  it_map = m_workspaces.find(w);
  if ( it_map != m_workspaces.end() )
    for ( it = it_map->second.data_rbegin(); 
          !result && (it!=it_map->second.data_rend()); ++it)
      {
        bool stop(false);
        boost::filesystem::path data( *it );
        boost::filesystem::path::iterator pit = path.begin();
        boost::filesystem::path::iterator dit = data.begin();
        
        while ( !stop && (pit!=path.end()) && (dit!=data.end()) )
          if ( *pit != *dit )
            stop = true;
          else
            {
              ++pit;
              ++dit;
            }
        
        if ( dit == data.end() )
          {
            result = true;
            p = pit->string();
            
            for ( ++pit; pit!=path.end(); ++pit )
              p = (boost::filesystem::path(p) / *pit).string();
          }
      }

  return result;
} // path_configuration::get_relative_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set workspaces.
 * \param w The workspaces.
 */
void bf::path_configuration::set_workspaces(const workspaces_map& w)
{
  m_workspaces = w;
} // path_configuration::set_workspaces

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given workspace name exists.
 * \param w The workspace searched.
 */
bool bf::path_configuration::has_workspace( const std::string& w ) const
{
  return m_workspaces.find(w) != m_workspaces.end();
} // path_configuration::has_workspace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a workspace by its name.
 * \param w The workspace to give.
 */
const bf::workspace&
bf::path_configuration::get_workspace( const std::string& w ) const
{
  CLAW_ASSERT( has_workspace( w ), "Workspace is required" );
  
  return m_workspaces.find(w)->second;
} // path_configuration::get_workspace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a workspace by its name.
 * \param w The workspace to give.
 */
const bf::path_configuration::workspaces_map& 
bf::path_configuration::get_workspaces() const
{
  return m_workspaces;
} // path_configuration::get_workspaces()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search the workspace according to a file.
 * \param path The path of the file.
 */
std::string 
bf::path_configuration::search_workspace( const std::string& path ) const
{
  std::string result("");

  workspace::path_list_const_iterator it;
  workspaces_const_iterator it_map;
  
  for ( it_map = m_workspaces.begin();
        it_map != m_workspaces.end() && result.empty(); ++it_map )
    for ( it = it_map->second.data_begin(); 
          it != it_map->second.data_end() && result.empty(); ++it )
      {
        boost::filesystem::path p_anim( boost::filesystem::canonical(path) );
        boost::filesystem::path p_data( boost::filesystem::canonical(*it) );
        
        if ( p_anim.string().find( p_data.string() ) == 0 )
          result = it_map->first;
      }

  return result;
} // path_configuration::search_workspace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the configuration.
 */
void bf::path_configuration::load()
{
  if ( create_config_file() )
    {
      const std::string path( get_config_directory() + s_config_file_name );
      std::ifstream f( path.c_str() );

      if (f)
        {
          claw::configuration_file config(f);

          claw::configuration_file::const_file_iterator it_file;
          for ( it_file = config.file_begin(); 
                it_file != config.file_end(); ++it_file )
            {
              m_workspaces[*it_file].clear();

              claw::configuration_file::const_field_iterator it;
              
              for ( it = config.field_begin(*it_file, s_items_directory_field);
                    it != config.field_end(*it_file, s_items_directory_field); 
                    ++it)
                m_workspaces[*it_file].add_item_class_path( *it );
              
              for ( it = config.field_begin(*it_file, s_data_directory_field);
                    it != config.field_end(*it_file, s_data_directory_field); 
                    ++it)
                m_workspaces[*it_file].add_data_path( *it );
            }
        }
    }
} // path_configuration::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the directory containing the configuration file, if it does not
 *        exists.
 * \return true if the directory already exists or if it has been created.
 */
bool bf::path_configuration::create_config_directory() const
{
  bool result = false;

  boost::filesystem::path path( get_config_directory() );

  if ( boost::filesystem::exists( path ) )
    result = boost::filesystem::is_directory( path );
  else
    result = boost::filesystem::create_directory( path );

  return result;
} // path_configuration::create_config_directory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the configuration file, if it does not exists.
 * \return true if the file already exists or if it has been created.
 */
bool bf::path_configuration::create_config_file() const
{
  bool result = false;

  if ( create_config_directory() )
    {
      boost::filesystem::path path
        ( get_config_directory() + s_config_file_name );

      if ( !boost::filesystem::exists( path ) )
        {
          std::ofstream f( path.string().c_str() );
          f << s_comment << " Configuration file for Bear Factory\n\n";
        }

      if ( boost::filesystem::exists( path ) )
        result = !boost::filesystem::is_directory( path );
    }

  return result;
} // path_configuration::create_config_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a random file matching a pattern name.
 * \param name The pattern of the file to find.
 * \param m The maximum number of files to find.
 * \return True if we found a file; false otherwise.
 */
bool bf::path_configuration::find_random_file_name
( std::string& name, std::size_t m, const std::string& w ) const
{
  bool result = find_cached_random_file_name(name, m);

  if ( !result )
    result = find_random_file_name_on_disk(name, m, w);

  return result;
} // path_configuration::find_random_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a random file matching a pattern name. The file is searched in
 *        the cache of the previous searches.
 * \param name The pattern of the file to find.
 * \param m The maximum number of files to find.
 * \return True if we found a file; false otherwise.
 */
bool bf::path_configuration::find_cached_random_file_name
( std::string& name, std::size_t m ) const
{
  cached_random_file_list_type::iterator it;
  bool result(false);
  bool stop(false);

  it = m_cached_random_file.begin();

  while ( !result && !stop && (it!=m_cached_random_file.end()) )
    if ( it->pattern == name )
      {
        stop = true;
        if ( it->count >= m )
          result = true;
      }
    else
      ++it;

  if ( result )
    {
      const std::size_t i
        ( (double)it->candidates.size() * rand() / (RAND_MAX+1.0) );

      std::list<std::string>::const_iterator itc = it->candidates.begin();
      std::advance(itc, i);

      name = *itc;

      // put the result at the beginning of the cache
      m_cached_random_file.push_front(*it);
      m_cached_random_file.erase(it);
    }
  else if ( stop )
    m_cached_random_file.erase(it);

  return result;
} // path_configuration::find_cached_random_file_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find a random file matching a pattern name.
 * \param name The pattern of the file to find.
 * \param m The maximum number of files to find.
 * \return True if we found a file; false otherwise.
 */
bool bf::path_configuration::find_random_file_name_on_disk
( std::string& name, std::size_t m, const std::string& w ) const
{
  workspaces_const_iterator it_map;
  workspace::path_list_const_iterator it;
  std::list<std::string> candidates;
  bool result(false);

  it_map = m_workspaces.find( w );
  if ( it_map != m_workspaces.end() )
    for ( it = it_map->second.data_begin(); 
          (it != it_map->second.data_end()) && (candidates.size() < m);
          ++it )
      {
        const boost::filesystem::path dirpath( *it );
        
        if ( boost::filesystem::exists( dirpath ) )
          if ( boost::filesystem::is_directory( dirpath ) )
            // plus 1 for the trailing slash of the root directory
            find_all_files_in_dir
              (*it, name, it->length() + 1, m, candidates);
      }

  if ( !candidates.empty() )
    {
      const std::size_t i
        ( (double)candidates.size() * rand() / (RAND_MAX+1.0) );

      it = candidates.begin();
      std::advance(it, i);

      std::string pattern(name);
      name = *it;
      result = true;

      // put the result at the beginning of the cache
      m_cached_random_file.push_front
        ( random_file_result(pattern, m, candidates) );

      if ( m_cached_random_file.size() > m_max_cached_files )
        m_cached_random_file.pop_back();
    }

  return result;
} // path_configuration::find_random_file_name_on_disk()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find all files matching a given pattern.
 * \param dirname The name of the directory to explore.
 * \param pattern The pattern of the name of the files to find.
 * \param offset The length of the path to the root directory.
 * \param m The maximum number of files to find.
 * \param result (out) The paths of the files.
 *
 * \sa glob_match
 */
void bf::path_configuration::find_all_files_in_dir
( const std::string& dirname, const std::string& pattern, std::size_t offset,
  std::size_t m, std::list<std::string>& result ) const
{
  const boost::filesystem::path path( dirname );

  CLAW_PRECOND( boost::filesystem::is_directory(path) );

  boost::filesystem::directory_iterator it(path);
  const boost::filesystem::directory_iterator eit;

  for ( ; (it!=eit) && (result.size() < m); ++it )
    {
      const std::string entry_path( it->path().string() );

      if ( boost::filesystem::is_directory(*it) )
        {
          if ( glob_potential_match(pattern, entry_path, offset) )
            find_all_files_in_dir(entry_path, pattern, offset, m, result);
        }
      else if ( glob_match(pattern, entry_path, offset) )
        result.push_back( entry_path );
    }
} // path_configuration::find_all_files_in_dir()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a string matches a pattern.
 * \param pattern The pattern.
 * \param text The text to check.
 * \param offset Position in \a text where the search starts.
 *
 * The characters '*', '?' and '#' in the pattern mean, respectively, anything,
 * a single character or nothing, and a single character.
 */
bool bf::path_configuration::glob_match
( const std::string& pattern, const std::string& text,
  std::size_t offset ) const
{
  return claw::glob_match
    ( pattern.begin(), pattern.end(), text.begin() + offset, text.end(),
      '*', '?', '#' );
} // path_configuration::glob_match()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a string can potentially match a pattern.
 * \param pattern The pattern.
 * \param text The text to check.
 * \param offset Position in \a text where the search starts.
 *
 * The characters '*', '?' and '#' in the pattern mean, respectively, anything,
 * a single character or nothing, and a single character.
 */
bool bf::path_configuration::glob_potential_match
( const std::string& pattern, const std::string& text,
  std::size_t offset ) const
{
  return claw::glob_potential_match
    ( pattern.begin(), pattern.end(), text.begin() + offset, text.end(),
      '*', '?', '#' );
} // path_configuration::glob_potential_match()
