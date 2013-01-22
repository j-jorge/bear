/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Definition of the GENERIC_ITEMS_EXPORT macro according to the fact
 *        that we create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __BEAR_GENERIC_ITEMS_CLASS_EXPORT_HPP__
#define __BEAR_GENERIC_ITEMS_CLASS_EXPORT_HPP__

#ifndef GENERIC_ITEMS_EXPORT
#  ifdef _WIN32
#    ifdef bear_generic_items_EXPORTS
#      define GENERIC_ITEMS_EXPORT __declspec(dllexport)
#    else
#      define GENERIC_ITEMS_EXPORT __declspec(dllimport)
#    endif // def bear_generic_items_EXPORTS
#  else // def _WIN32
#    define GENERIC_ITEMS_EXPORT
#  endif // def _WIN32
#endif // ndef GENERIC_ITEMS_EXPORT

#endif // __BEAR_GENERIC_ITEMS_CLASS_EXPORT_HPP__
