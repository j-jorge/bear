/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Removes the current shader program.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_SHADER_POP_HPP__
#define __VISUAL_SCENE_SHADER_POP_HPP__

#include "visual/base_scene_element.hpp"
#include "visual/shader_program.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief Removes the current shader program.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_shader_pop:
      public base_scene_element
    {
    public:
      scene_shader_pop();

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

      bool always_displayed() const;

    }; // class scene_shader_pop
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_SHADER_POP_HPP__
