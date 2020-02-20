/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that stores a graphic representation of an object and its
 *        position in the world. This class is used to store the visible items
 *        in the rendering step.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SCENE_VISUAL_HPP__
#define __ENGINE_SCENE_VISUAL_HPP__

#include "bear/visual/scene_element.hpp"
#include "bear/visual/sprite.hpp"
#include "bear/universe/types.hpp"
#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A class that stores a graphic representation of an object and its
     *        position in the world. This class is used to store the visible
     *        items in the rendering step.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT scene_visual
    {
    public:
      /** \brief Compare two scene_visual instances on their z_position. */
      struct ENGINE_EXPORT z_position_compare
      {
        bool operator()( const scene_visual& s1, const scene_visual& s2 ) const;
      }; // struct z_position_compare

    public:
      scene_visual( universe::coordinate_type x, universe::coordinate_type y,
                    const visual::sprite& spr, int z = 0 );
      scene_visual( const universe::position_type& pos,
                    const visual::sprite& spr, int z = 0 );
      scene_visual( const visual::scene_element& e, int z = 0 );
      scene_visual( const visual::base_scene_element& e, int z = 0 );

    public:
      /** \brief The visual to display. */
      visual::scene_element scene_element;

      /** \brief Position of the visual in the rendering procedure. */
      int z_position;

    }; // class scene_visual
  } // namespace engine
} // namespace bear

#endif // __ENGINE_SCENE_VISUAL_HPP__
