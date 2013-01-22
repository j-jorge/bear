/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A control for editing a field that takes its value in a set.
 * \author Julien Jorge
 */
#ifndef __BF_BOOL_EDIT_HPP__
#define __BF_BOOL_EDIT_HPP__

#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/checkbox.h>

namespace bf
{
  /**
   * \brief A control for editing a field that can takes ites value in a set.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT bool_edit:
    public base_edit<bool_type>,
    public wxCheckBox
  {
  public:
    /** \brief The type of the value of the edited field. */
    typedef bool_type value_type;

  public:
    bool_edit
    ( wxWindow& parent, const value_type& v = default_value<bool_type>::get() );

    bool validate();

  private:
    void init();
    void value_updated();

    void on_change( wxCommandEvent& event );

  }; // class bool_edit
} // namespace bf

#endif // __BF_BOOL_EDIT_HPP__
