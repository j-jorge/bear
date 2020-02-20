/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the INPUT_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __INPUT_CLASS_EXPORT_HPP__
#define __INPUT_CLASS_EXPORT_HPP__

#ifndef INPUT_EXPORT
#  ifdef _WIN32
#    ifdef bear_input_EXPORTS
#      define INPUT_EXPORT __declspec(dllexport)
#    else
#      define INPUT_EXPORT __declspec(dllimport)
#    endif // def bear_input_EXPORTS
#  else // def _WIN32
#    define INPUT_EXPORT
#  endif // def _WIN32
#endif // ndef INPUT_EXPORT

#endif // __INPUT_CLASS_EXPORT_HPP__
