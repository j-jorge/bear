/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that can give forced movement.
 * \author Sebastien Angibaud
 */
#ifndef __ENGINE_FORCED_MOVEMENT_APPLICATOR_HPP__
#define __ENGINE_FORCED_MOVEMENT_APPLICATOR_HPP__

#include "engine/class_export.hpp"
#include "universe/forced_movement/forced_movement.hpp"

#include "universe/derived_item_handle.hpp"

#include <vector>

namespace bear
{
  namespace engine
  {
    class base_item;
    class forced_movement_applicator_loader;

    /**
     * \brief An item that can give forced movement.
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
     * \author Sebastien Angibaud
     */
    class ENGINE_EXPORT forced_movement_applicator
    {
      friend class forced_movement_applicator_loader;

    private:
      typedef universe::derived_item_handle<base_item> handle_type;

    public:
      bool is_valid() const;

      const universe::forced_movement& get_movement() const;
      void give_movement();

    private:
      /** \brief The items to move. */
      std::vector<handle_type> m_actor;

      /** \brief The item from which we take the movement. */
      universe::forced_movement m_movement;

    }; // class forced_movement_applicator
  } // namespace engine
} // namespace bear

#endif // __ENGINE_FORCED_MOVEMENT_APPLICATOR_HPP__
