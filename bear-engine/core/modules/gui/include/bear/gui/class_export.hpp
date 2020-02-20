/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the GUI_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __GUI_CLASS_EXPORT_HPP__
#define __GUI_CLASS_EXPORT_HPP__

#ifndef GUI_EXPORT
#  ifdef _WIN32
#    ifdef bear_gui_EXPORTS
#      define GUI_EXPORT __declspec(dllexport)
#    else
#      define GUI_EXPORT __declspec(dllimport)
#    endif // def bear_gui_EXPORTS
#  else // def _WIN32
#    define GUI_EXPORT
#  endif // def _WIN32
#endif // ndef GUI_EXPORT

#endif // __GUI_CLASS_EXPORT_HPP__
