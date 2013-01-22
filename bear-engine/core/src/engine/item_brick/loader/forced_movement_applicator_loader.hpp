/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The item loader for forced_movement_applicator.
 * \author Julien Jorge
 */
#ifndef __ENGINE_FORCED_MOVEMENT_APPLICATOR_LOADER_HPP__
#define __ENGINE_FORCED_MOVEMENT_APPLICATOR_LOADER_HPP__

#include <string>

#include "engine/class_export.hpp"

#include "engine/item_brick/loader/forced_movement_loader.hpp"
#include "universe/types.hpp"

namespace bear
{
  namespace engine
  {
    class forced_movement_applicator;

    /**
     * \brief The item loader for the forced_movement_applicator.
     *
     * The custom fields of this class are:
     *
     *  - \a actor: (item list) \b the item to which the movement is applied
     *    (default = none),
     *  - \a movement: (item) \b [required] \c an item having a movement
     *    applied to him, the movement that will be applied to the actor,
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
    class ENGINE_EXPORT forced_movement_applicator_loader:
      public forced_movement_loader
    {
    public:
      explicit forced_movement_applicator_loader
        ( forced_movement_applicator& item );

      virtual forced_movement_applicator_loader* clone() const;

      bool set_field
      ( const std::string& name, const std::vector<base_item*>& value );

    private:
      /** \brief The item loaded by this loader. */
      forced_movement_applicator& m_item;

    }; // class forced_movement_applicator_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_FORCED_MOVEMENT_APPLICATOR_LOADER_HPP__
