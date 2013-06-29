/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A static map is a class allowing us to represent a 2D map of items in
 *        a compact way.
 * \author Julien Jorge.
 */
#ifndef __UNIVERSE_STATIC_MAP_HPP__
#define __UNIVERSE_STATIC_MAP_HPP__

#include <list>
#include <vector>
#include <claw/box_2d.hpp>
#include <claw/coordinate_2d.hpp>

#include "universe/types.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A static map is a class allowing us to represent a 2D map of items
     *        in a compact way.
     *
     * The full map is divided into little boxes in which items are listed.
     * Let's say that we have a map of 1000 x 1000 cells. There isn't items in
     * all the million cells but all take place in memory. By using a static_map
     * of size 100 x 100 (with boxes of size 10 x 10), we can reduce the memory
     * used by a square of 100 empty cells into only one cell. If there is items
     * in a box, we list them in a cell ; the memory used remains the same but
     * the access is a little bit longer.
     *
     * \b Template parameters
     * - ItemType is the type of the stored items. Must inherit from
     *   physical_item_state.
     */
    template<class ItemType>
    class static_map
    {
    public:
      /** \brief The type of the items we store. */
      typedef ItemType item_type;

      /** \brief The type of an area. */
      typedef rectangle_type area_type;

      /** \brief A list of items, can contains duplicates. */
      typedef std::list<item_type> item_list;

    private:
      /** \brief Items in a cell. */
      typedef std::list<item_type> item_box;

      /** \brief A column of the map. */
      typedef std::vector<item_box> column;

      /** \brief The whole map. */
      typedef std::vector<column> map;

    public:
      static_map
      ( unsigned int width, unsigned int height, unsigned int box_size );

      void insert( const item_type& item );

      template<typename AreaIterator>
      void get_areas
      ( AreaIterator first, AreaIterator last, item_list& items ) const;
      template<typename AreaIterator>
      void get_areas_unique
      ( AreaIterator first, AreaIterator last, item_list& items ) const;

      void get_area_unique( const area_type& area, item_list& items ) const;
      void get_area( const area_type& area, item_list& items ) const;
      void get_all_unique( item_list& items ) const;

      unsigned int empty_cells() const;
      void
      cells_load( unsigned int& min, unsigned int& max, double& avg ) const;

    private:
      void make_set( item_list& items ) const;

    private:
      /** \brief The size of the boxes. */
      const unsigned int m_box_size;

      /** \brief The real size of the map. */
      const claw::math::coordinate_2d<unsigned int> m_size;

      /** \brief The whole map. */
      map m_map;

    }; // class static_map

  } // namespace universe
} // namespace bear

#include "universe/impl/static_map.tpp"

#endif // __UNIVERSE_STATIC_MAP__
