/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A temporary decoration with little effects.
 * \author Julien Jorge
 */
#ifndef __BEAR_DECORATIVE_EFFECT_HPP__
#define __BEAR_DECORATIVE_EFFECT_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/basic_renderable_item.hpp"
#include "bear/universe/derived_item_handle.hpp"

#include "bear/generic_items/class_export.hpp"
#include "bear/engine/export.hpp"

namespace bear
{
  /**
   * \brief A temporary decoration with little effects.
   * \author Julien Jorge
   *
   * This item has no custom fields.
   *
   */
  class GENERIC_ITEMS_EXPORT decorative_effect:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(decorative_effect);

  private:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

    /** \brief The type of the handle of the modified item. */
    typedef universe::derived_item_handle
      <engine::with_rendering_attributes, engine::base_item> handle_type;

  public:
    decorative_effect();

    void build();
    void progress( universe::time_type elapsed_time );
    bool set_bool_field( const std::string& name, bool value );
    bool set_item_field( const std::string& name, base_item* value );
    bool set_real_field( const std::string& name, double value );
    bool set_color_field( const std::string& name, visual::color value );

    void set_item
      ( engine::base_item* item, bool same_lifespan, bool restore = false,
        bool loop_back = false );

    void set_duration(universe::time_type d);
    universe::time_type get_duration() const;

    void resize_item( bool r );

    void set_size_factor_init(double f);
    void set_color_init(const visual::color& c);
    void set_angle_offset_init(double f);

    void set_size_factor_end(double f);
    void set_color_end(const visual::color& c);
    void set_angle_offset_end(double f);

    void set_size_factor(double init, double end);
    void set_color
      (const visual::color& init_color, const visual::color& end_color);
    void set_angle_offset(double init, double end);

  private:
    void apply_effect() const;

  private:
    /** \brief The initial rendering attributes, used for the reference. */
    visual::bitmap_rendering_attributes m_rendering_attributes;

    /** \brief The total duration of the effect. */
    universe::time_type m_duration;

    /** \brief Elapsed time since the begining of the effect. */
    universe::time_type m_elapsed_time;
    
    /** \brief Indicates that the effect has a loop back. */
    bool m_loop_back;

    /** \brief Initial factor applied to the size. */
    double m_size_factor_init;

    /** \brief Final factor applied to the size. */
    double m_size_factor_end;

    /** \brief Initial color. */
    visual::color m_color_init;

    /** \brief Final color. */
    visual::color m_color_end;

    /** \brief Initial offset applied to the angle. */
    double m_angle_offset_init;

    /** \brief Final offset applied to the angle. */
    double m_angle_offset_end;

    /** \brief The item to modify. */
    handle_type m_item;

    /** \brief Tell if m_item must be killed with *this. */
    bool m_same_lifespan;

    /** \brief Tell if the aspect of m_item must be restored at the end of the
        effect. */
    bool m_restore_at_end;

    /** \brief Tell to resize the item when the size in the rendering attributes
        is changed. */
    bool m_resize_item;

    /** \brief The initial size of the item. */
    universe::size_box_type m_initial_size;

  }; // class decorative_effect
} // namespace bear

#endif // __BEAR_DECORATIVE_EFFECT_HPP__
