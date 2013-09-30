/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog that displays the controls for editing a given type of value.
 * \author Julien Jorge
 */
#ifndef __BF_VALUE_EDITOR_DIALOG_HPP__
#define __BF_VALUE_EDITOR_DIALOG_HPP__

#include "bf/image_pool.hpp"

#include <list>

#include <wx/dialog.h>
#include <wx/listbox.h>

namespace bf
{
  /**
   * \brief A dialog that displays the controls for editing a given type of
   *        value.
   * \author Julien Jorge
   */
  template<typename Editor, typename Type = typename Editor::value_type>
  class value_editor_dialog:
    public wxDialog
  {
  public:
    /** The type of the current class. */
    typedef value_editor_dialog<Editor, Type> self_type;

    /** \brief The type of the editor. */
    typedef Editor editor_type;

    /** \brief The type of the edited value. */
    typedef Type value_type;

  public:
    value_editor_dialog
    ( wxWindow& parent, const wxString& title, const value_type& v );

    value_editor_dialog
    ( wxWindow& parent, const wxString& title, const value_type& v,
      const image_pool& pool );

    value_editor_dialog
    ( wxWindow& parent, const wxString& title, const wxArrayString& values,
      const value_type& v );

    value_editor_dialog
    ( wxWindow& parent, const wxString& title, const value_type& min,
      const value_type& max, const value_type& v );

    const value_type& get_value() const;
    void set_value( const value_type& v );

    editor_type& get_editor();

  private:
    void init();
    void create_sizers();

    void on_ok(wxCommandEvent& event);

  private:
    /** \brief The control used for editing the value. */
    editor_type* m_editor;

    /** \brief The effective value */
    value_type m_value;

  }; // class value_editor_dialog

  /**
   * \brief Specialisation of value_editor_dialog for lists of values.
   * \author Julien Jorge
   */
  template<typename Editor, typename T>
  class value_editor_dialog< Editor, std::list<T> >:
    public wxDialog
  {
  public:
    /** The type of the current class. */
    typedef value_editor_dialog< Editor, std::list<T> > self_type;

    /** \brief The type of dialog displayed for editing the values. */
    typedef value_editor_dialog<Editor, T> dialog_type;

    /** \brief The type of the edited value. */
    typedef std::list<T> value_type;

  public:
    value_editor_dialog
    ( wxWindow& parent, const wxString& type, const value_type& v );

    value_editor_dialog
    ( wxWindow& parent, const wxString& type, const value_type& v,
      const image_pool& pool );

    value_editor_dialog
    ( wxWindow& parent, const wxString& type, const wxArrayString& values,
      const value_type& v );

    value_editor_dialog
    ( wxWindow& parent, const wxString& type, const T& min, const T& max,
      const value_type& v );

    const value_type& get_value() const;
    void set_value( const value_type& v );

  private:
    void init();

    void create_controls();
    void create_sizers();

    void fill();
    void edit_value( unsigned int index );

    void on_up( wxCommandEvent& event );
    void on_down( wxCommandEvent& event );
    void on_new( wxCommandEvent& event );
    void on_edit( wxCommandEvent& event );
    void on_delete( wxCommandEvent& event );

  private:
    /** \brief The dialog displayed when editing the value. */
    dialog_type* m_dialog;

    /** \brief The control that displays the list of values. */
    wxListBox* m_list;

    /** \brief The effective value */
    value_type m_value;

  }; // class value_editor_dialog
} // namespace bf

#include "bf/impl/value_editor_dialog.tpp"

#endif // __BF_VALUE_EDITOR_DIALOG_HPP__
