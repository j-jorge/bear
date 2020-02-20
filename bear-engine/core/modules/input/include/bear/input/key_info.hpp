/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Informations on a pressed key.
 * \author Julien Jorge
 */
#ifndef __INPUT_KEY_INFO_HPP__
#define __INPUT_KEY_INFO_HPP__

#include "bear/charset/def.hpp"
#include "bear/input/class_export.hpp"

namespace bear
{
  namespace input
  {
    /** \brief Code representing a key on the keyboard. */
    typedef unsigned int key_code;

    /**
     * \brief A keyboard entry, by the user.
     * \author Julien Jorge
     */
    class INPUT_EXPORT key_info
    {
    public:
      key_info();
      explicit key_info( key_code c );
      key_info( key_code c, charset::char_type s );

      static key_info from_char( charset::char_type s );

      bool operator==( const key_info& that ) const;
      bool operator<( const key_info& that ) const;

      key_code get_code() const;
      charset::char_type get_symbol() const;

      bool is_escape() const;
      bool is_enter() const;
      bool is_tab() const;
      bool is_control() const;
      bool is_shift() const;
      bool is_alt() const;
      bool is_up() const;
      bool is_down() const;
      bool is_left() const;
      bool is_right() const;
      bool is_home() const;
      bool is_end() const;
      bool is_delete() const;
      bool is_backspace() const;
      bool is_back() const;
      bool is_function(unsigned int f) const;

      bool is_printable() const;

    private:
      /** \brief A code representing the keyboard key. */
      key_code m_code;

      /** \brief The translated symbol. */
      charset::char_type  m_symbol;

    }; // class key_info
  } // namespace input
} // namespace bear

#endif // __INPUT_KEY_INFO_HPP__
