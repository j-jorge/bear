/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class representing a hidden_block.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_HIDDEN_BLOCK_HPP__
#define __BEAR_HIDDEN_BLOCK_HPP__

#include "generic_items/block.hpp"
#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A class representing a block that change its opacity
   * when it collides an other item.
   *
   * The custom fields of this class are:
   *  - \a opacity.passive: \c (real) The opacity when no item collides.
   *  - \a opacity.active: \c (real) The opacity when we an item collides.
   *  - \a transition_duration: \c (real) The duration for opacity changing.
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT hidden_block:
    public block
  {
    DECLARE_BASE_ITEM(hidden_block);

    typedef block super;

  public:
    hidden_block();
    void progress( universe::time_type elapsed_time );

    bool set_real_field( const std::string& name, double value );

  private:
    void collision( engine::base_item& that, universe::collision_info& info );
    void select_active_opacity();
    void select_passive_opacity();

  protected:
    void to_string( std::string& str ) const;

  private:
    /** \brief Indicates if an item collides. */
    bool m_active_state;

    /** \brief Indicates if there is a new collision during the iteration. */
    bool m_new_collision;

    /** \brief The duration of the change of opacity. */
    bear::universe::time_type m_transition_duration;

    /** \brief The time since the last modification. */
    bear::universe::time_type m_last_modification;

    /** \brief The opacity when no item collides. */
    double m_passive_opacity;

    /** \brief The opacity when an item collides. */
    double m_active_opacity;

  }; // class hidden_block
} // namespace bear

#endif // __BEAR_HIDDEN_BLOCK_HPP__
