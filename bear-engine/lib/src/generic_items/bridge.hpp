/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that represents a bridge.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_BRIDGE_HPP__
#define __BEAR_BRIDGE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_friction.hpp"
#include "engine/item_brick/item_with_restricted_z_collision.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"
#include "engine/item_brick/z_shift.hpp"
#include "engine/export.hpp"

#include "generic_items/class_export.hpp"
#include "generic_items/reference_item.hpp"

#include "universe/derived_item_handle.hpp"

#include <list>

namespace bear
{
  /**
   * \brief An item that represents a bridge.
   * \author Sebastien Angibaud
   * The fields of this item are
   *  - any field supported by the parent classes.
   */
  class GENERIC_ITEMS_EXPORT bridge:
    public engine::item_with_decoration
    < engine::basic_renderable_item
      < engine::item_with_friction
	< engine::item_with_restricted_z_collision
	  < engine::base_item > > > >
  {
    DECLARE_BASE_ITEM(bridge);
    
  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_decoration
    < engine::basic_renderable_item
      < engine::item_with_friction
	< engine::item_with_restricted_z_collision
	  < engine::base_item > > > > super;
  
    /** \brief A class that stores informations about an item on bridge. */
    class item_on_bridge
    {
    public:
      /** \brief The type of the item handle. */
      typedef universe::derived_item_handle< engine::base_item > item_handle;
    
    public:
      item_on_bridge();
      ~item_on_bridge();
    
      void set_item_on_bridge(engine::base_item* item);
      reference_item* get_reference_item();
      const reference_item* get_reference_item() const;
      const item_handle& get_item() const;
    
      bool operator<( const item_on_bridge& that ) const;
    
    private:
      /* \brief The item on the bridge. */
      item_handle m_item;
    
      /* \brief The reference item under the item. */
      reference_item* m_reference_item;
    }; // class item_on_bridge
    
    /** \brief The type of the list in which we store items in bridge. */
    typedef std::list< item_on_bridge > items_list;
    
    /** \brief The type of a item in the list. */
    typedef std::list< item_on_bridge >::iterator items_list_iterator;
    
    /** \brief The type of a item in the list. */
    typedef std::list< item_on_bridge >::const_iterator
    items_list_const_iterator;
    
  public:
    bridge();
    
    bool set_item_field( const std::string& name, base_item* value );
    bool set_real_field( const std::string& name, double value );
    void progress( bear::universe::time_type elapsed_time );
    void on_enters_layer();
    void get_visual( std::list<engine::scene_visual>& visuals ) const;
    bool is_valid() const;
    
  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    void add_bridge_visuals
    ( std::list<engine::scene_visual>& visuals,
      const base_item* start, const base_item* end,
      const universe::coordinate_type& unity,
      universe::coordinate_type& length, 
      universe::position_type& previous_pos ) const;
    void add_bridge_visual
    ( std::list<engine::scene_visual>& visuals,
      const universe::position_type& left_pos,
      const universe::position_type& right_pos ) const;
    
    void collision_as_bridge
    ( engine::base_item& that, universe::collision_info& info );
    void collision( engine::base_item& that, universe::collision_info& info );
    bool align_on_bridge
    ( engine::base_item& that, universe::collision_info& info );
    
    universe::position_type compute_align_position
    ( engine::base_item& that, const universe::position_type& previous_pos,
      const universe::position_type& next_pos) const;
    void compute_neighboor
    ( const universe::position_type& pos, 
      universe::position_type& previous_pos,
      universe::position_type& next_pos) const;
    void search_or_create_item(engine::base_item& that);
    void update_items_list();
    bool check_erase_item
    (items_list_iterator it, const universe::position_type& previous_pos,
     const universe::position_type& next_pos) const;
    universe::coordinate_type compute_giving_way
    (const base_item& item) const;
    bool check_item
    ( const universe::position_type& pos,
      const universe::position_type& previous_pos,
      const universe::position_type& next_pos,
      universe::coordinate_type margin ) const;
    void get_dependent_items( std::list<physical_item*>& d ) const;
    
    void apply_angle_to
    ( engine::base_item& that, const universe::collision_info& info) const;
    universe::coordinate_type get_bridge_length() const;
    void create_extremities( base_item* & item, base_item* reference);
    
  private:
    /** \brief The list of collised items. */
    items_list m_items;
    
    /** \brief The How far the bridge can fall. */
    universe::coordinate_type m_max_fall;
    
    /** \brief Reference item on top left corner for line computing. */
    base_item* m_top_left_ref;
    
    /** \brief Reference item on top right corner for line computing. */
    base_item* m_top_right_ref;
    
    /** \brief Given reference item on top left corner. */
    base_item* m_top_left_reference;
    
    /** \brief Given reference item on top right corner. */
    base_item* m_top_right_reference;
    
    /** \brief The object that will manage the z position of the colliding
        items. */
    engine::z_shift m_z_shift;

    /** \brief The width of the surface of the bridge. */
    static const universe::coordinate_type s_line_width;
  }; // class bridge
} // namespace bear

#endif // __BEAR_BRIDGE_HPP__
