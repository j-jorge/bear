/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Version of the engine.
 * \author Julien Jorge
 */
#ifndef __BEAR_VERSION_HPP__
#define __BEAR_VERSION_HPP__

#define BEAR_TO_STR_BIS(v) # v
#define BEAR_TO_STR(v) BEAR_TO_STR_BIS(v)

#define BEAR_MAJOR_VERSION 0
#define BEAR_MINOR_VERSION 7
#define BEAR_RELEASE_NUMBER 0
#define BEAR_VERSION_STRING "Bear Engine " BEAR_TO_STR(BEAR_MAJOR_VERSION) "." \
  BEAR_TO_STR(BEAR_MINOR_VERSION) "." BEAR_TO_STR(BEAR_RELEASE_NUMBER)

#endif // __BEAR_VERSION_HPP__
