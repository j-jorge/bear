/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Macro to call gettext on the bear-engine text domain.
 * \author Julien Jorge
 */
#ifndef __BEAR_GETTEXT_HPP__
#define __BEAR_GETTEXT_HPP__

#include <libintl.h>

#define bear_gettext(s) dgettext( "bear-engine", (s) )

#endif // __BEAR_GETTEXT_HPP__
