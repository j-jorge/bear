/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The gettext_translator class is an implementation of the
 *        base_translator class using the gettext library.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GETTEXT_TRANSLATOR_HPP__
#define __ENGINE_GETTEXT_TRANSLATOR_HPP__

#include "bear/engine/i18n/base_translator.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The gettext_translator class is an implementation of the
     *        base_translator class using the gettext library.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT gettext_translator:
      public base_translator
    {
    public:
      gettext_translator( std::string domain_name );

      virtual gettext_translator* clone() const;
      virtual std::string get( std::string key );

    private:
      /** \brief The domain name passed to gettext to retrieve the
          translations. */
      const std::string m_domain_name;

    }; // class gettext_translator
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GETTEXT_TRANSLATOR_HPP__
