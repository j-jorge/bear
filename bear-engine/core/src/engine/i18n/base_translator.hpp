/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The base_translator class is the base class for the objects that
 *        manages a set of translations provided by the game's plugins.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_TRANSLATOR_HPP__
#define __ENGINE_BASE_TRANSLATOR_HPP__

#include "engine/class_export.hpp"
#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The base_translator class is the base class for the objects that
     *        manages a set of translations provided by the game's plugins.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT base_translator
    {
    public:
      virtual ~base_translator();

      virtual base_translator* clone() const = 0;
      virtual std::string get( std::string key ) = 0;

    }; // class base_translator
  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_TRANSLATOR_HPP__
