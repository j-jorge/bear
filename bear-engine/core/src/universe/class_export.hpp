/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Definition of the UNIVERSE_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CLASS_EXPORT_HPP__
#define __UNIVERSE_CLASS_EXPORT_HPP__

#ifndef UNIVERSE_EXPORT
#  ifdef _WIN32
#    ifdef bear_universe_EXPORTS
#      define UNIVERSE_EXPORT __declspec(dllexport)
#    else
#      define UNIVERSE_EXPORT __declspec(dllimport)
#    endif // def bear_universe_EXPORTS
#  else // def _WIN32
#    define UNIVERSE_EXPORT
#  endif // def _WIN32
#endif // ndef UNIVERSE_EXPORT

#endif // __UNIVERSE_CLASS_EXPORT_HPP__
