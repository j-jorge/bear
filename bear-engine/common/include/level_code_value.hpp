/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Codes describing level parameters in a compiled level file.
 * \author Julien Jorge
 */
#ifndef __BEAR_LEVEL_CODE_VALUE_HPP__
#define __BEAR_LEVEL_CODE_VALUE_HPP__

namespace bear
{
  /**
   * \brief Codes describing level parameters in a compiled level file.
   */
  class level_code_value
  {
  public:
    /** \brief The type of the codes. */
    typedef unsigned int value_type;

  public:
    /** \brief End of file. */
    static const value_type eof = 0;

    /** \brief Declare that an item will be defined. */
    static const value_type item_declaration       = 30;

    /** \brief Defition of the fields of a declared item. */
    static const value_type item_definition        = 31;

    /** \brief Known class of type bear::engine::base_item. */
    static const value_type base_item              = 32;

    /** \brief Dynamic field of type integer. */
    static const value_type field_int       = 40;

    /** \brief Dynamic field of type value_typeeger. */
    static const value_type field_u_int     = 41;

    /** \brief Dynamic field of type real. */
    static const value_type field_real      = 42;

    /** \brief Dynamic field of type boolean. */
    static const value_type field_bool      = 43;

    /** \brief Dynamic field of type string. */
    static const value_type field_string    = 44;

    /** \brief Dynamic field of type sprite. */
    static const value_type field_sprite    = 45;

    /** \brief Dynamic field of type animation. */
    static const value_type field_animation = 46;

    /** \brief Dynamic field of type item. */
    static const value_type field_item      = 47;

    /** \brief Dynamic field of type sample. */
    static const value_type field_sample    = 48;

    /** \brief Dynamic field of type font. */
    static const value_type field_font      = 49;

    /** \brief Dynamic field taking a list of values. */
    static const value_type field_list      = 50;

    /** \brief Layer. */
    static const value_type layer           = 70;

    /** \brief Dynamic field of type color. */
    static const value_type field_color     = 100;

    /** \brief Dynamic field of type easing. */
    static const value_type field_easing    = 101;

  }; // class level_code_value
} // namespace bear

#endif // __BEAR_LEVEL_CODE_VALUE_HPP__
