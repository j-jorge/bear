/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item with a boolean associated to each side.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ACTIVABLE_SIDES_HPP__
#define __ENGINE_ACTIVABLE_SIDES_HPP__

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item with a boolean associated to each side.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT activable_sides
    {
    public:
      activable_sides();

      bool left_side_is_active() const;
      bool right_side_is_active() const;
      bool top_side_is_active() const;
      bool bottom_side_is_active() const;

      void set_left_side_activation(bool a);
      void set_right_side_activation(bool a);
      void set_top_side_activation(bool a);
      void set_bottom_side_activation(bool a);

    private:
      /** \brief Tell if the left side is solid. */
      bool m_left_side_is_active;

      /** \brief Tell if the right side is solid. */
      bool m_right_side_is_active;

      /** \brief Tell if the top side is solid. */
      bool m_top_side_is_active;

      /** \brief Tell if the bottom side is solid. */
      bool m_bottom_side_is_active;

    }; // class activable_sides
  } // namespace engine
} // namespace bear

#endif // __ENGINE_ACTIVABLE_SIDES_HPP__
