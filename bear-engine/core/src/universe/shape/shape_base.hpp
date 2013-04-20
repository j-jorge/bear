/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The common interface for all the shapes of the physical items.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_SHAPE_BASE_HPP__
#define __UNIVERSE_SHAPE_BASE_HPP__

#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class rectangle;
    class curved_box;

    /**
     * \brief The common interface for all the shapes of the physical items.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT shape_base
    {
    public:
      virtual ~shape_base();
      virtual shape_base* clone() const = 0;

      bool intersects( const shape_base& that ) const;

      size_type get_width() const;
      void set_width( size_type width );

      size_type get_height() const;
      void set_height( size_type height );

      coordinate_type get_bottom() const;
      void set_bottom( coordinate_type pos );

      coordinate_type get_left() const;
      void set_left( coordinate_type pos );

      virtual bool intersects( const rectangle& that ) const = 0;
      virtual bool intersects( const curved_box& that ) const = 0;

    private:
      virtual coordinate_type do_get_bottom() const = 0;
      virtual void do_set_bottom( coordinate_type p ) = 0;

      virtual coordinate_type do_get_left() const = 0;
      virtual void do_set_left( coordinate_type p ) = 0;

      virtual size_type do_get_width() const = 0;
      virtual void do_set_width( size_type s ) = 0;

      virtual size_type do_get_height() const = 0;
      virtual void do_set_height( size_type s ) = 0;

      virtual bool do_intersects( const shape_base& that ) const = 0;

    }; // class shape_base
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_SHAPE_BASE_HPP__
