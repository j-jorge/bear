/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Definition of the BEAR_EDITOR_EXPORT macro according to the fact that
 *        we create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __BF_LIBEDITOR_EXPORT_HPP__
#define __BF_LIBEDITOR_EXPORT_HPP__

#ifndef BEAR_EDITOR_EXPORT
#  ifdef _WIN32
#    ifdef bear_editor_EXPORTS
#      define BEAR_EDITOR_EXPORT __declspec(dllexport)
#    else
#      define BEAR_EDITOR_EXPORT __declspec(dllimport)
#    endif // def bear_editor_EXPORTS
#  else // def _WIN32
#    define BEAR_EDITOR_EXPORT
#  endif // def _WIN32
#endif // ndef BEAR_EDITOR_EXPORT

#endif // __BF_LIBEDITOR_EXPORT_HPP__
