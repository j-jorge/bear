/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The android_gettext_translator class is an implementation of the
 *        base_translator class using the gettext library.
 * \author Julien Jorge
 */
#ifdef __ANDROID__

#ifndef __ENGINE_ANDROID_GETTEXT_TRANSLATOR_HPP__
#define __ENGINE_ANDROID_GETTEXT_TRANSLATOR_HPP__

#include "bear/engine/i18n/base_translator.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The android_gettext_translator class is an implementation of the
     *        base_translator class using the gettext library.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT android_gettext_translator:
      public base_translator
    {
    public:
      android_gettext_translator( std::string domain_name );

      virtual android_gettext_translator* clone() const;
      virtual std::string get( std::string key );

    private:
      std::string get_mo_file_path( std::string language ) const;
      std::string make_translation_file_path( std::string language ) const;

      std::string get_user_locale() const;

      void read_mo_file( std::string mo_file_path ) const;

    private:
      /** \brief The domain name passed to gettext to retrieve the
          translations. */
      const std::string m_domain_name;

    }; // class android_gettext_translator
  } // namespace engine
} // namespace bear

#endif // __ENGINE_ANDROID_GETTEXT_TRANSLATOR_HPP__

#endif // __ANDROID__
