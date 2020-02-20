/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Definition of the COMMUNICATION_EXPORT macro according to the fact
 *        that we create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __COMMUNICATION_CLASS_EXPORT_HPP__
#define __COMMUNICATION_CLASS_EXPORT_HPP__

#ifndef COMMUNICATION_EXPORT
#  ifdef _WIN32
#    ifdef bear_communication_EXPORTS
#      define COMMUNICATION_EXPORT __declspec(dllexport)
#    else
#      define COMMUNICATION_EXPORT __declspec(dllimport)
#    endif // def bear_communication_EXPORTS
#  else // def _WIN32
#    define COMMUNICATION_EXPORT
#  endif // def _WIN32
#endif // ndef COMMUNICATION_EXPORT

#endif // __COMMUNICATION_CLASS_EXPORT_HPP__
