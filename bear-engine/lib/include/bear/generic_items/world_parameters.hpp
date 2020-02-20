/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class sets the parameters of universe::world then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_WORLD_PARAMETERS_HPP__
#define __BEAR_WORLD_PARAMETERS_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/universe/environment_type.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class sets the parameters of universe::world then die.
   *
   * The fields of this item are
   *  - \a gravity_x: (real) \c the x value of the gravity (default = 0),
   *  - \a gravity_y: (real) \c the y value of the gravity (default = 9.81),
   *  - \a speed_epsilon_x: (real) \c the value under which the speed is
   *    considered to be equal to zero on the x axis (default = 0.01),
   *  - \a speed_epsilon_y: (real) \c the value under which the speed is
   *    considered to be equal to zero on the y axis (default = 0.01),
   *  - \a angular_speed_epsilon: (real) \c the value under which the angular
   *    speed is considered to be equal to zero (default = 0.01),
   *  - \a unit: (real) \c number of units of the world in a meter
   *    (default = 3000),
   *  - \a default_friction: (real) default friction applied to the items
   *    (default = 0.99).
   *  - \a default_density: (real) default density of the world
   *    (default = 0).
   *  - \a default_environment: (string) default environment of the world
   *    (default = "air_environment").
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT world_parameters:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(world_parameters);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    world_parameters();

    bool set_real_field( const std::string& name, double value );
    bool set_string_field( const std::string& name, const std::string& value);

    void build();

  private:
    /** \brief The gravity. */
    universe::force_type m_gravity;

    /** \brief The value under which the speed considered to be equal to
        zero. */
    universe::speed_type m_speed_epsilon;

    /** \brief Value under which the angular speed is considered as zero. */
    double m_angular_speed_epsilon;

    /** \brief The unit of the world. */
    universe::coordinate_type m_unit;

    /** \brief Default friction applied to the items. */
    double m_default_friction;

    /** \brief Default density applied to the items. */
    double m_default_density;

    /** \brief Default environment of the world. */
    universe::environment_type m_default_environment;

  }; // class world_parameters
} // namespace bear

#endif // __BEAR_WORLD_PARAMETERS_HPP__
