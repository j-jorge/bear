/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A writing on the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_WRITING_HPP__
#define __VISUAL_SCENE_WRITING_HPP__

#include "visual/base_scene_element.hpp"
#include "visual/writing.hpp"

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A writing on the screen.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_writing:
      public base_scene_element
    {
    private:
      /** \brief The type of the writing to display. */
      typedef writing writing_type;

    public:
      scene_writing( coordinate_type x, coordinate_type y, const writing& s );

      base_scene_element* clone() const;

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;
      void render( const bitmap_writing& w, base_screen& scr ) const;

    private:
      /** \brief The writing. */
      const writing m_writing;

    }; // class scene_writing
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCENE_WRITING_HPP__
