/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Definition of the DEBUG_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __DEBUG_CLASS_EXPORT_HPP__
#define __DEBUG_CLASS_EXPORT_HPP__

#ifndef DEBUG_EXPORT
#  ifdef _WIN32
#    ifdef bear_debug_EXPORTS
#      define DEBUG_EXPORT __declspec(dllexport)
#    else
#      define DEBUG_EXPORT __declspec(dllimport)
#    endif // def bear_debug_EXPORTS
#  else // def _WIN32
#    define DEBUG_EXPORT
#  endif // def _WIN32
#endif // ndef DEBUG_EXPORT

#endif // __DEBUG_CLASS_EXPORT_HPP__
