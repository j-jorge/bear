/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An object that activates a toggle according to the mouse.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_MOUSE_OVER_MANAGER_HPP__
#define __BEAR_MOUSE_OVER_MANAGER_HPP__

#include "engine/export.hpp"
#include "engine/item_brick/item_with_input_listener.hpp"
#include "engine/base_item.hpp"
#include "generic_items/class_export.hpp"
#include "universe/item_handle.hpp"
#include "visual/animation.hpp"

namespace bear
{
  /**
   * \brief An object that place an animation under items according to 
   * the mouse.
   * \author Sebastien Angibaud
   *
   */
  class GENERIC_ITEMS_EXPORT mouse_over_manager :
    public engine::item_with_input_listener
  < engine::base_item >
  {
    DECLARE_BASE_ITEM(mouse_over_manager);

    /** \brief The type of the parent class. */
    typedef engine::item_with_input_listener
      < engine::base_item > super;

    /** \brief The type of an handle on a base_item. */
    typedef bear::universe::item_handle handle_type;

    /** \brief The type of a pair of a handle and boolean. */
    typedef std::pair< handle_type, bool > handle_pair_type;
    
    /** \brief A list of handle_type. */
    typedef std::list< handle_pair_type > handle_list;

  public:
    mouse_over_manager();
    ~mouse_over_manager();

    void progress( universe::time_type elapsed_time );
    bool set_item_list_field
      ( const std::string& name, 
        const std::vector<bear::engine::base_item*>& value);    
    bool set_animation_field
    ( const std::string& name, const bear::visual::animation& value );
    bool set_sample_field( const std::string& name, audio::sample* value );

  private:
    void get_visual( std::list<bear::engine::scene_visual>& visuals ) const;
    bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );
    void get_dependent_items
      ( std::list<bear::universe::physical_item*>& d ) const;
    void play_sound( handle_type item );

  private:
    /** \brief The item to scan. */
    handle_list m_items;
    
    /** \brief The position of the mouse. */
    bear::universe::position_type m_mouse_position;

    /** \brief The animation when the mouse in inside. */
    bear::visual::animation m_animation_on;      

    /** \brief The animation when the mouse is ouside. */
    bear::visual::animation m_animation_off; 

    /** \brief The sample played when the toggle is on. */
    audio::sample* m_sample;
  }; // class mouse_over_manager
} // namespace bear

#endif // __BEAR_MOUSE_OVER_MANAGER_HPP__
