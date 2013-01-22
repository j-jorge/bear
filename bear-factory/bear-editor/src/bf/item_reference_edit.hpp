/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A control for choosing an item reference.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_REFERENCE_EDIT_HPP__
#define __BF_ITEM_REFERENCE_EDIT_HPP__

#include "bf/simple_edit.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/combobox.h>

namespace bf
{
  /**
   * \brief A control for choosing an item reference.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT item_reference_edit:
    public simple_edit<item_reference_type>,
    public wxComboBox
  {
 public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_PATTERN_TEXT
      }; // enum control_id

    /** The type of the current class. */
    typedef item_reference_edit self_type;

    /** \brief The type of the value of the edited field. */
    typedef simple_edit<item_reference_type>::value_type value_type;

  public:
    item_reference_edit
    ( wxWindow& parent, const wxArrayString& choices, const value_type& v );

    bool validate();

  private:
    void value_updated();
    void fill_id_list();

    void on_pattern_change( wxCommandEvent& event );

  private:
    /** \brief The choices of identifiers. */
    const wxArrayString m_choices;

    /** \brief The last value that produces a valid entry. */
    wxString m_last_valid_value;

    DECLARE_EVENT_TABLE()

  }; // class item_reference_edit
} // namespace bf

#endif // __BF_ITEM_REFERENCE_EDIT_HPP__
