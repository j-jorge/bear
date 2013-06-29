/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Description of the world.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WORLD_HPP__
#define __ENGINE_WORLD_HPP__

#include "universe/world.hpp"
#include "engine/class_export.hpp"
#include "engine/population.hpp"
#include "engine/scene_visual.hpp"

#include <list>

namespace bear
{
  namespace engine
  {
    /**
     * \brief Description of the world.
     *
     * The world contains the walls and all the entities.
     */
    class ENGINE_EXPORT world :
      public universe::world
    {
    public:
      /** \brief The type of the iterator on the living items. */
      typedef population::const_iterator const_item_iterator;

    public:
      world( const universe::size_box_type& size );
      ~world();

      void start();

      void progress_entities
      ( const region_type& regions, universe::time_type elapsed_time );

      void get_visual
      ( std::list<scene_visual>& visuals,
        const universe::rectangle_type& camera_box ) const;

      const_item_iterator living_items_begin() const;
      const_item_iterator living_items_end() const;

      void add_static( base_item* const& who );

      void register_item( base_item* const& who );
      void release_item( base_item* const& who );
      void drop_item( base_item* const& who );

      void print_item_stats() const;

    private:
      /** \brief The static items won't be deleted when cleaning the
          population. */
      std::list<base_item*> m_static_items;

      /** \brief All items that can die a any time. */
      population m_population;

    }; // class world
  } // namespace engine
} // namespace bear

#endif // __ENGINE_WORLD_HPP__
