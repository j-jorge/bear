/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief All the items of a level.
 * \author Julien Jorge
 */
#ifndef __ENGINE_POPULATION_HPP__
#define __ENGINE_POPULATION_HPP__

#include <map>
#include <set>
#include <claw/functional.hpp>
#include <claw/iterator.hpp>

#include "bear/engine/base_item.hpp"
#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief All the items of a level.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT population
    {
    private:
      /** brief The type of the map containing the items. */
      typedef std::map<base_item::id_type, base_item*> item_map;

    public:
      /** \brief Iterator on the living items. */
      typedef claw::wrapped_iterator
        < base_item,
          item_map::const_iterator,
          claw::unary_compose
          < claw::dereference<base_item>,
            claw::const_pair_second<item_map::value_type> > >
      ::iterator_type const_iterator;

    public:
      ~population();

      void insert( base_item* item );
      void kill( const base_item* item );
      void drop( const base_item* item );

      bool exists( base_item::id_type id ) const;

      void remove_dead_items();
      void clear();

      const_iterator begin() const;
      const_iterator end() const;

    private:
      /** \brief All items currently in the game. */
      item_map m_items;

      /** \brief The items that will be deleted by calling
          remove_dead_items(). */
      std::set<base_item::id_type> m_dead_items;

      /** \brief The items that will be removed by calling
          remove_dead_items(). */
      std::set<base_item::id_type> m_dropped_items;

    }; // class population
  } // namespace engine
} // namespace bear

#endif // __ENGINE_POPULATION_HPP__
