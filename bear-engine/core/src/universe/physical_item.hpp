/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The common interface for all the physical items living in our
 *        universe.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_PHYSICAL_ITEM_HPP__
#define __UNIVERSE_PHYSICAL_ITEM_HPP__

#include "universe/const_item_handle.hpp"
#include "universe/physical_item_state.hpp"
#include "universe/environment_type.hpp"
#include "universe/world_progress_structure.hpp"
#include "universe/forced_movement/forced_movement.hpp"

#include <claw/math.hpp>

#include <list>
#include <vector>

namespace bear
{
  namespace universe
  {
    class collision_align_policy;
    class collision_info;
    class item_handle;
    class base_link;
    class world;

    /**
     * \brief The common interface for all the physical items living in our
     *        universe.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT physical_item :
      public physical_item_state
    {
    public:
      /** \brief The type of the class that stores our fields. */
      typedef physical_item_state super;

      /** \brief The type of the list in which we store the links. */
      typedef std::list<base_link*> link_list_type;

      /** \brief Const iterator on the list of links. */
      typedef link_list_type::const_iterator const_link_iterator;

      /** \brief The list of items passed to get_dependent_items(). */
      typedef std::vector<physical_item*> item_list;

    private:
      /** \brief A list of handles. */
      typedef std::list<item_handle*> handle_list;

      /** \brief A list of const_handles. */
      typedef std::list<const_item_handle*> const_handle_list;

    public:
      physical_item();
      physical_item( const physical_item& that );
      virtual ~physical_item();

      forced_movement& get_forced_movement();
      void set_forced_movement( const forced_movement& m );
      bool has_forced_movement() const;
      void clear_forced_movement();

      void set_movement_reference( const physical_item* item );
      const physical_item* get_movement_reference() const;

      bool is_linked_to( const physical_item& item ) const;
      bool is_linked_to( const physical_item& item, std::size_t id ) const;

      time_type get_age() const;

      virtual void get_dependent_items( item_list& d ) const;

      virtual void time_step( time_type elapsed_time );

      virtual void enters_active_region();
      virtual void leaves_active_region();

      virtual void move( time_type elapsed_time );
      virtual void collision( collision_info& info );

      bool collides_with( const physical_item& that ) const;

      // public only for world
      void set_owner(world& owner);
      void quit_owner();
      world_progress_structure& get_world_progress_structure();
      const world_progress_structure& get_world_progress_structure() const;
      // -end- public only for world

      // public only for base_link
      void add_link( base_link& link );
      void remove_link( base_link& link );
      // -end- public only for base_link

      const_link_iterator links_begin() const;
      const_link_iterator links_end() const;
      void remove_all_links();

      // public only for item_handle
      void add_handle( item_handle* h ) const;
      void add_handle( const_item_handle* h ) const;
      void remove_handle( item_handle* h ) const;
      void remove_handle( const_item_handle* h ) const;
      // -end- public only for item_handle

      void adjust_cinetic();

      bool has_owner() const;
      world& get_owner() const;

      bool default_collision( const collision_info& info );
      bool default_collision
        ( const collision_info& info, const collision_align_policy& policy );

      bool interesting_collision( const physical_item& that ) const;

    protected:
      void default_move( time_type elapsed_time );

      bool collision_align_left( const collision_info& info );
      bool collision_align_right( const collision_info& info );
      bool collision_align_top( const collision_info& info );
      bool collision_align_bottom( const collision_info& info );
      bool collision_middle( const collision_info& info );

      bool collision_align_left
        ( const collision_info& info, const collision_align_policy& policy );
      bool collision_align_right
        ( const collision_info& info, const collision_align_policy& policy );
      bool collision_align_top
        ( const collision_info& info, const collision_align_policy& policy );
      bool collision_align_bottom
        ( const collision_info& info, const collision_align_policy& policy );
      bool collision_middle
        ( const collision_info& info, const collision_align_policy& policy );

      bool collision_align_left
        ( const collision_info& info, const position_type& pos );
      bool collision_align_right
        ( const collision_info& info, const position_type& pos );
      bool collision_align_top
        ( const collision_info& info, const position_type& pos );
      bool collision_align_bottom
        ( const collision_info& info, const position_type& pos );

      bool collision_align_left
        ( const collision_info& info, const position_type& pos,
          const collision_align_policy& policy );
      bool collision_align_right
        ( const collision_info& info, const position_type& pos,
          const collision_align_policy& policy );
      bool collision_align_top
        ( const collision_info& info, const position_type& pos,
          const collision_align_policy& policy );
      bool collision_align_bottom
        ( const collision_info& info, const position_type& pos,
          const collision_align_policy& policy );

      bool is_only_in_environment(const universe::environment_type e) const;
      bool is_in_environment(const universe::environment_type e) const;

    private:
      bool
      collision_align_at
      ( physical_item_state& that, const position_type& pos ) const;

      void remove_all_handles() const;

      coordinate_type get_align_epsilon() const;

      virtual bool do_interesting_collision( const physical_item& that ) const;

      // not implemented
      physical_item& operator=(const physical_item&);

    private:
      /** \brief Forced movement applied to this item. */
      forced_movement m_forced_movement;

      /** \brief The item relative to which I move. */
      const_item_handle m_movement_reference;

      /** \brief The links concerning the item. */
      link_list_type m_links;

      /** \brief The handles on me. */
      mutable handle_list m_handles;

      /** \brief The handles on me. */
      mutable const_handle_list m_const_handles;

      /** \brief The world in which this item lives. */
      world* m_owner;

      /** \brief A structure used by the world for the progress of the item. */
      world_progress_structure m_world_progress_structure;

      /** \brief The age of this item. */
      time_type m_age;

    }; // class physical_item
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_PHYSICAL_ITEM_HPP__
