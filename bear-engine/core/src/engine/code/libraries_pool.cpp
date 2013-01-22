/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::libraries_pool class.
 * \author Julien Jorge
 */
#include <algorithm>
#include "engine/libraries_pool.hpp"

#include <claw/functional.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::libraries_pool::~libraries_pool()
{
  std::for_each( m_libraries.begin(), m_libraries.end(),
     claw::delete_function<claw::dynamic_library*>() );
} // libraries_pool::~libraries_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a library in the pool.
 * \param name The name of the library.
 * \param current_program Tell if \a name is the current program.
 */
void bear::engine::libraries_pool::add_library
( const std::string& name, bool current_program )
{
  m_libraries.push_front( new claw::dynamic_library(name, current_program) );
} // libraries_pool::add_library()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if one of the libraries has a given symbol.
 * \param symbol The symbol to find.
 */
bool
bear::engine::libraries_pool::have_symbol( const std::string& symbol ) const
{
  bool result=false;
  libraries_list::const_iterator it;

  for (it=m_libraries.begin(); !result && (it!=m_libraries.end()); ++it)
    result = (*it)->have_symbol( symbol );

  return result;
} // libraries_pool::have_symbol()
