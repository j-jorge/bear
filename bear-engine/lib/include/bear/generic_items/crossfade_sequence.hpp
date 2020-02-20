/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A decorative sprite.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_CROSSFADE_SEQUENCE_HPP__
#define __BEAR_CROSSFADE_SEQUENCE_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/with_rendering_attributes.hpp"
#include "bear/generic_items/class_export.hpp"
#include "bear/universe/derived_item_handle.hpp"
#include "bear/engine/scene_visual.hpp"
#include "bear/engine/export.hpp"

#include <vector>

namespace bear
{
  /**
   * \brief A rectangle in which we render a sequence of sprite with fade.
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT crossfade_sequence:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(crossfade_sequence);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /** \brief The type of the list of the handles on the items. */
    typedef std::vector
      < bear::universe::derived_item_handle
        < bear::engine::with_rendering_attributes >
      > item_list_type;

  public:
    crossfade_sequence();

    virtual void progress( universe::time_type elapsed_time );
    void build();

    virtual bool set_bool_field( const std::string& name, bool value );
    virtual bool set_real_field( const std::string& name, double value );
    virtual bool set_real_list_field
    ( const std::string& name, const std::vector<double>& value );
    virtual bool set_item_list_field
    ( const std::string& name, const std::vector<base_item*>& value );

    virtual bool is_valid() const;

  private:
    /** \brief The duration sequence. */
    std::vector<double> m_duration_sequence;

    /** \brief The list of items to reveal. */
    item_list_type m_items_sequence;

    /** \brief The duration since the beginning of the current sprite. */
    double m_last_sprite;

    /** \brief The index of the current sprite. */
    unsigned int m_index;

    /** \brief The duration of fade. */
    double m_fadeout_duration;

    /** \brief Indicates if there is a fadein for the first sprite. */
    bool m_fadein_for_first_sprite;

    /** \brief Indicates if there is a fadeout for the last sprite. */
    bool m_fadeout_for_last_sprite;

  }; // class crossfade_sequence
} // namespace bear

#endif // __BEAR_CROSSFADE_SEQUENCE_HPP__
