/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the EXPR_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __EXPR_CLASS_EXPORT_HPP__
#define __EXPR_CLASS_EXPORT_HPP__

#ifndef EXPR_EXPORT
#  ifdef _WIN32
#    ifdef bear_expr_EXPORTS
#      define EXPR_EXPORT __declspec(dllexport)
#    else
#      define EXPR_EXPORT __declspec(dllimport)
#    endif // def bear_expr_EXPORTS
#  else // def _WIN32
#    define EXPR_EXPORT
#  endif // def _WIN32
#endif // ndef EXPR_EXPORT

#endif // __EXPR_CLASS_EXPORT_HPP__
