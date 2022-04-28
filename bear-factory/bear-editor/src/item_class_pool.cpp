/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_class_pool class.
 * \author Julien Jorge
 */
#include "bf/item_class_pool.hpp"

#include "bf/class_not_found.hpp"
#include "bf/item_class_xml_parser.hpp"
#include "bf/path_configuration.hpp"
#include "bf/scan_dir.hpp"

#include <claw/assert.hpp>
#include <claw/logger/logger.hpp>
#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The map where we associate a file with its class.
 */
bf::item_class_pool::open_item_class_file::open_item_class_file
( std::map<std::string, std::string>& c )
  : class_files(c)
{
} // item_class_pool::open_item_class_file::open_item_class_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a class and its file.
 * \param path The path to the file describing the class.
 */
void bf::item_class_pool::open_item_class_file::operator()
  ( const std::string& path )
{
  try
    {
      const std::string class_name =
        item_class_xml_parser::get_item_class_name(path);

      if ( class_files.find(class_name) != class_files.end() )
        claw::logger << claw::log_error << "Duplicated item class '"
                     << class_name << "' in '" << path << '\'' << std::endl;
      else
        class_files[class_name] = path;
    }
  catch(std::exception& e)
    {
      claw::logger << claw::log_error << path << ": " << e.what() << std::endl;
    }
} // item_class_pool::open_item_class_file::operator()()


/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param w The name of workspace. 
 */
bf::item_class_pool::item_class_pool( const std::string& w )
{
  scan_directory( w );
} // item_class_pool::item_class_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::item_class_pool::~item_class_pool()
{
  item_class_map::iterator it;

  for ( it=m_item_class.begin(); it!=m_item_class.end(); ++it )
    delete it->second;
} // item_class_pool::~item_class_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have an item class with a given name.
 * \param class_name The name of the item class to check.
 */
bool bf::item_class_pool::has_item_class( const std::string& class_name ) const
{
  return m_item_class.find(class_name) != m_item_class.end();
} // item_class_pool::has_item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item class with a given name.
 * \param class_name The name of the item class we want.
 */
const bf::item_class&
bf::item_class_pool::get_item_class( const std::string& class_name ) const
{
  item_class_map::const_iterator it = m_item_class.find(class_name);

  if ( it == m_item_class.end() )
    throw class_not_found(class_name);
  else
    return *it->second;
} // item_class_pool::get_item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer to the item class with a given name.
 * \param class_name The name of the item class we want.
 */
const bf::item_class*
bf::item_class_pool::get_item_class_ptr( const std::string& class_name ) const
{
  item_class_map::const_iterator it = m_item_class.find(class_name);

  if ( it == m_item_class.end() )
    throw class_not_found(class_name);
  else
    return it->second;
} // item_class_pool::get_item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the begining of the pool.
 */
bf::item_class_pool::const_iterator bf::item_class_pool::begin() const
{
  return const_iterator( m_item_class.begin() );
} // item_class_pool::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator just pas the end of the pool.
 */
bf::item_class_pool::const_iterator bf::item_class_pool::end() const
{
  return const_iterator( m_item_class.end() );
} // item_class_pool::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read all item files from a given directory and in its subdirectories.
 * \param w The workspace used.
 */
void bf::item_class_pool::scan_directory( const std::string& w )
{
  std::vector<std::string> ext(1);
  ext[0] = ".xml";

  if ( path_configuration::get_instance().has_workspace( w ) )
    {
      workspace::path_list_const_iterator it;
      const workspace& work
        ( path_configuration::get_instance().get_workspace( w ) );
      
      for ( it = work.item_class_begin(); 
            it != work.item_class_end(); ++it )
        {
          std::map<std::string, std::string> files;
          
          open_item_class_file f(files);
          scan_dir<open_item_class_file> scan;
          
          scan( *it, f, ext.begin(), ext.end() );
          
          while ( !files.empty() )
            load_class( files.begin()->first, files );
        }
    }

  field_unicity_test();
} // item_class_pool::scan_directory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the file of an item class.
 * \param name The name of the class to read.
 * \param files The files associated to the item classes.
 */
void bf::item_class_pool::load_class
( const std::string& name, std::map<std::string, std::string>& files )
{
  std::list<std::string> pending;
  pending.push_front(name);

  while ( !pending.empty() )
    {
      const std::string class_name( pending.front() );
      item_class* item(NULL);

      try
        {
          item_class_xml_parser r;

          item = r.read( *this, files[class_name] );
          m_item_class[item->get_class_name()] = item;
          pending.pop_front();
          files.erase(class_name);
        }
      catch( class_not_found& e )
        {
          delete item;

          if ( std::find( pending.begin(), pending.end(), e.class_name() )
               != pending.end() )
            {
              claw::logger << claw::log_error << "Circular inheritance for '"
                           << e.class_name() << '\'' << std::endl;
              pending.pop_front();
              files.erase(class_name);
            }
          else if ( files.find( e.class_name() ) == files.end() )
            {
              claw::logger << claw::log_error << "Can't find class '"
                           << e.class_name() << "' for '" << class_name << '\''
                           << std::endl;
              pending.pop_front();
              files.erase(class_name);
            }
          else
            pending.push_front(e.class_name());
        }
      catch( std::exception& e )
        {
          claw::logger << claw::log_error << e.what() << std::endl;
          delete item;
          pending.pop_front();
          files.erase(class_name);
        }
    }
} // item_class_pool::load_classes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test, for each classe, the unicity of fields.
 */
void bf::item_class_pool::field_unicity_test()
{
  const_iterator it;
  std::set<std::string> not_valid_classes;

  for ( it=begin(); it!=end(); ++it )
    {
      std::string error_msg;
      if ( !it->field_unicity_test(error_msg) )
        {
          claw::logger << claw::log_warning << "Ignoring class '"
                       << it->get_class_name() << "' : " << error_msg
                       << std::endl;

          not_valid_classes.insert(it->get_class_name());
        }
    }

  std::set<std::string>::const_iterator it2;

  for ( it2=not_valid_classes.begin(); it2!=not_valid_classes.end(); ++it2 )
    {
      delete m_item_class[*it2];
      m_item_class.erase(*it2);
    }
} // item_class_pool::control_sprite_size()
