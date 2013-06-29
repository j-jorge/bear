/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Enables a shader program for the next scene elements.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_SHADER_PUSH_HPP__
#define __VISUAL_SCENE_SHADER_PUSH_HPP__

#include "visual/base_scene_element.hpp"
#include "visual/shader_program.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief Enables a shader program for the next scene elements.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_shader_push:
      public base_scene_element
    {
    public:
      scene_shader_push( shader_program p );

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

      bool always_displayed() const;

    private:
      /** \brief The shader to use when rendering the next elements. */
      const shader_program m_shader;

    }; // class scene_shader_push
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_SHADER_PUSH_HPP__
