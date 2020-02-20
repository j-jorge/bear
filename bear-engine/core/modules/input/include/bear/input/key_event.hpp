/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a keyboard key event.
 * \author Julien Jorge
 */
#ifndef __INPUT_KEY_EVENT_HPP__
#define __INPUT_KEY_EVENT_HPP__

#include "bear/input/key_info.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief A key event.
     * \author Julien Jorge
     */
    class INPUT_EXPORT key_event
    {
    public:
      /** \brief Different types of events. */
      enum event_type
        {
          key_event_pressed,
          key_event_released,
          key_event_maintained,
          key_event_character,
          key_event_unknown
        }; // enum event_type

    public:
      key_event( event_type t, const key_info& info );

      event_type get_type() const;
      const key_info& get_info() const;

    private:
      /** \brief The type of the event. */
      event_type m_type;

      /** \brief Some informations about the concerned key. */
      key_info m_info;

    }; // class key_event

  } // namespace input
} // namespace bear

#endif // __INPUT_KEY_EVENT_HPP__
