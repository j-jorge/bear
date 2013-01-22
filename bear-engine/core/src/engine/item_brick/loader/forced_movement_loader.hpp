/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The item loader for forced_movement.
 * \author Julien Jorge
 */
#ifndef __ENGINE_FORCED_MOVEMENT_LOADER_HPP__
#define __ENGINE_FORCED_MOVEMENT_LOADER_HPP__

#include <string>

#include "engine/class_export.hpp"

#include "engine/loader/item_loader_base.hpp"

#include "universe/forced_movement/forced_movement.hpp"
#include "universe/types.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The item loader for the forced_movement.
     *
     * The custom fields of this class are:
     *
     *  - \a movement: (item) \b [required] \c an item having a movement
     *    applied to him,
     *  - \a auto_remove: (bool) \c tell if the movement removes himself
     *    from the moving item when finished (default = false).
     *  - \a auto_remove: (bool) Tell if the angle of moving items are
     *    updated according to their angular speed (default = false).
     *  - ratio.x: real, the ratio of width to compute actor's position
     *    (0 for left side, 0.5 for middle, 1 for right side). (default = 0.5)
     *  - ratio.y: real, the ratio of height to compute actor's position
     *    (0 for bottom side, 0.5 for middle, 1 for top side). (default = 0.5)
     *  - gap.x: real, The x-gap to applied. (default = 0)
     *  - gap.y: real, The y-gap to applied. (default = 0)
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT forced_movement_loader:
      public item_loader_base
    {
    public:
      forced_movement_loader
        ( const std::string& prefix, universe::forced_movement& mvt );

      virtual forced_movement_loader* clone() const;

      bool set_field( const std::string& name, bool value );
      bool set_field( const std::string& name, double value );
      bool set_field( const std::string& name, base_item* value );

    private:
      /** \brief The item loaded by this loader. */
      universe::forced_movement& m_forced_movement;

    }; // class forced_movement_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_FORCED_MOVEMENT_LOADER_HPP__
