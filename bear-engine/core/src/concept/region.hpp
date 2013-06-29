/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A region is a list of shapes in a 2D space.
 * \author Julien Jorge.
 */
#ifndef __CONCEPT_REGION_HPP__
#define __CONCEPT_REGION_HPP__

#include <list>

namespace bear
{
  namespace concept
  {
    /**
     * \brief A region is a list of shapes in a 2D space.
     *
     * The \a Shape type should have the following methods :
     * - bool intersects( Shape ) : tell if two shapes intersects,
     * - bool includes( Shape ) : tell if a shape is included in an other one,
     * - bool includes( Point ) : tell if a shape includes a coordinate.
     *
     * \author Julien Jorge.
     */
    template <class Shape, class Container = std::list<Shape> >
    class region : public Container
    {
    public:
      /** \brief The type of the shapes. */
      typedef Shape shape_type;

      /** \brief The type of the parent class. */
      typedef Container super;

    public:
      bool intersects( const shape_type& shape ) const;

      bool includes( const shape_type& shape ) const;
      template<typename Point>
      bool includes( const Point& p ) const;

    }; // class region

  } // namespace concept
} // namespace bear

#include "concept/impl/region.tpp"

#endif // __CONCEPT_REGION_HPP__
