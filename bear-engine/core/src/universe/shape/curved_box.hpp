/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A rectangle with a curved top.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CURVED_BOX_HPP__
#define __UNIVERSE_CURVED_BOX_HPP__

#include "universe/shape/shape_base.hpp"

#include <claw/curve.hpp>

namespace bear
{
  namespace universe
  {
    class rectangle;

    /**
     * \brief A rectangle with a curved top.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT curved_box:
      public shape_base
    {
    public:
      /** \brief The type of the curve describing the top of the shape. */
      typedef claw::math::curve<position_type> curve_type;

    public:
      curved_box();

      virtual curved_box* clone() const;

      virtual bool intersects( const rectangle& that ) const;
      virtual bool intersects( const curved_box& that ) const;

      coordinate_type get_steepness() const;
      void set_steepness( coordinate_type s );

      coordinate_type get_margin() const;
      void set_margin( coordinate_type s );

      vector_type get_left_control_point() const;
      void set_left_control_point( const vector_type& p );

      vector_type get_right_control_point() const;
      void set_right_control_point( const vector_type& p );

      curve_type get_curve() const;

      coordinate_type get_y_at_x( coordinate_type x ) const;

    private:
      virtual coordinate_type do_get_bottom() const;
      virtual void do_set_bottom( coordinate_type p );

      virtual coordinate_type do_get_left() const;
      virtual void do_set_left( coordinate_type p );

      virtual size_type do_get_width() const;
      virtual void do_set_width( size_type s );

      virtual size_type do_get_height() const;
      virtual void do_set_height( size_type s );

      virtual bool do_intersects( const shape_base& that ) const;

      bool check_intersection_above
        ( const position_type& bottom_left_position,
          const position_type& bottom_right_position ) const;

    private:
      /** \brief The reference position. */
      position_type m_bottom_left;

      /** \brief The size of the rectangle. */
      size_box_type m_size;

      /** \brief The steepness of the top. */
      coordinate_type m_steepness;

      /** \brief The margin of the top. */
      coordinate_type m_top_margin;

      /** \brief The vector of left control point. */
      vector_type m_left_control_point;
    
      /** \brief The vector of right control point. */
      vector_type m_right_control_point;
    
      /** \brief The width of the curve, uset to check the collisions. */
      static coordinate_type s_line_width;

    }; // class curved_box
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_CURVED_BOX_HPP__
