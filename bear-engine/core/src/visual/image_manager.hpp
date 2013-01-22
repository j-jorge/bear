/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to manage images resources.
 * \author Julien Jorge
 */
#ifndef __VISUAL_IMAGE_MANAGER_HPP__
#define __VISUAL_IMAGE_MANAGER_HPP__

#include "visual/image.hpp"
#include <iostream>
#include <map>
#include <string>

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class to manage images resources.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT image_manager
    {
    public:
      void clear();
      void load_image( const std::string& name, std::istream& file );

      void clear_images();
      void restore_image( const std::string& name, std::istream& file );

      const image& get_image( const std::string& name ) const;
      void get_image_names( std::vector<std::string>& names ) const;

      bool exists( const std::string& name ) const;

    private:
      /** \brief All images. */
      std::map<std::string, image> m_images;

    }; // class image_manager

  } // namespace visual
} // namespace bear

#endif // __VISUAL_IMAGE_MANAGER_HPP__
