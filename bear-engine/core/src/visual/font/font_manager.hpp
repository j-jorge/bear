/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to manage font resources.
 * \author Julien Jorge
 */
#ifndef __VISUAL_FONT_MANAGER_HPP__
#define __VISUAL_FONT_MANAGER_HPP__

#include "visual/font/font.hpp"
#include "visual/font/bitmap_charmap.hpp"
#include "visual/font/true_type_memory_file.hpp"

#include <iostream>
#include <map>
#include <string>

#include <boost/tuple/tuple.hpp>

#include "visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class to manage font resources.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT font_manager
    {
    private:
      /** \brief The type of the container associating the character mapping of
          a bitmap font with the identifier of the bitmap font. */
      typedef
        std::map<std::string, bitmap_charmap> id_to_charmap_type;

      /** \brief The type of the container associating a true type memory file
          with the identifier of the true type font. */
      typedef
        std::map<std::string, true_type_memory_file> id_to_memory_file_type;

      /** \brief The structure describing a font in the manager. */
      typedef boost::tuple<std::string, double> font_id_type;

      /** \brief The type of the container associating a font with its
          identifier. */
      typedef std::map<font_id_type, base_font*> font_map_type;

    public:
      ~font_manager();
      void clear();

      void load_font( std::string name, const bitmap_charmap& m );
      void load_font( std::string name, std::istream& file );

      void clear_fonts();
      void restore_fonts();

      font get_font( std::string name, double size ) const;

      bool exists( const std::string& name ) const;

    private:
      /** \brief The character mapping of each bitmap font. */
      id_to_charmap_type m_bitmap_charmap;

      /** \brief The file data of each true type font. */
      id_to_memory_file_type m_memory_file;

      /** \brief All the fonts built by this manager. */
      mutable font_map_type m_font;

    }; // class font_manager

  } // namespace visual
} // namespace bear

#endif // __VISUAL_FONT_MANAGER_HPP__
