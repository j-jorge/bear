/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of the bf::item_field_edit
 *        class.
 * \author Julien Jorge
 */
#include "bf/human_readable.hpp"

/*----------------------------------------------------------------------------*/
template<typename T>
const wxEventType
bf::set_field_value_event<T>::set_field_value_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param field_name The name of the field.
 * \param val The new value of the field.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
template<typename T>
bf::set_field_value_event<T>::set_field_value_event
( const std::string& field_name, const T& val, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_field_name(field_name), m_value(val)
{

} // set_field_value_event::set_field_value_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
template<typename T>
wxEvent* bf::set_field_value_event<T>::Clone() const
{
  return new set_field_value_event<T>(*this);
} // set_field_value_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the field to set.
 */
template<typename T>
const std::string& bf::set_field_value_event<T>::get_field_name() const
{
  return m_field_name;
} // set_field_value_event::get_field_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the new value of the field.
 */
template<typename T>
const T& bf::set_field_value_event<T>::get_value() const
{
  return m_value;
} // set_field_value_event::get_value()




/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a field in a string.
 * \param field_name The name of the field for which we want the value.
 * \param item The item in which the value is taken.
 */
template<typename Type>
wxString bf::item_field_edit::value_to_text_converter<Type>::operator()
  ( const std::string& field_name, const item_instance& item ) const
{
  Type val;
  item.get_value( field_name, val );

  return human_readable<Type>::convert( val );
} // item_field_edit::value_to_text_converter::operator()()
