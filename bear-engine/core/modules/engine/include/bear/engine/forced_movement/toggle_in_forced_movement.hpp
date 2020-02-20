/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The toggle_in_forced_movement class is used to insert a toggle in a
 *        forced movement.
 * \author Julien Jorge
 */
#ifndef __ENGINE_TOGGLE_IN_FORCED_MOVEMENT_HPP__
#define __ENGINE_TOGGLE_IN_FORCED_MOVEMENT_HPP__

#include "bear/universe/forced_movement/base_forced_movement.hpp"
#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class with_toggle;

    /**
     * \brief The toggle_in_forced_movement class is used to insert a toggle in
     *        a forced movement.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT toggle_in_forced_movement:
      public universe::base_forced_movement
    {
    public:
      toggle_in_forced_movement();

      universe::base_forced_movement* clone() const;

      void use_toggle_delay( bool b );

      bool is_finished() const;

    private:
      void do_init();
      universe::time_type do_next_position( universe::time_type elapsed_time );

      with_toggle* get_toggle() const;

    private:
      /** \brief Tell if the duration of the forced movement in the duration of
          the toggle. */
      bool m_use_toggle_delay;

      /** \brief The elapsed time since the creation of the movement. */
      universe::time_type m_elapsed_time;

    }; // class toggle_in_forced_movement
  } // namespace engine
} // namespace bear

#endif // __ENGINE_TOGGLE_IN_FORCED_MOVEMENT_HPP__
