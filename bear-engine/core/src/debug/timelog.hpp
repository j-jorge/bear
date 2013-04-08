/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class provides some utility methods to log the time measures.
 * \author Julien Jorge
 */
#ifndef __DEBUG_TIMELOG_HPP__
#define __DEBUG_TIMELOG_HPP__

#include "debug/class_export.hpp"

#include <iostream>

namespace bear
{
  namespace debug
  {
    /**
     * \brief This class provides some utility methods to log the time measures.
     * \author Julien Jorge
     */
    class DEBUG_EXPORT timelog
    {
    public:
      static std::ostream& get_output();

      static std::string format_key( std::string key );

    private:
      /** \brief The stream in which the measures are written. */
      static std::ostream* s_output;

    }; // class timelog
  } // namespace debug
} // namespace bear

#endif // __DEBUG_TIMELOG_HPP__
