/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to transform a value into a human readable text.
 * \author Julien Jorge
 */
#ifndef __BF_HUMAN_READABLE_HPP__
#define __BF_HUMAN_READABLE_HPP__

#include "bf/color.hpp"
#include "bf/custom_type.hpp"
#include "bf/any_animation.hpp"
#include "bf/font.hpp"
#include "bf/sample.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/string.h>
#include <list>

namespace bf
{
  /**
   * \brief A class to transform a value into a human readable text.
   * \author Julien Jorge
   */
  template<typename Type>
  class human_readable
  {
  public:
    /** \brief The type of the value. */
    typedef Type value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable

  /**
   * \brief Specialisation for list of values.
   * \author Julien Jorge
   */
  template<typename T>
  class human_readable< std::list<T> >
  {
  public:
    /** \brief The type of the value. */
    typedef std::list<T> value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [list]

  /**
   * \brief Specialisation for bool_type "true"/"false".
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<bool_type>
  {
  public:
    /** \brief The type of the value. */
    typedef bool_type value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [bool_type]

  /**
   * \brief Specialisation for string_type: add quotes.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<string_type>
  {
  public:
    /** \brief The type of the value. */
    typedef string_type value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [string_type]

  /**
   * \brief Specialisation for sprite.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<sprite>
  {
  public:
    /** \brief The type of the value. */
    typedef sprite value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [sprite]

  /**
   * \brief Specialisation for animation.
   * \author Sebastien Angibaud
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<animation>
  {
  public:
    /** \brief The type of the value. */
    typedef animation value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [animation]

  /**
   * \brief Specialisation for animation_file_type.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<animation_file_type>
  {
  public:
    /** \brief The type of the value. */
    typedef animation_file_type value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [animation_file_type]

  /**
   * \brief Specialisation for any_animation.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<any_animation>
  {
  public:
    /** \brief The type of the value. */
    typedef any_animation value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [any_animation]

  /**
   * \brief Specialisation for sample.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<sample>
  {
  public:
    /** \brief The type of the value. */
    typedef sample value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [sample]

  /**
   * \brief Specialisation for font.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<font>
  {
  public:
    /** \brief The type of the value. */
    typedef font value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [font]

  /**
   * \brief Specialisation for color.
   * \author Sébastien Angibaud
   */
  template<>
  class BEAR_EDITOR_EXPORT human_readable<color>
  {
  public:
    /** \brief The type of the value. */
    typedef color value_type;

  public:
    static wxString convert( const value_type& v );

  }; // class human_readable [color]
} // namespace bf

#include "bf/impl/human_readable.tpp"

#endif // __BF_HUMAN_READABLE_HPP__
