/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the bf::item_field_edit
 *        class.
 * \author Julien Jorge
 */
#include "bf/default_value.hpp"
#include "bf/dialog_maker.hpp"
#include "bf/free_edit.hpp"
#include "bf/human_readable.hpp"
#include "bf/item_class.hpp"
#include "bf/interval_edit.hpp"
#include "bf/set_edit.hpp"

#include <claw/assert.hpp>

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
} // item_field_edit::convert_value_to_text()




/*----------------------------------------------------------------------------*/
/**
 * \brief Get the common value, if any, of a field of all items in the group.
 * \param f The field for which we want the value.
 * \param val (out) The value.
 * \return true if all items has the same value for the given field.
 */
template<typename Type>
bool
bf::item_field_edit::get_common_value( const type_field& f, Type& val ) const
{
  CLAW_PRECOND( !empty() );

  wxString text_result;

  // The initialization turns off the following warning:
  //   "'result' may be used uninitialized in this function"
  // It occurs on the "for" loop below but should not. Actually, result is
  // initialized in the first "if/else" below.
  Type result( default_value<Type>::get() );

  item_iterator it=begin();

  if ( it->has_value(f) )
    {
      it->get_value(f.get_name(), result);
      text_result = human_readable<Type>::convert(result);
    }
  else
    {
      const std::string text(it->get_class().get_default_value(f.get_name()));
      text_result = std_to_wx_string( text );
      std::istringstream iss(text);
      stream_conv<Type>::read(iss, result);
    }

  for (++it; it!=end(); ++it)
    if ( it->has_value(f) )
      {
        Type v;
        it->get_value( f.get_name(), v );

        if ( (v != result)
             && (human_readable<Type>::convert(v) != text_result) )
          return false;
      }
    else
      {
        const wxString as_text =
          std_to_wx_string( it->get_class().get_default_value(f.get_name()) );

        if (as_text != text_result)
          return false;
      }

  val = result;
  return true;
} // item_field_edit::get_common_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the adequate dialog for editing a given field.
 * \param f The type of the field we are editing.
 * \param type The name of the type of the field.
 */
template<typename Type>
void bf::item_field_edit::show_simple_property_dialog
( const type_field& f, const wxString& type )
{
   switch ( f.get_range_type() )
    {
    case type_field::field_range_free:
      show_property_dialog< free_edit<Type> >(f, type);
      break;
    case type_field::field_range_set:
      show_property_dialog< set_edit<Type> >(f, type);
      break;
    case type_field::field_range_interval:
      show_property_dialog< interval_edit<Type> >(f, type);
      break;
    default:
      {
        CLAW_ASSERT(false, "range type is not valid.");
      }
    }
} // item_field_edit::show_simple_property_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the adequate dialog for editing a given field.
 * \param f The type of the field we are editing.
 * \param type The name of the type of the field.
 */
template<typename Control>
void bf::item_field_edit::show_property_dialog
( const type_field& f, const wxString& type )
{
  if ( f.is_list() )
    edit_field< Control, std::list<typename Control::value_type> >(f, type);
  else
    edit_field<Control, typename Control::value_type>(f, type);
} // item_field_edit::show_property_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create and show the dialog for editing the field and update the item.
 * \param f The field to edit.
 * \param type The name of the type of the field.
 */
template<typename Control, typename Type>
void
bf::item_field_edit::edit_field( const type_field& f, const wxString& type )
{
  typedef dialog_maker<Control, Type> dialog_maker_type;
  typedef typename dialog_maker_type::dialog_type dialog_type;

  dialog_type* dlg;

  Type v;
  if ( get_common_value(f, v) )
    dlg = dialog_maker_type::create(*this, type, f, v);
  else
    dlg = dialog_maker_type::create(*this, type, f, default_value<Type>::get());

  show_dialog(f.get_name(), *dlg);
  dlg->Destroy();
} // item_field_edit::edit_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create and show the dialog for editing the field and update the item.
 * \param f The field to edit.
 * \param type The name of the type of the field, as displayed in the title of
 *        the editor.
 * \param values The valid item identifiers for this field.
 */
template<typename Control, typename Type>
void bf::item_field_edit::edit_field
( const type_field& f, const wxString& type, const wxArrayString& values )
{
  typedef value_editor_dialog<Control, Type> dialog_type;

  dialog_type* dlg;

  Type v;
  if ( get_common_value(f, v) )
    dlg = new dialog_type(*this, type, values, v);
  else
    dlg = new dialog_type(*this, type, values, default_value<Type>::get());

  show_dialog(f.get_name(), *dlg);
  dlg->Destroy();
} // item_field_edit::edit_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show a created dialog for editing the field and update the item.
 * \param field_name The name of the edited field to edit.
 * \param dlg The dialog used to edit the field.
 */
template<typename DialogType>
void bf::item_field_edit::show_dialog
( const std::string& field_name, DialogType& dlg )
{
  if ( dlg.ShowModal() == wxID_OK )
    {
      typedef typename DialogType::value_type value_type;

      set_field_value_event<value_type> event
        ( field_name, dlg.get_value(),
          set_field_value_event<value_type>::set_field_value_event_type,
          GetId() );
      event.SetEventObject(this);

      if( ProcessEvent(event) )
        update_values();
    }
} // item_field_edit::show_dialog()
