/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item with a visual::bitmap_rendering_attributes.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_RENDERING_ATTRIBUTES_HPP__
#define __ENGINE_WITH_RENDERING_ATTRIBUTES_HPP__

#include "bear/visual/bitmap_rendering_attributes.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item with a visual::bitmap_rendering_attributes.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_rendering_attributes
    {
    public:
      virtual ~with_rendering_attributes();

      visual::bitmap_rendering_attributes& get_rendering_attributes();
      const visual::bitmap_rendering_attributes&
        get_rendering_attributes() const;

      void set_rendering_attributes
        ( const visual::bitmap_rendering_attributes& attr );

    private:
      /** \brief Global rendering attributes of the item. */
      visual::bitmap_rendering_attributes m_rendering_attributes;

    }; // class with_rendering_attributes
  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_RENDERING_ATTRIBUTES_HPP__
