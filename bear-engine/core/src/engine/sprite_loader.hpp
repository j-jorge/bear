/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to load sprites and animations
 * \author Julien Jorge
 */
#ifndef __ENGINE_SPRITE_LOADER_HPP__
#define __ENGINE_SPRITE_LOADER_HPP__

#include "visual/animation.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class compiled_file;
    class level_globals;

    class ENGINE_EXPORT sprite_loader
    {
    public:
      static visual::sprite
      load_sprite( compiled_file& f, level_globals& glob );

      static visual::animation
      load_animation( compiled_file& f, level_globals& glob );
      static visual::animation
      load_any_animation( compiled_file& f, level_globals& glob );
      static visual::animation
      load_animation_data( compiled_file& f, level_globals& glob );

      static void
      load_bitmap_rendering_attributes
      ( compiled_file& f, visual::bitmap_rendering_attributes& result );

    private:
      static visual::animation
      load_animation_v0_5( compiled_file& f, level_globals& glob );
    }; // class sprite_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SPRITE_LOADER_HPP__
