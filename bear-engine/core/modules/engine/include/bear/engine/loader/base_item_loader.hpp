/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item loader for the base item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_ITEM_LOADER_HPP__
#define __ENGINE_BASE_ITEM_LOADER_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/engine/loader/item_loader_base.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief The item loader for the base item.
     * \author Julien Jorge
     *  - \a position.left: \c The x-coordinate of the left edge,
     *  - \a position.bottom: \c The y-coordinate of the bottom egde,
     *  - \a position.depth: \c The z-coordinate,
     *  - \a size.height: \c The height of the box,
     *  - \a size.width: \c The width of the box,
     *  - \a mass: \c Mass of the item,
     *  - \a density: \c Density of the item,
     *  - \a elasticty: \c Elasticity of the item,
     *  - \a hardness: \c Hardness of the item,
     *  - \a system_angle: \c Angle of the item,
     *  - \a speed.x: \c The initial speed on the x-axis,
     *  - \a speed.y: \c The initial speed on the y-axis,
     *  - \a angular_speed: \c The initial angular speed,
     *  - \a artificial: \c Tell if the item is artificial,
     *  - \a can_move_items: \c Tell if the item can move other items,
     *  - \a phantom: \c Tell if the item is a phantom,
     *  - \a global: \c Tell if the item is global,
     *  - \a fixed.x: \c Tell if the item is fixed on x-coordinate,
     *  - \a fixed.y: \c Tell if the item is fixed on y-coordinate,
     *  - \a free_system: \c Tell if the system angle of the item is not
     *    modified by the collisions.
     */
    class ENGINE_EXPORT base_item_loader:
      public item_loader_base
    {
    public:
      explicit base_item_loader( base_item& item );

      virtual base_item_loader* clone() const;

      virtual bool set_field( const std::string& name, int value );
      virtual bool set_field( const std::string& name, double value );
      virtual bool set_field( const std::string& name, bool value );

    private:
      /** \brief The item loaded by this loader. */
      base_item& m_item;

    }; // class base_item_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_ITEM_LOADER_HPP__
