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
#include "bear/engine/resource_pool.hpp"

#include "bear/engine/resource_pool/base_resource_pool.hpp"
#include "bear/engine/resource_pool/directory_resource_pool.hpp"

#include <claw/exception.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the instance.
 */
bear::engine::resource_pool& bear::engine::resource_pool::get_instance()
{
  return super::get_instance();
} // resource_pool::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroy the instance.
 */
bear::engine::resource_pool::~resource_pool()
{
  for ( std::size_t i(0); i!=m_pool.size(); ++i )
    delete m_pool[i];
} // resource_pool::add_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a path in which to seek resources.
 * \param path The path to add.
 */
void bear::engine::resource_pool::add_path( const std::string& path )
{
  add_pool( new directory_resource_pool( path ) );
} // resource_pool::add_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a pool in which to seek resources.
 * \param pool The pool to add. The instance will keep the pointer and delete it
 *        in the destructor.
 */
void bear::engine::resource_pool::add_pool( base_resource_pool* pool )
{
  CLAW_PRECOND( pool != NULL );

  m_pool.insert( m_pool.begin(), pool );
} // resource_pool::add_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a file.
 * \param name The path of the file to get.
 * \param os Where we must write the content of the file.
 */
void bear::engine::resource_pool::get_file
( const std::string& name, std::ostream& os )
{
  for ( std::size_t i(0); i!=m_pool.size(); ++i )
    if ( m_pool[i]->exists( name ) )
      {
        m_pool[i]->get_file( name, os );
        return;
      }

  throw claw::exception( "Can't find file '" + name + "'" );
} // resource_pool::get_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Checks if we know a file with a given name.
 * \param name The name of the file to find.
 */
bool bear::engine::resource_pool::exists( const std::string& name ) const
{
  for ( std::size_t i(0); i!=m_pool.size(); ++i )
    if ( m_pool[i]->exists( name ) )
      return true;

  return false;
} // resource_pool::exists()
