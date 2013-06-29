/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the ENGINE_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CLASS_EXPORT_HPP__
#define __ENGINE_CLASS_EXPORT_HPP__

#ifndef ENGINE_EXPORT
#  ifdef _WIN32
#    ifdef bear_engine_EXPORTS
#      define ENGINE_EXPORT __declspec(dllexport)
#    else
#      define ENGINE_EXPORT __declspec(dllimport)
#    endif // def bear_engine_EXPORTS
#  else // def _WIN32
#    define ENGINE_EXPORT
#  endif // def _WIN32
#endif // ndef ENGINE_EXPORT

#endif // __ENGINE_CLASS_EXPORT_HPP__
