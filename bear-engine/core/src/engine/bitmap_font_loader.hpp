/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class loads a bitmap font.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BITMAP_FONT_LOADER_HPP__
#define __ENGINE_BITMAP_FONT_LOADER_HPP__

#include "visual/font/bitmap_charmap.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class level_globals;

    /**
     * \brief This class loads a bitmap font.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT bitmap_font_loader
    {
    public:
      bitmap_font_loader( std::istream& f, level_globals& glob );

      visual::bitmap_charmap run();

    private:
      visual::bitmap_charmap read_legacy();
      visual::bitmap_charmap read_autofont();

      void read_autofont_image
        ( visual::bitmap_charmap& cs,
          const std::string& image_name ) const;
      void read_autofont_options( visual::bitmap_charmap& cs );

      void upper_to_lower( visual::bitmap_charmap& cs ) const;

      std::string get_next_line() const;
      void fail( const std::string& msg ) const;

    private:
      /** \brief The file from which we load the font. */
      std::istream& m_file;

      /** \brief The level_globals in which we load the resources. */
      level_globals& m_level_globals;

      /** \brief The keyword that starts an autofont file. */
      static const std::string s_autofont_keyword;

      /** \brief The keyword that starts a section of options. */
      static const std::string s_options_keyword;

    }; // class bitmap_font_loader
  } // namespace engine
} // namespace bear

#endif // __ENGINE_BITMAP_FONT_LOADER_HPP__
