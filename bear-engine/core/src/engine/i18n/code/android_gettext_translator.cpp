/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::android_gettext_translator class.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#include "engine/i18n/android_gettext_translator.hpp"

#include "engine/resource_pool.hpp"

#include <libintl.h>
#include <stdioext.h>
#include <sstream>

#include <SDL/SDL_system.h>

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param domain_name The domain name passed to gettext to retrieve the
 *        translators.
 */
bear::engine::android_gettext_translator::android_gettext_translator
( std::string domain_name )
  : m_domain_name( domain_name )
{
  std::string user_locale( get_user_locale() );

  if ( user_locale.empty() )
    return;

  const std::string mo_file_path( get_mo_file_path( user_locale ) );

  if ( mo_file_path.empty() )
    return;

  read_mo_file( mo_file_path );
} // android_gettext_translator::android_gettext_translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a dynamically allocated copy of this instance.
 */
bear::engine::android_gettext_translator*
bear::engine::android_gettext_translator::clone() const
{
  return new android_gettext_translator( *this );
} // android_gettext_translator::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the translator of a given text.
 * \param key The text to translate.
 */
std::string bear::engine::android_gettext_translator::get( std::string key )
{
  return dgettext( m_domain_name.c_str(), key.c_str() );
} // android_gettext_translator::get()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the path to the best translation file for the current domain.
 * \param language The languae code for which we search the translations.
 */
std::string bear::engine::android_gettext_translator::get_mo_file_path
( std::string language ) const
{
  std::string path( make_translation_file_path(language) );

  if ( resource_pool::get_instance().exists( path ) )
    return path;

  const std::string separators( "#._" );
  
  for ( std::string::const_iterator next_separator(separators.begin());
        next_separator != separators.end(); ++next_separator )
    {
      const std::size_t separator_position
        ( language.find_last_of( *next_separator ) );

      if ( separator_position != std::string::npos )
        {
          language = language.substr(0, separator_position);

          path = make_translation_file_path(language);

          if ( resource_pool::get_instance().exists( path ) )
            return path;
        }
    }

  claw::logger << claw::log_warning << "Can't find translations in domain '"
               << m_domain_name << "'." << std::endl;

  return std::string();
} // android_gettext_translator::get_mo_file_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Buids the path to the translation file for a given language in the
 *        current domain.
 * \param language The languae code for which we search the translations.
 */
std::string bear::engine::android_gettext_translator::make_translation_file_path
( std::string language ) const
{
  return "share/locale/" + language + "/LC_MESSAGES/" + m_domain_name + ".mo";
} // android_gettext_translator::make_translation_file_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds the user's locale name.
 */
std::string bear::engine::android_gettext_translator::get_user_locale() const
{
  const char* user_locale( SDL_AndroidGetLocale() );

  if ( user_locale == NULL )
    {
      claw::logger << claw::log_warning << "Can't get user's locale."
                   << std::endl;
      return std::string();
    }
  else
    {
      claw::logger << claw::log_verbose << "User's locale is '" << user_locale
                   << "'." << std::endl;

      return user_locale;
    }
} // android_gettext_translator::get_user_locale()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads the translations from a given .mo file.
 * \param mo_file_path The path to the file, relatively to the root of the
 *        resource pool.
 */
void bear::engine::android_gettext_translator::read_mo_file
( std::string mo_file_path ) const
{
  // get the content of the translation file
  std::stringstream mo_file_stream;
  resource_pool::get_instance().get_file( mo_file_path, mo_file_stream );

  // compute the size of the file
  mo_file_stream.seekg( 0, std::ios::end );
  const std::size_t mo_file_content_length( mo_file_stream.tellg() );
  mo_file_stream.seekg( 0, std::ios::beg );

  // read the file in a buffer so we can access it as a FILE using fmemopen
  char* const mo_file_content( new char[ mo_file_content_length ] );
  mo_file_stream.read( mo_file_content, mo_file_content_length );

  FILE* const mo_file
    ( fmemopen( mo_file_content, mo_file_content_length, "rb" ) );

  if ( mo_file != NULL )
    {
      // and finally pass the file to the internationalization library.
      loadMessageCatalogFile( m_domain_name.c_str(), mo_file );
      fclose( mo_file );
    }

  delete[] mo_file_content;
} // android_gettext_translator::android_gettext_translator()

#endif // __ANDROID__
