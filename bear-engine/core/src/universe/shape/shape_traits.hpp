/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A traits class that helps to access remarkable points of shapes.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_SHAPE_TRAITS_HPP__
#define __UNIVERSE_SHAPE_TRAITS_HPP__

#include "universe/types.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A traits class that helps to access remarkable points of shapes.
     *
     * The type passed as template parameter must implements the following
     * methods:
     *  - coordinate_type T::get_bottom() const,
     *  - void T::set_bottom( coordinate_type ) const,
     *  - coordinate_type T::get_left() const,
     *  - void T::set_left( coordinate_type ) const,
     *  - size_type T::get_width() const,
     *  - void T::set_width( size_type ) const,
     *  - size_type T::get_height() const,
     *  - void T::set_height( size_type ) const.
     *
     * \author Julien Jorge
     */
    template<typename T>
    struct shape_traits
    {
      /** \brief The type of the shape to which these traits are applied. */
      typedef T shape_type;

      static size_box_type get_size( const shape_type& s );
      static void set_size( shape_type& s, const size_box_type& size );

      static size_type get_width( const shape_type& s );
      static void set_width( shape_type& s, size_type width );

      static size_type get_height( const shape_type& s );
      static void set_height( shape_type& s, size_type height );

      static void set_bounding_box
      ( shape_type& s, const rectangle_type& r );
      static rectangle_type get_bounding_box( const shape_type& s );

      static void set_top( shape_type& s, coordinate_type pos );
      static void set_bottom( shape_type& s, coordinate_type pos );
      static void set_left( shape_type& s, coordinate_type pos );
      static void set_right( shape_type& s, coordinate_type pos );
      static void set_horizontal_middle( shape_type& s, coordinate_type pos );
      static void set_vertical_middle( shape_type& s, coordinate_type pos );

      static void set_top_left( shape_type& s, const position_type& pos );
      static void set_top_middle( shape_type& s, const position_type& pos );
      static void set_top_right( shape_type& s, const position_type& pos );
      static void set_bottom_left( shape_type& s, const position_type& pos );
      static void set_bottom_middle( shape_type& s, const position_type& pos );
      static void set_bottom_right( shape_type& s, const position_type& pos );
      static void set_left_middle( shape_type& s, const position_type& pos );
      static void set_right_middle( shape_type& s, const position_type& pos );

      static void set_top_left
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_top_middle
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_top_right
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_bottom_left
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_bottom_middle
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_bottom_right
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_left_middle
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_right_middle
      ( shape_type& s, coordinate_type x, coordinate_type y );

      static coordinate_type get_left( const shape_type& s );
      static coordinate_type get_top( const shape_type& s );
      static coordinate_type get_right( const shape_type& s );
      static coordinate_type get_bottom( const shape_type& s );
      static coordinate_type get_horizontal_middle( const shape_type& s );
      static coordinate_type get_vertical_middle( const shape_type& s );

      static position_type get_top_left( const shape_type& s );
      static position_type get_top_middle( const shape_type& s );
      static position_type get_top_right( const shape_type& s );
      static position_type get_bottom_left( const shape_type& s );
      static position_type get_bottom_middle( const shape_type& s );
      static position_type get_bottom_right( const shape_type& s );
      static position_type get_left_middle( const shape_type& s );
      static position_type get_right_middle( const shape_type& s );

      static position_type get_center( const shape_type& s );
      static void set_center
      ( shape_type& s, coordinate_type x, coordinate_type y );
      static void set_center( shape_type& s, const position_type& pos );
      
    }; // struct shape_traits

  } // namespace universe
} // namespace bear

#include "universe/shape/impl/shape_traits.tpp"

#endif // __UNIVERSE_SHAPE_TRAITS_HPP__
