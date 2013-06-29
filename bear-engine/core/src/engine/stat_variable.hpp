/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A structure that stores a statistic variable.
 * \author Julien Jorge
 */
#ifndef __ENGINE_STAT_VARIABLE_HPP__
#define __ENGINE_STAT_VARIABLE_HPP__

#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A variable that can be sent to the server.
     */
    struct stat_variable
    {
      template<typename ValueType>
      stat_variable( std::string n, ValueType v );
      
      /** \brief The name of the variable. */
      std::string name;
      
      /** \brief The value of the variable. */
      std::string value;      
    }; // struct variable
  } // namespace engine
} // namespace bear

#include "engine/impl/stat_variable.tpp"

#endif // __ENGINE_STAT_VARIABLE_HPP__
