/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Version of the editor.
 * \author Julien Jorge
 */
#ifndef __BF_VERSION_HPP__
#define __BF_VERSION_HPP__

#define BF_TO_STR_BIS(v) # v
#define BF_TO_STR(v) BF_TO_STR_BIS(v)

#define BF_MAJOR_VERSION 0
#define BF_MINOR_VERSION 9
#define BF_RELEASE_NUMBER 0
#define BF_VERSION_STRING "Bear Factory, " BF_TO_STR(BF_MAJOR_VERSION) "." \
  BF_TO_STR(BF_MINOR_VERSION) "." BF_TO_STR(BF_RELEASE_NUMBER)

#endif // __BF_VERSION_HPP__
