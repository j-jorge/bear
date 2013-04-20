/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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

      size_box_type get_size() const;
      void set_size( const size_box_type& size );

      size_type get_width() const;
      void set_width( size_type width );

      size_type get_height() const;
      void set_height( size_type height );

      void set_bounding_box( const rectangle_type& r );
      rectangle_type get_bounding_box() const;

      void set_top( coordinate_type pos );
      void set_bottom( coordinate_type pos );
      void set_left( coordinate_type pos );
      void set_right( coordinate_type pos );
      void set_horizontal_middle( coordinate_type pos );
      void set_vertical_middle( coordinate_type pos );

      void set_top_left( const position_type& pos );
      void set_top_middle( const position_type& pos );
      void set_top_right( const position_type& pos );
      void set_bottom_left( const position_type& pos );
      void set_bottom_middle( const position_type& pos );
      void set_bottom_right( const position_type& pos );
      void set_left_middle( const position_type& pos );
      void set_right_middle( const position_type& pos );

      void set_top_left( coordinate_type x, coordinate_type y );
      void set_top_middle( coordinate_type x, coordinate_type y );
      void set_top_right( coordinate_type x, coordinate_type y );
      void set_bottom_left( coordinate_type x, coordinate_type y );
      void set_bottom_middle( coordinate_type x, coordinate_type y );
      void set_bottom_right( coordinate_type x, coordinate_type y );
      void set_left_middle( coordinate_type x, coordinate_type y );
      void set_right_middle( coordinate_type x, coordinate_type y );

      coordinate_type get_left() const;
      coordinate_type get_top() const;
      coordinate_type get_right() const;
      coordinate_type get_bottom() const;
      coordinate_type get_horizontal_middle() const;
      coordinate_type get_vertical_middle() const;

      position_type get_top_left() const;
      position_type get_top_middle() const;
      position_type get_top_right() const;
      position_type get_bottom_left() const;
      position_type get_bottom_middle() const;
      position_type get_bottom_right() const;
      position_type get_left_middle() const;
      position_type get_right_middle() const;

      position_type get_center_of_mass() const;
      void set_center_of_mass( coordinate_type x, coordinate_type y );
      void set_center_of_mass( const position_type& pos );

    private:
      /** \brief The real shape behind this object. */
      shape_base* m_impl;

    }; // class shape
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_SHAPE_HPP__
