/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A rectangular shape.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_RECTANGLE_HPP__
#define __UNIVERSE_RECTANGLE_HPP__

#include "universe/shape/shape_base.hpp"

namespace bear
{
  namespace universe
  {
    class curved_box;

    /**
     * \brief A rectangular shape.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT rectangle:
      public shape_base
    {
    public:
      rectangle();
      explicit rectangle( const rectangle_type& that );

      virtual rectangle* clone() const;

      virtual bool intersects( const rectangle& that ) const;
      virtual bool intersects( const curved_box& that ) const;

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

    private:
      /** \brief The reference position. */
      position_type m_bottom_left;

      /** \brief The size of the rectangle. */
      size_box_type m_size;

    }; // class rectangle
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_RECTANGLE_HPP__
