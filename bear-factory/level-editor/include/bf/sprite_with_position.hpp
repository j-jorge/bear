/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A sprite with a position and an angle.
 * \author Julien Jorge
 */
#ifndef __BF_SPRITE_WITH_POSITION_HPP__
#define __BF_SPRITE_WITH_POSITION_HPP__

#include <wx/gdicmn.h>

#include "bf/sprite.hpp"

namespace bf
{
  /**
   * \brief A sprite with a position.
   * \author Julien Jorge
   */
  struct sprite_with_position
  {
    /** \brief The sprite. */
    sprite spr;

    /** \brief The position. */
    wxPoint pos;

  }; // struct sprite_with_position
} // namespace bf

#endif // __BF_SPRITE_WITH_POSITION_HPP__
