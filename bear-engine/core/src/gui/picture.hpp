/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A simple component to display an image (sprite) in windows or any
 *        visual component.
 * \author Julien Jorge
 */
#ifndef __GUI_PICTURE_HPP__
#define __GUI_PICTURE_HPP__

#include "gui/scene_element.hpp"
#include "visual/sprite.hpp"

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A simple component to display an image (sprite) in windows or any
     *        visual component.
     * \author Julien Jorge
     */
    class GUI_EXPORT picture:
      public scene_element
    {
    public:
      explicit picture
      ( const visual::sprite& pict = visual::sprite() );

      void set_picture( const visual::sprite& pict );

    }; // class picture
  } // namespace gui
} // namespace bear

#endif // __GUI_PICTURE_HPP__
