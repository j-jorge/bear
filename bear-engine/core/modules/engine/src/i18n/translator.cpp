/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::translator class.
 * \author Julien Jorge
 */
#include "bear/engine/i18n/translator.hpp"

#include "bear/engine/i18n/base_translator.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::translator::translator()
  : m_impl(NULL)
{

} // translator::translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy.
 */
bear::engine::translator::translator( const translator& that )
{
  if ( that.m_impl == NULL )
    m_impl = NULL;
  else
    m_impl = that.m_impl->clone();
} // translator::translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor from a given implementation.
 * \param impl The implementation.
 */
bear::engine::translator::translator( const base_translator& impl )
  : m_impl( impl.clone() )
{

} // translator::translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::translator::~translator()
{
  delete m_impl;
} // translator::~translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap this instance with another one.
 * \param that The instance to swap with.
 */
void bear::engine::translator::swap( translator& that )
{
  std::swap( m_impl, that.m_impl );
} // translator::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign another instance to this one.
 * \param that The instance to copy.
 */
bear::engine::translator&
bear::engine::translator::operator=( translator that )
{
  swap( that );
  return *this;
} // translator::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the translator of a given text.
 * \param key The text to translate.
 */
std::string bear::engine::translator::get( std::string key )
{
  if ( m_impl != NULL )
    return m_impl->get( key );
  else
    return key;
} // translator::get()




namespace std
{
  /**
   * \brief Swaps the value of two translators.
   * \param a One translator.
   * \param b Another translator.
   */
  void swap( bear::engine::translator& a, bear::engine::translator& b )
  {
    a.swap( b );
  } // swap()

} // namespace std
