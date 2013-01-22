/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::libraries_pool class.
 * \author Julien Jorge
 */
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a symbol from the libraries.
 * \param name The symbol to get.
 * \pre have_symbol( name )
 */
template<class T>
T bear::engine::libraries_pool::get_symbol( const std::string& name ) const
{
  CLAW_PRECOND( have_symbol(name) );

  bool found=false;
  libraries_list::const_iterator it = m_libraries.begin();

  while ( !found )
    if ( (*it)->have_symbol(name) )
      found = true;
    else
      ++it;

  return (*it)->get_symbol<T>( name );
} // libraries_pool::get_symbol()
