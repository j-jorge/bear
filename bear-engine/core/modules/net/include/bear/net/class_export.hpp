/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the NET_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __NET_CLASS_EXPORT_HPP__
#define __NET_CLASS_EXPORT_HPP__

#ifndef NET_EXPORT
#  ifdef _WIN32
#    ifdef bear_net_EXPORTS
#      define NET_EXPORT __declspec(dllexport)
#    else
#      define NET_EXPORT __declspec(dllimport)
#    endif // def bear_net_EXPORTS
#  else // def _WIN32
#    define NET_EXPORT
#  endif // def _WIN32
#endif // ndef NET_EXPORT

#endif // __NET_CLASS_EXPORT_HPP__
