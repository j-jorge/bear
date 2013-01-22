/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Definition of the AUDIO_EXPORT macro according to the fact that we
 *        create the library or link to it.
 * \author Julien Jorge
 */
#ifndef __AUDIO_CLASS_EXPORT_HPP__
#define __AUDIO_CLASS_EXPORT_HPP__

#ifndef AUDIO_EXPORT
#  ifdef _WIN32
#    ifdef bear_audio_EXPORTS
#      define AUDIO_EXPORT __declspec(dllexport)
#    else
#      define AUDIO_EXPORT __declspec(dllimport)
#    endif // def bear_audio_EXPORTS
#  else // def _WIN32
#    define AUDIO_EXPORT
#  endif // def _WIN32
#endif // ndef AUDIO_EXPORT

#endif // __AUDIO_CLASS_EXPORT_HPP__
