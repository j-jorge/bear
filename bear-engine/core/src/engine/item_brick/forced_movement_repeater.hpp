/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that can give a forced movement to several clones of a given
 *        item and place them at a constant time interval from each other.
 * \author Julien Jorge
 */
#ifndef __ENGINE_FORCED_MOVEMENT_REPEATER_HPP__
#define __ENGINE_FORCED_MOVEMENT_REPEATER_HPP__

#include "engine/class_export.hpp"
#include "universe/forced_movement/forced_movement.hpp"

#include <vector>

namespace bear
{
  namespace engine
  {
    class base_item;
    class forced_movement_repeater_loader;

    /**
     * \brief An item that can give a forced movement to several clones of a
     *        given item and place them at a constant time interval from each
     *        other.
     *
     * The custom fields of this class are:
     *
     *  - \a actor: (item) the item to which and to the clones of which
     *    the movement is applied (default = none),
     *  - \a count: (unsigned int) the number of clones to create (default = 0),
     *  - \a delay: (real) the time interval between the placement of two
     *    clones,
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
    class ENGINE_EXPORT forced_movement_repeater
    {
      friend class forced_movement_repeater_loader;

    public:
      forced_movement_repeater();
      ~forced_movement_repeater();

      bool is_valid() const;

      void give_movement();

    private:
      /** \brief The item to clone and to move. */
      base_item* m_actor;

      /** \brief The movement to apply. */
      universe::forced_movement m_movement;
      
      /** \brief The number of clones to create. */
      std::size_t m_count;

      /** \brief The time difference between the positions of the clones. */
      universe::time_type m_delay;

    }; // class forced_movement_repeater
  } // namespace engine
} // namespace bear

#endif // __ENGINE_FORCED_MOVEMENT_REPEATER_HPP__
