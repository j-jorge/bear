/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A base class providing methods for simple types (integer_type,
 *        string_type and so on).
 * \author Julien Jorge
 */
#ifndef __BF_SIMPLE_EDIT_HPP__
#define __BF_SIMPLE_EDIT_HPP__

#include "bf/base_edit.hpp"

#include <wx/string.h>

namespace bf
{
  /**
   * \brief A base class providing methods for simple types (integer_type,
   *        string_type and so on).
   * \author Julien Jorge
   */
  template<typename Type>
  class simple_edit:
    public base_edit<Type>
  {
  public:
    /** \brief The type of the parent class. */
    typedef base_edit<Type> super;

    /** \brief The type of the value of the edited field. */
    typedef typename base_edit<Type>::value_type value_type;

  public:
    simple_edit( const value_type& v );

  protected:
    wxString value_to_string() const;
    bool value_from_string( const wxString& str );

  }; // class simple_edit
} // namespace bf

#include "bf/impl/simple_edit.tpp"

#endif // __BF_SIMPLE_EDIT_HPP__
