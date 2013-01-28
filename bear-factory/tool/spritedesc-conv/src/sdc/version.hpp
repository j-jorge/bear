/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Version of the tool.
 * \author Julien Jorge
 */
#ifndef __SDC_VERSION_HPP__
#define __SDC_VERSION_HPP__

#define SDC_TO_STR_BIS(v) # v
#define SDC_TO_STR(v) SDC_TO_STR_BIS(v)

#define SDC_MAJOR_VERSION 1
#define SDC_MINOR_VERSION 0
#define SDC_RELEASE_NUMBER 0
#define SDC_VERSION_STRING "Pack My Sprites, "                          \
  SDC_TO_STR(SDC_MAJOR_VERSION) "." SDC_TO_STR(SDC_MINOR_VERSION)       \
  "." SDC_TO_STR(SDC_RELEASE_NUMBER)

#endif // __SDC_VERSION_HPP__
