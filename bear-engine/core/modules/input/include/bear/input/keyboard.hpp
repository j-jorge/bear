/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a keyboard.
 * \author Julien Jorge
 */
#ifndef __INPUT_KEYBOARD_HPP__
#define __INPUT_KEYBOARD_HPP__

#include <string>
#include <vector>
#include <list>

#include "bear/charset/def.hpp"
#include "bear/input/key_event.hpp"
#include "bear/input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /**
     * \brief A class representing a keyboard.
     */
    class INPUT_EXPORT keyboard
    {
    public:
      /** \brief Iterator on the pressed keys. */
      typedef std::list<key_code>::const_iterator const_iterator;

      /** \brief The type of the list of the key events. */
      typedef std::list<key_event> event_list;

    public:
      static std::string get_name_of( key_code k );
      static  std::string get_translated_name_of( key_code k );
      static key_code get_key_named( const std::string& n );

      const_iterator begin() const;
      const_iterator end() const;
      const event_list& get_events() const;
      bool empty() const;

      // only for input::system
      void refresh();

    private:
      void refresh_events();
      void refresh_keys();

      key_code sdl_key_to_local( unsigned int sdl_val, bool shift,
                                 bool alt ) const;

    public:
#include "bear/input/key_codes.hpp"

    private:
      /** \brief Pressed keys. */
      std::list<key_code> m_pressed_keys;

      /** \brief Pressed keys. */
      event_list m_key_events;

    }; // class keyboard
  } // namespace input
} // namespace bear

#endif // __INPUT_KEYBOARD_HPP__
