/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::dialog_maker class.
 * \author Julien Jorge
 */

#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the dialog.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param f The edited field.
 * \param v The initial value.
 */
template<typename Control, typename Type>
typename bf::dialog_maker<Control, Type>::dialog_type*
bf::dialog_maker<Control, Type>::create
( wxWindow& parent, const wxString& type, const type_field& f,
  const value_type& v )
{
  return new dialog_type
    ( parent, std_to_wx_string(f.get_name()) + wxT(" (") + type + wxT(")"), v );
} // dialog_maker::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the dialog.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param f The edited field.
 * \param v The initial value.
 * \param pool The pool from which we take the images of the sprites.
 */
template<typename Control, typename Type>
typename bf::dialog_maker<Control, Type>::dialog_type*
bf::dialog_maker<Control, Type>::create
( wxWindow& parent, const wxString& type, const type_field& f,
  const value_type& v, const image_pool& pool )
{
  return new dialog_type
    ( parent, std_to_wx_string(f.get_name()) + wxT(" (") + type + wxT(")"), v,
      pool );
} // dialog_maker::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the dialog.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param f The edited field.
 * \param v The initial value.
 */
template<typename T, typename Type>
typename bf::dialog_maker<bf::set_edit<T>, Type>::dialog_type*
bf::dialog_maker<bf::set_edit<T>, Type>::create
( wxWindow& parent, const wxString& type, const type_field& f,
  const value_type& v )
{
  wxArrayString values;
  std::list<std::string> raw_values;
  typename std::list<std::string>::const_iterator it;

  f.get_set(raw_values);
  raw_values.sort();

  for (it=raw_values.begin(); it!=raw_values.end(); ++it)
    values.Add( std_to_wx_string(*it) );

  return new dialog_type
    ( parent,
      std_to_wx_string(f.get_name()) + wxT(" (") + type + wxT(")"), values, v );
} // dialog_maker::create() [set_edit]

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the dialog.
 * \param parent The parent window.
 * \param type The name of the edited type.
 * \param f The edited field.
 * \param v The initial value.
 */
template<typename T, typename Type>
typename bf::dialog_maker<bf::interval_edit<T>, Type>::dialog_type*
bf::dialog_maker<bf::interval_edit<T>, Type>::create
( wxWindow& parent, const wxString& type, const type_field& f,
  const value_type& v )
{
  typename T::value_type min, max;

  f.get_interval(min, max);

  return new dialog_type
    ( parent, std_to_wx_string(f.get_name()) + wxT(" (") + type + wxT(")"),
      T(min), T(max), v );
} // dialog_maker::create() [interval_edit]

