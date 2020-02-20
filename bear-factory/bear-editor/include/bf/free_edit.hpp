/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A control for editing a field that can take any value.
 * \author Julien Jorge
 */
#ifndef __BF_FREE_EDIT_HPP__
#define __BF_FREE_EDIT_HPP__

#include "bf/simple_edit.hpp"
#include "bf/default_value.hpp"

#include <wx/textctrl.h>

namespace bf
{
  /**
   * \brief A control for editing a field that can take any value.
   * \author Julien Jorge
   */
  template<typename Type>
  class free_edit:
    public simple_edit<Type>,
    public wxTextCtrl
  {
  public:
    /** The type of the current class. */
    typedef free_edit<Type> self_type;

    /** \brief The type of the value of the edited field. */
    typedef typename simple_edit<Type>::value_type value_type;

  public:
    explicit free_edit( wxWindow& parent );
    free_edit( wxWindow& parent, const value_type& v );

    bool validate();

  private:
    void value_updated();

  }; // class free_edit
} // namespace bf

#include "bf/impl/free_edit.tpp"

#endif // __BF_FREE_EDIT_HPP__
