/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The value of a field of type "sample".
 * \author Julien Jorge
 */
#ifndef __BF_SAMPLE_HPP__
#define __BF_SAMPLE_HPP__

#include "bf/bitmap_rendering_attributes.hpp"
#include "bf/libeditor_export.hpp"

#include <string>

namespace bf
{
  /**
   * \brief The value of a field of type "sample".
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT sample:
    public bitmap_rendering_attributes
  {
  public:
    sample();

    void set_path( const std::string& name );
    void set_loops( const unsigned int loops );
    void set_volume( const double v );

    unsigned int get_loops() const;
    double get_volume() const;
    const std::string& get_path() const;

    bool operator==( const sample& that ) const;
    bool operator!=( const sample& that ) const;

    void compile( compiled_file& f ) const;

  private:
    /** \brief The path to the resource to use. */
    std::string m_path;

    /** \brief How many times the sample will be played. */
    unsigned int m_loops;

    /** \brief The volume at which the sample is played. */
    double m_volume;

  }; // class sample
} // namespace bf

#endif // __BF_SAMPLE_HPP__
