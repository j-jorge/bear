/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A rectangle in which decorative sprite move according to a flow.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_DECORATIVE_FLOW_HPP__
#define __BEAR_DECORATIVE_FLOW_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"
#include "engine/item_brick/activable_sides.hpp"
#include "generic_items/class_export.hpp"
#include "engine/scene_visual.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A rectangle in which decorative sprite move according to a flow.
   * \author Sebastien Angibaud
   * The fields of this item are
   *  - \a min_speed.x : (real) \c The minimum speed on X-axis (default = 0).
   *  - \a min_speed.y : (real) \c The minimum speed on Y-axis (default = 0).
   *  - \a max_speed.x : (real) \c The maximum speed on X-axis (default = 1).
   *  - \a max_speed.y : (real) \c The maximum speed on Y-axis (default = 1).
   *  - \a items_per_second : (real) \c The number of decoration generated per
   *    second (default = 1).
   *  - any field supported by the parent classes.
   *
   */
  class GENERIC_ITEMS_EXPORT decorative_flow:
    public engine::item_with_decoration
    < engine::basic_renderable_item<engine::base_item> >,
    public engine::activable_sides
  {
    DECLARE_BASE_ITEM(decorative_flow);

  private:
    /** \brief The type of the parent class. */
    typedef engine::item_with_decoration
      < engine::basic_renderable_item<engine::base_item> > super;

  public:
    decorative_flow();

    virtual void progress( universe::time_type elapsed_time );
    void build();
    virtual bool set_real_field( const std::string& name, double value );

    virtual void get_visual( std::list<engine::scene_visual>& visuals ) const;

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    void update_decorations( universe::time_type elapsed_time );
    void create_decoration();
    void initiate_decoration();

  private:
    /* \brief The minimum speed of the flow. */
    universe::speed_type m_min_speed_flow;

    /* \brief The maximum speed of the flow. */
    universe::speed_type m_max_speed_flow;

    /* \brief The number of item per second in average. */
    double m_item_per_second;

    std::list< universe::position_type > m_decorations;

    /* \brief The size of decorations. */
    universe::size_box_type m_decoration_size;

  }; // class decorative_flow
} // namespace bear

#endif // __BEAR_DECORATIVE_FLOW_HPP__
