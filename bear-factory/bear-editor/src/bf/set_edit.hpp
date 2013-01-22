/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A control for editing a field that takes its value in a set.
 * \author Julien Jorge
 */
#ifndef __BF_SET_EDIT_HPP__
#define __BF_SET_EDIT_HPP__

#include "bf/simple_edit.hpp"
#include "bf/default_value.hpp"

#include <wx/choice.h>

namespace bf
{
  /**
   * \brief A control for editing a field that can takes ites value in a set.
   * \author Julien Jorge
   */
  template<typename Type>
  class set_edit:
    public simple_edit<Type>,
    public wxChoice
  {
  public:
    /** The type of the current class. */
    typedef set_edit<Type> self_type;

    /** \brief The type of the value of the edited field. */
    typedef typename simple_edit<Type>::value_type value_type;

  public:
    set_edit
    ( wxWindow& parent, const wxArrayString& choices,
      const value_type& v = default_value<Type>::get() );

    bool validate();

  private:
    void value_updated();

  }; // class set_edit
} // namespace bf

#include "bf/impl/set_edit.tpp"

#endif // __BF_SET_EDIT_HPP__
