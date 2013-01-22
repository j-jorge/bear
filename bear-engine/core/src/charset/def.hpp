/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Various types to use for strings and unicode.
 * \author Julien Jorge
 */
#ifndef __CHARSET_DEF_HPP__
#define __CHARSET_DEF_HPP__

#include <string>

namespace bear
{
  namespace charset
  {
    /** \brief Unicode character type. */
    typedef wchar_t char_type;

    /** \brief Unicode string. */
    typedef std::string string_type;

  } // namespace charset
} // namespace bear

#endif // __CHARSET_DEF_HPP__
