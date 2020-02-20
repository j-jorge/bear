/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class observing the state of the finger.
 * \author Julien Jorge
 */
#ifndef __INPUT_FINGER_STATUS_HPP__
#define __INPUT_FINGER_STATUS_HPP__

#include "bear/input/finger.hpp"

#include "bear/input/class_export.hpp"

namespace bear
{
  namespace input
  {
    class input_listener;

    /**
     * \brief A class observing the state of the finger.
     * \author Julien Jorge
     */
    class INPUT_EXPORT finger_status
    {
    private:
      /** \brief The type of the event list of the finger. */
      typedef finger::event_list event_list;

    public:
      void read();

      void scan_inputs( input_listener& listener ) const;

    private:
      /** \brief The key events. */
      event_list m_events;

    }; // finger_status
  } // namespace input
} // namespace bear

#endif // __INPUT_FINGER_STATUS_HPP__
