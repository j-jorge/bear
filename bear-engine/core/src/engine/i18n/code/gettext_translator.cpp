/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::gettext_translator class.
 * \author Julien Jorge
 */
#include "engine/i18n/gettext_translator.hpp"

#include <libintl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param domain_name The domain name passed to gettext to retrieve the
 *        translators.
 */
bear::engine::gettext_translator::gettext_translator
( std::string domain_name )
  : m_domain_name( domain_name )
{

} // gettext_translator::gettext_translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a dynamically allocated copy of this instance.
 */
bear::engine::gettext_translator*
bear::engine::gettext_translator::clone() const
{
  return new gettext_translator( *this );
} // gettext_translator::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the translator of a given text.
 * \param key The text to translate.
 */
std::string bear::engine::gettext_translator::get( std::string key )
{
  return dgettext( m_domain_name.c_str(), key.c_str() );
} // gettext_translator::get()
