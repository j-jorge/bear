/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the VISUAL_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __VISUAL_CLASS_EXPORT_HPP__
#define __VISUAL_CLASS_EXPORT_HPP__

#ifndef VISUAL_EXPORT
#  ifdef _WIN32
#    ifdef bear_visual_EXPORTS
#      define VISUAL_EXPORT __declspec(dllexport)
#    else
#      define VISUAL_EXPORT __declspec(dllimport)
#    endif // def bear_visual_EXPORTS
#  else // def _WIN32
#    define VISUAL_EXPORT
#  endif // def _WIN32
#endif // ndef VISUAL_EXPORT

#endif // __VISUAL_CLASS_EXPORT_HPP__
