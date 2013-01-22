/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that create dust decorative items and throws splinter
 *        decorative items.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_EXPLOSION_EFFECT_ITEM_HPP__
#define __BEAR_EXPLOSION_EFFECT_ITEM_HPP__

#include "engine/base_item.hpp"
#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief An item that create dust decorative items and throws splinter
   *        decorative items.
   * \author Sebastien Angibaud
   * The fields of this item are
   *  - \a dust_animations : (list of animations)
   * \c List of animations for dust items.
   *  - \a splinter_animations : (list of animation)
   * \c List of animations for splinter items.
   * \- \a force : (real)\c The force given at created splinter items
   * (default = 0).
   * \- \a dust_count : (unsigned int)\c Number of dust items to create
   * (default = 100).
   * \- \a splinter_count : (unsigned int)\c Number of splinter items to
   * create (default = 10).
   *  - any field supported by the parent classes.
   */
  class GENERIC_ITEMS_EXPORT explosion_effect_item:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(explosion_effect_item);

  private:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    explosion_effect_item();

    void progress( universe::time_type elapsed_time );

    bool set_animation_list_field
      ( const std::string& name, const std::vector<visual::animation>& value );
    bool set_real_field( const std::string& name, double value );
    bool set_u_integer_field( const std::string& name, unsigned int value );
    
    void add_splinter_animation(const visual::animation& anim);
    void add_dust_animation(const visual::animation& anim);
    void set_force(double force);
    void set_splinter_count( unsigned int splinter_count);
    void set_dust_count( unsigned int dust_count);

  private:
    void create_dust_items();
    void create_dust_item
    (unsigned int dust_index, bear::universe::position_type pos);
    void create_splinter_items();
    void create_splinter_item
    (unsigned int splinter_index, bear::universe::position_type pos);

  private:
    /* \brief List of animations for dust items. */
    std::vector<visual::animation> m_dust_animations;

    /* \brief List of animations for splinter items. */
    std::vector<visual::animation> m_splinter_animations;

    /* \brief The force given at created splinter items. */
    universe::coordinate_type m_force;

    /* \brief Number of dust items to create. */
    unsigned int m_dust_count;

    /* \brief Number of splinter items to create. */
    unsigned int m_splinter_count;
  }; // class explosion_effect_item
} // namespace bear

#endif // __BEAR_EXPLOSION_EFFECT_ITEM_HPP__
