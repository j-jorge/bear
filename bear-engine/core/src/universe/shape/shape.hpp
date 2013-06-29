/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A proxy for the various shapes.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_SHAPE_HPP__
#define __UNIVERSE_SHAPE_HPP__

#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class shape_base;

    /**
     * \brief A proxy for the various shapes.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT shape
    {
    public:
      shape();
      shape( const shape_base& s );
      shape( const shape& that );
      ~shape();

      shape& operator=( shape that );

      bool intersects( const shape& that ) const;

      size_type get_width() const;
      void set_width( size_type width );

      size_type get_height() const;
      void set_height( size_type height );

      coordinate_type get_bottom() const;
      void set_bottom( coordinate_type pos );

      coordinate_type get_left() const;
      void set_left( coordinate_type pos );

      shape_base* clone_impl() const;

    private:
      /** \brief The real shape behind this object. */
      shape_base* m_impl;

    }; // class shape
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_SHAPE_HPP__
