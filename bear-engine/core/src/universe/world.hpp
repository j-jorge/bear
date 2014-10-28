/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This is the representation of the world.
 * \author Julien Jorge.
 */
#ifndef __UNIVERSE_WORLD_HPP__
#define __UNIVERSE_WORLD_HPP__

#include "concept/item_container.hpp"
#include "concept/region.hpp"

#include "universe/environment_type.hpp"
#include "universe/item_picking_filter.hpp"
#include "universe/static_map.hpp"

#include "universe/class_export.hpp"

#include <boost/bimap.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <vector>

namespace bear
{
  namespace universe
  {
    class density_rectangle;
    class environment_rectangle;
    class force_rectangle;
    class friction_rectangle;
    class physical_item;
    class physical_item_state;

    /**
     * \brief This is the representation of the world.
     *
     * The world is made of static surfaces (round, walls, ...), living items
     * (heroes, enemies, ... ) and everything is governed by physical rules.
     *
     * \author Julien Jorge.
     */
    class UNIVERSE_EXPORT world:
      public concept::item_container<physical_item*>
    {
    public:
      /** \brief Structure used for representing a region (a part) of the
          world. */
      typedef concept::region<rectangle_type> region_type;

      /** \brief The type of the map containing static items. */
      typedef static_map<physical_item*> item_map;

      /** \brief A list of items. */
      typedef std::vector<physical_item*> item_list;

    private:
      typedef boost::adjacency_list<> dependency_graph_type;
      typedef
        boost::bimap< physical_item*, dependency_graph_type::vertex_descriptor >
        dependency_vertex_map;

    public:
      explicit world( const size_box_type& size );
      ~world();

      void progress_entities
      ( const region_type& regions, time_type elapsed_time );

      void add_static(physical_item* who);

      time_type get_world_time() const;

      const size_box_type& get_size() const;
      void print_stats() const;

      const force_type& get_gravity() const;
      void set_gravity( const force_type& g );
      void set_scaled_gravity( const force_type& g );

      coordinate_type get_position_epsilon() const;
      void set_position_epsilon( coordinate_type epsilon );
      void set_scaled_position_epsilon( coordinate_type epsilon );

      const speed_type& get_speed_epsilon() const;
      void set_speed_epsilon( const speed_type& speed );
      void set_scaled_speed_epsilon( const speed_type& speed );

      double get_angular_speed_epsilon() const;
      void set_angular_speed_epsilon( double angular_speed );

      void set_unit( coordinate_type u );
      coordinate_type to_world_unit( coordinate_type m ) const;

      void set_default_friction( double f );
      double get_average_friction( const rectangle_type& r ) const;
      friction_rectangle*
      add_friction_rectangle( const rectangle_type& r, double f );

      universe::force_type get_average_force( const rectangle_type& r ) const;
      force_rectangle*
      add_force_rectangle( const rectangle_type& r, universe::force_type f );

      void set_default_density( double d );
      double get_average_density( const rectangle_type& r ) const;
      density_rectangle*
      add_density_rectangle( const rectangle_type& r, double f );

      force_type
        get_total_force_on_item( const physical_item_state& item ) const;

      void get_environments
      ( const rectangle_type& r,
        std::set<universe::environment_type>& environments ) const;
      bool is_in_environment
      (const position_type& pos, universe::environment_type environment) const;
      environment_rectangle* add_environment_rectangle
      ( const rectangle_type& r, const universe::environment_type e );
      void set_default_environment( const universe::environment_type e );

      void pick_items_by_position
      ( item_list& items, position_type p,
        const item_picking_filter& filter = item_picking_filter() ) const;
      void pick_items_in_rectangle
      ( item_list& items, rectangle_type r,
        const item_picking_filter& filter = item_picking_filter() ) const;
      void pick_items_in_circle
      ( item_list& items, position_type c, coordinate_type r,
        const item_picking_filter& filter = item_picking_filter() ) const;
      physical_item* pick_item_in_direction
      ( position_type p, vector_type dir,
        const item_picking_filter& filter = item_picking_filter() ) const;

    protected:
      void list_active_items
      ( item_list& items, const region_type& regions,
        const item_picking_filter& filter = item_picking_filter() ) const;

    private:
      struct candidate_collision
      {
        explicit candidate_collision( physical_item* i );

        physical_item* item;
        rectangle_type bounding_box;
      };

      typedef std::vector<candidate_collision> candidate_collisions;

    private:
      void detect_collision_all
        ( item_list& items, const candidate_collisions& potential_collision );
      physical_item* pick_next_collision( item_list& pending ) const;

      void detect_collision
      ( physical_item* item, item_list& pending, item_list& all_items,
        const candidate_collisions& potential_collision ) const;

      bool process_collision( physical_item& self, physical_item& that ) const;

      void search_items_for_collision
        ( const physical_item& item,
          const candidate_collisions& potential_collision,
          item_list& colliding, double& mass, double& area ) const;

      void item_found_in_collision
      ( const physical_item& item, physical_item* it, item_list& colliding,
        double& mass, double& area ) const;

      void search_pending_items_for_collision
      ( const physical_item& item, item_list& pending,
        std::list<item_list::iterator>& colliding ) const;

      void search_interesting_items
      ( const region_type& regions, item_list& items,
        candidate_collisions& potential_collision ) const;

      void stabilize_dependent_items( item_list& items ) const;
      void find_dependency_links
        ( item_list& pending, dependency_graph_type& graph,
          dependency_vertex_map& vertex, std::set<physical_item*>& single_items,
          physical_item* item ) const;
      void add_dependency_edge
        ( item_list& pending, dependency_graph_type& graph,
          dependency_vertex_map& vertex, std::set<physical_item*>& single_items,
          physical_item* tail, physical_item* head ) const;
      void add_dependency_vertex
        ( item_list& pending, dependency_graph_type& graph,
          dependency_vertex_map& vertex, std::set<physical_item*>& single_items,
          physical_item* v ) const;
      void make_sorted_dependency_list
        ( const dependency_graph_type& graph,
          const dependency_vertex_map& vertex,
          const std::set<physical_item*>& single_items,
          item_list& items ) const;

      void progress_items
      ( const item_list& items, time_type elapsed_time ) const;

      void progress_physic
      ( time_type elapsed_time, const item_list& items ) const;
      void progress_physic_move_item
      ( time_type elapsed_time, physical_item& item ) const;
      void apply_links(const item_list& items) const;

      void active_region_traffic( const item_list& items );

      void list_static_items
      ( const region_type& regions, item_list& items ) const;

      bool item_in_regions
      ( const physical_item& item, const region_type& regions ) const;

      void add( physical_item* const& who );
      void remove( physical_item* const& who );

      bool select_item( item_list& items, physical_item* it ) const;
      void unselect_item( item_list& items, item_list::iterator it ) const;

      void add_to_collision_queue
        ( item_list& items, physical_item* item,
          const candidate_collisions& potential_collision ) const;
      void add_to_collision_queue_no_neighborhood
      ( item_list& items, physical_item* item ) const;
      bool create_neighborhood
        ( physical_item& item,
          const candidate_collisions& potential_collision ) const;

      bool interesting_collision
        ( const physical_item& a, const physical_item& b ) const;

    private:
      // not implemented.
      world( const world& );
      world& operator=( world );

    private:
      /** \brief Size of the parts of m_static_surfaces. */
      static const unsigned int s_map_compression;

      /** \brief The elapsed time since the creation of the world. */
      time_type m_time;

      /** \brief The living entities. Can be added and deleted any time. */
      item_list m_entities;

      /** \brief The static surfaces of the world. */
      item_map m_static_surfaces;

      /** \brief The global static items. */
      item_list m_global_static_items;

      /** \brief The size of the world. */
      size_box_type m_size;

      /** \brief Entity in the last active region. */
      item_list m_last_interesting_items;

      /** \brief The unit of the world. m_unit units == 1 meter. */
      coordinate_type m_unit;

      /** \brief Gravity applied to the items. */
      force_type m_gravity;

      /** \brief Default friction applied to the items. */
      double m_default_friction;

      /** \brief A set of regions where the friction of the environment differs
          from m_default_friction. */
      std::list<friction_rectangle*> m_friction_rectangle;

      /** \brief A set of regions where the force is applied. */
      std::list<force_rectangle*> m_force_rectangle;

      /** \brief A set of regions with environment. */
      std::list<environment_rectangle*> m_environment_rectangle;

      /** \brief Default environment of the world. */
      environment_type m_default_environment;

      /** \brief Default density applied to the items. */
      double m_default_density;

      /** \brief A set of regions where the density of the environment differs
          from m_default_density. */
      std::list<density_rectangle*> m_density_rectangle;

      /** \brief Value under which a coordinate is considered as zero. */
      coordinate_type m_position_epsilon;

      /** \brief Value under which the speed is considered as zero. */
      speed_type m_speed_epsilon;

      /** \brief Value under which the angular speed is considered as zero. */
      double m_angular_speed_epsilon;

      /** \brief Value under which the acceleration is considered as zero. */
      force_type m_acceleration_epsilon;

    }; // class world
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_WORLD_HPP__
