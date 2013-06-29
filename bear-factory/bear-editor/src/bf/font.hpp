/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The value of a field of type "font".
 * \author Julien Jorge
 */
#ifndef __BF_FONT_HPP__
#define __BF_FONT_HPP__

#include "bf/libeditor_export.hpp"

#include <string>

namespace bf
{
  class compiled_file;

  /**
   * \brief Font type.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT font
  {
  public:
    font();

    void set_font_name( std::string name );
    std::string get_font_name() const;

    void set_size( double s );
    double get_size() const;

    void compile( compiled_file& f ) const;

    bool operator==( const font& that ) const;
    bool operator!=( const font& that ) const;

  private:
    /** \brief The name of the font resource to use. */
    std::string m_font_name;

    /** \brief The size of the text displayed with this font. */
    double m_size;

  }; // class font
} // namespace bf

#endif // __BF_FONT_HPP__
