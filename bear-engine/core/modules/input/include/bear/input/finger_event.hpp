/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a finger finger event.
 * \author Julien Jorge
 */
#ifndef __INPUT_FINGER_EVENT_HPP__
#define __INPUT_FINGER_EVENT_HPP__

#include "bear/input/types.hpp"

#include "bear/input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief A finger event.
     * \author Julien Jorge
     */
    class INPUT_EXPORT finger_event
    {
    public:
      /** \brief Different types of events. */
      enum event_type
        {
          finger_event_pressed,
          finger_event_released,
          finger_event_motion
        }; // enum event_type

    public:
      static finger_event create_pressed_event
        ( const position_type& p, int finger_id );
      static finger_event create_released_event
        ( const position_type& p, int finger_id );
      static finger_event create_motion_event
        ( const position_type& p, int finger_id, const position_type& d );

      int get_finger_id() const;
      event_type get_type() const;
      const position_type& get_position() const;
      const position_type& get_distance() const;

      finger_event at_position( const position_type& p ) const;

    private:
      finger_event( event_type t, int finger_id );

    private:
      /** \brief The type of the event. */
      event_type m_type;
      
      /** \brief The identifier of the finger emitting this event. */
      int m_finger_id;

      /** \brief The position where the event occurred. */
      position_type m_position;

      /** \brief The distance of the motion. */
      position_type m_distance;

    }; // class finger_event

  } // namespace input
} // namespace bear

#endif // __INPUT_FINGER_EVENT_HPP__
