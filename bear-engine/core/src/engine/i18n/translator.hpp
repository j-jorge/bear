/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class manages a set of translations provided by the game's
 *        plugins.
 * \author Julien Jorge
 */
#ifndef __ENGINE_TRANSLATOR_HPP__
#define __ENGINE_TRANSLATOR_HPP__

#include "engine/class_export.hpp"

#include <string>

namespace bear
{
  namespace engine
  {
    class base_translator;

    /**
     * \brief This class manages a set of translations provided by the game's
     *        plugins.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT translator
    {
    public:
      translator();
      translator( const translator& that );
      translator( const base_translator& impl );
      ~translator();

      void swap( translator& that );
      translator& operator=( translator that );

      std::string get( std::string key );

    private:
      /** \brief The instance that effectively provides the translations. */
      base_translator* m_impl;

    }; // class translator
  } // namespace engine
} // namespace bear

namespace std
{
  void swap( bear::engine::translator& a, bear::engine::translator& b );
} // namespace std

#endif // __ENGINE_TRANSLATOR_HPP__
