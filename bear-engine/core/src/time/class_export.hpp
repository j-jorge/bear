/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Definition of the TIME_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __TIME_CLASS_EXPORT_HPP__
#define __TIME_CLASS_EXPORT_HPP__

#ifndef TIME_EXPORT
#  ifdef _WIN32
#    ifdef bear_time_EXPORTS
#      define TIME_EXPORT __declspec(dllexport)
#    else
#      define TIME_EXPORT __declspec(dllimport)
#    endif // def bear_time_EXPORTS
#  else // def _WIN32
#    define TIME_EXPORT
#  endif // def _WIN32
#endif // ndef TIME_EXPORT

#endif // __TIME_CLASS_EXPORT_HPP__
