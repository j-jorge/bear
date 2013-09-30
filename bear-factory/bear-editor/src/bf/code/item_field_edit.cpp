/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_field_edit class.
 * \author Julien Jorge
 */
#include "bf/item_field_edit.hpp"

#include "bf/bool_edit.hpp"
#include "bf/color_edit.hpp"
#include "bf/custom_type.hpp"
#include "bf/dialog_maker.hpp"
#include "bf/easing_edit.hpp"
#include "bf/free_edit.hpp"
#include "bf/is_visual_type.hpp"
#include "bf/item_class.hpp"
#include "bf/item_reference_edit.hpp"
#include "bf/set_edit.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/any_animation_edit.hpp"
#include "bf/font_edit.hpp"
#include "bf/sample_edit.hpp"
#include "bf/sprite_edit.hpp"

#include "bf/call_by_field_type.hpp"

#include <list>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Create and show the dialog for editing the field and update the item.
 * \param f The field to edit.
 * \param type The name of the type of the field.
 */
template<typename Control, typename Type>
void
bf::item_field_edit::field_editor<Control, Type, false>::open
( item_field_edit& self, const type_field& f, const wxString& type )
{
  typedef dialog_maker<Control, Type> dialog_maker_type;
  typedef typename dialog_maker_type::dialog_type dialog_type;

  Type v;

  if ( !self.get_common_value(f, v) )
    v = default_value<Type>::get();

  dialog_type* const dlg = dialog_maker_type::create( self, type, f, v );
  self.show_dialog(f.get_name(), *dlg);
  dlg->Destroy();
} // item_field_edit::field_editor::open()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates and show the dialog for editing the field and update the
 *        item. The created editor uses the image pool.
 * \param f The field to edit.
 * \param type The name of the type of the field.
 */
template<typename Control, typename Type>
void
bf::item_field_edit::field_editor<Control, Type, true>::open
( item_field_edit& self, const type_field& f, const wxString& type )
{
  typedef dialog_maker<Control, Type> dialog_maker_type;
  typedef typename dialog_maker_type::dialog_type dialog_type;

  Type v;

  if ( !self.get_common_value(f, v) )
    v = default_value<Type>::get();

  dialog_type* const dlg =
    dialog_maker_type::create( self, type, f, v, *self.m_image_pool );
  self.show_dialog(f.get_name(), *dlg);
  dlg->Destroy();
} // item_field_edit::field_editor::open()





/*----------------------------------------------------------------------------*/
const wxEventType
bf::delete_item_field_event::delete_field_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param field_name The name of the cleared field.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::delete_item_field_event::delete_item_field_event
( const std::string& field_name, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_field_name(field_name)
{

} // delete_item_field_event::delete_item_field_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::delete_item_field_event::Clone() const
{
  return new delete_item_field_event(*this);
} // delete_item_field_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the field to clear.
 */
const std::string& bf::delete_item_field_event::get_field_name() const
{
  return m_field_name;
} // delete_item_field_event::get_field_name()




/*----------------------------------------------------------------------------*/
const wxEventType
bf::request_item_id_event::request_item_id_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param field The field to which the id will be given.
 * \param val The array in which the identifiers will be stored.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::request_item_id_event::request_item_id_event
( const type_field& field, wxArrayString& val, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_field(field), m_values(val)
{

} // request_item_id_event::request_item_id_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::request_item_id_event::Clone() const
{
  return new request_item_id_event(*this);
} // request_item_id_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the field to which the id will be given.
 */
const bf::type_field& bf::request_item_id_event::get_type_field() const
{
  return m_field;
} // request_item_id_event::get_type_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an id in the list of valid identifiers.
 * \param id The identifier to add.
 */
void bf::request_item_id_event::add_id( const wxString& id ) const
{
  m_values.Add(id);
} // request_item_id_event::add_id()




/*----------------------------------------------------------------------------*/
const wxColour bf::item_field_edit::s_field_prefix_colour(*wxLIGHT_GREY);
const std::string bf::item_field_edit::s_no_prefix("- no class -");

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent Pointer to the owner.
 * \param id The identifier of this window.
 */
bf::item_field_edit::item_field_edit( wxWindow* parent, wxWindowID id )
  : wxListView( parent, id, wxDefaultPosition, wxDefaultSize,
                wxLC_REPORT | wxLC_VRULES | wxLC_SINGLE_SEL ),
    m_last_selected_field(wxNOT_FOUND), m_image_pool(NULL)
{
  InsertColumn(0, _("Property"));
  InsertColumn(1, _("Value"));

  Connect( wxEVT_SIZE,
           wxSizeEventHandler(item_field_edit::on_size) );
  Connect( wxEVT_COMMAND_LIST_COL_BEGIN_DRAG,
           wxListEventHandler(item_field_edit::on_column_begin_drag) );
  Connect( wxEVT_COMMAND_LIST_COL_END_DRAG,
           wxListEventHandler(item_field_edit::on_column_end_drag) );
  Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
           wxListEventHandler(item_field_edit::on_item_activated) );
  Connect( wxEVT_KEY_UP,
           wxKeyEventHandler(item_field_edit::on_key_up) );
} // item_field_edit::item_field_edit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh the values of the fields.
 */
void bf::item_field_edit::refresh()
{
  update_values();
} // item_field_edit::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item for which we want the properties.
 * \param item The item instance concerned by this window.
 * \return true if something has changed.
 */
bool bf::item_field_edit::add_item( item_instance* item )
{
  if ( item == NULL )
    return clear();

  if ( m_group.insert(item).second )
    {
      fill_fields();
      return true;
    }
  else
    return false;
} // item_field_edit::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add several items for which we want the properties.
 * \param items The item instances concerned by this window.
 * \return true if something has changed.
 */
bool
bf::item_field_edit::add_items( const std::vector<item_instance*>& items )
{
  bool result = false;

  for ( std::vector<item_instance*>::const_iterator it=items.begin();
        it!=items.end(); ++it )
    {
      bool inserted = m_group.insert(*it).second;
      result = result || inserted;
    }

  if( result )
    fill_fields();

  return result;
} // item_field_edit::add_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop editing the properties of an item.
 * \param item The item to remove.
 * \return true if something has changed.
 */
bool bf::item_field_edit::remove_item( item_instance* item )
{
  if ( m_group.erase(item) == 0 )
    return false;

  fill_fields();
  return true;
} // item_field_edit::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all items.
 * \return true if something has changed.
 */
bool bf::item_field_edit::clear()
{
  bool result = !empty();

  m_group.clear();
  int index = GetFirstSelected();

  if (index != wxNOT_FOUND)
    m_last_selected_field = index;

  DeleteAllItems();

  return result;
} // item_field_edit::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the control contains no items.
 */
bool bf::item_field_edit::empty() const
{
  return m_group.empty();
} // item_field_edit::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is only one edited item.
 */
bool bf::item_field_edit::has_single_item() const
{
  if ( empty() )
    return false;
  else
    return ++begin() == end();
} // item_field_edit::has_single_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the single edited item.
 */
bf::item_instance& bf::item_field_edit::get_single_item() const
{
  CLAW_PRECOND(has_single_item());

  return *begin();
} // item_field_edit::get_single_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first edited item.
 */
bf::item_field_edit::item_iterator bf::item_field_edit::begin() const
{
  return m_group.begin();
} // item_field_edit::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just after the last edited item.
 */
bf::item_field_edit::item_iterator bf::item_field_edit::end() const
{
  return m_group.end();
} // item_field_edit::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a pointer to the class of all items if they all have the same
 *        class.
 */
const bf::item_class* bf::item_field_edit::get_common_class() const
{
  if ( empty() )
    return NULL;

  const item_class* result(NULL);

  item_iterator it=begin();
  result = it->get_class_ptr();

  for (++it; (result != NULL) && (it!=end()); ++it)
    if ( result != it->get_class_ptr() )
      result = NULL;

  return result;
} // item_field_edit::get_common_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the field with a given name, common to all items.
 * \param name The name of the field.
 */
const bf::type_field&
bf::item_field_edit::get_common_field( const std::string& name ) const
{
  CLAW_PRECOND( !empty() );

#ifndef NDEBUG
  for (item_iterator it=begin(); it!=end(); ++it)
    {
      CLAW_ASSERT( it->get_class().has_field(name),
                   "Class '" + it->get_class().get_class_name() +
                   "' has no field named '" + name + "'." );
    }

  const type_field::field_type check_type =
    begin()->get_class().get_field(name).get_field_type();

  for (item_iterator it=begin(); it!=end(); ++it)
    {
      CLAW_ASSERT( it->get_class().get_field(name).get_field_type()
                   == check_type,
                   "Class '" + it->get_class().get_class_name() +
                   "' has a field named '" + name
                   + "' but with a different type." );
    }

  const bool check_list =
    begin()->get_class().get_field(name).is_list();

  for (item_iterator it=begin(); it!=end(); ++it)
    {
      CLAW_ASSERT( it->get_class().get_field(name).is_list() == check_list,
                   "Class '" + it->get_class().get_class_name() +
                   "' has a field named '" + name
                   + "' but with a different list status." );
    }
#endif

  return (*m_group.begin())->get_class().get_field(name);
} // item_field_edit::get_common_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of a field from the properties list.
 * \param i The index of the row in the properties list.
 * \param name (out) The name of the field.
 * \return false if \a i does not corresponds to a field line (ie. this is a
 *         prefix).
 */
bool
bf::item_field_edit::get_field_name( unsigned int i, std::string& name ) const
{
  bool result = false;

  name = wx_to_std_string(GetItemText(i));

  if ( GetItemBackgroundColour(i) != s_field_prefix_colour )
    {
      result = true;
      std::string field_prefix;

      while ( (i!=0) && field_prefix.empty() )
        {
          --i;
          if ( GetItemBackgroundColour(i) == s_field_prefix_colour )
            field_prefix = wx_to_std_string(GetItemText(i));
        }

      if ( !field_prefix.empty() && (field_prefix != s_no_prefix) )
        name = field_prefix + '.' + name;
    }

#ifndef NDEBUG
  bool check_all_items_have_field(true);

  for ( item_iterator it=begin(); check_all_items_have_field && (it!=end());
        ++it )
    check_all_items_have_field = it->get_class().has_field(name);

  CLAW_POSTCOND( !result || check_all_items_have_field );
#endif

  return result;
} // item_field_edit::get_field_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create and show the edit frame for a field.
 * \param name The name of the field.
 */
void bf::item_field_edit::create_field_editor( const std::string& name )
{
  const type_field& f = get_common_field(name);
  m_last_edited_field = name;

  switch ( f.get_field_type() )
    {
    case type_field::integer_field_type:
      show_simple_property_dialog<integer_type>(f, _("Integer"));
      break;
    case type_field::u_integer_field_type:
      show_simple_property_dialog<u_integer_type>(f, _("Unsigned integer"));
      break;
    case type_field::real_field_type:
      show_simple_property_dialog<real_type>(f, _("Real number"));
      break;
    case type_field::boolean_field_type:
      if ( !f.is_list() )
        toggle_boolean_field_value(f);
      else
        show_property_dialog<bool_edit>(f, _("Boolean value"));
      break;
    case type_field::string_field_type:
      show_string_property_dialog(f);
      break;
    case type_field::sprite_field_type:
      show_property_dialog<sprite_edit>(f, _("Sprite"));
      break;
    case type_field::animation_field_type:
      show_property_dialog<any_animation_edit>(f, _("Animation"));
      break;
    case type_field::item_reference_field_type:
      show_item_reference_property_dialog(f);
      break;
    case type_field::font_field_type:
      show_property_dialog<font_edit>(f, _("Font"));
      break;
    case type_field::sample_field_type:
      show_property_dialog<sample_edit>(f, _("Sound sample"));
      break;    
    case type_field::color_field_type:
      show_property_dialog<color_edit>(f, _("Color"));
      break;
    case type_field::easing_field_type:
      show_property_dialog<easing_edit>(f, _("Easing function"));
      break;
    }
} // item_field_edit::create_field_editor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the image pool to use to access the images of the item.
 * \param pool The pool to use.
 */
void bf::item_field_edit::set_active_image_pool( image_pool* pool )
{
  m_image_pool = pool;
} // item_field_edit::set_active_image_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enumerate all the properties of the current item.
 */
void bf::item_field_edit::enumerate_properties()
{
  if (empty())
    return;

  std::list<const item_class*> item_classes = get_common_classes();

  std::set<std::string> fields;
  std::set<std::string> removed;

  for ( ; !item_classes.empty(); item_classes.pop_front() )
    {
      const item_class* c=item_classes.front();

      for ( item_class::field_iterator it=c->field_begin();
            it!=c->field_end(); ++it )
        {
          const std::string field_name( it->get_name() );

          if ( c->is_removed_field(field_name) )
            {
              fields.erase(field_name);
              removed.insert(field_name);
            }
          else if ( removed.find(field_name) == removed.end() )
            fields.insert(field_name);
        }
    }

  show_fields( std::list<std::string>(fields.begin(), fields.end()) );
} // item_field_edit::enumerate_properties()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the classes present in the hierarchy of all edited items.
 */
std::list<const bf::item_class*> bf::item_field_edit::get_common_classes() const
{
  if ( empty() )
    return std::list<const item_class*>();

  typedef std::list<const item_class*> class_list;

  item_iterator it=begin();
  class_list item_classes;
  it->get_class().find_hierarchy( item_classes );
  item_classes.sort();
  item_classes.unique();

  for ( ++it; it!=end(); ++it )
    {
      class_list candidates;
      it->get_class().find_hierarchy(candidates);
      candidates.sort();
      candidates.unique();

      class_list old_classes;
      item_classes.swap(old_classes);

      std::set_intersection
        ( old_classes.begin(), old_classes.end(),
          candidates.begin(), candidates.end(),
          std::back_inserter(item_classes) );
    }

  return item_classes;
} // item_field_edit::get_common_classes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the fields of a given class.
 * \param fields (out) The fields of \a classname are pushed back to this table.
 * \param item The class for which we want the fields.
 */
void bf::item_field_edit::get_fields_of
( std::vector<std::string>& fields, const item_class& item ) const
{
  item_class::field_iterator it;

  for ( it=item.field_begin(); it!=item.field_end(); ++it )
    fields.push_back(it->get_name());
} // item_field_edit::get_fields_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the fields in the view of the properties.
 * \param fields The names of the fields to add.
 */
void bf::item_field_edit::show_fields( const std::list<std::string>& fields )
{
  wxString prefix;
  wxString last_prefix;
  std::size_t index = 0;
  std::list<std::string>::const_iterator it;
  int last_edited_index(wxNOT_FOUND);

  for ( it=fields.begin(); it!=fields.end(); ++it )
    {
      if ( *it == m_last_edited_field )
        last_edited_index = index;

      wxString f(std_to_wx_string(*it));
      prefix = f.BeforeFirst(wxT('.'));

      if (prefix == f)
        prefix = std_to_wx_string(s_no_prefix);
      else
        f = f.AfterFirst(wxT('.'));

      if (prefix != last_prefix)
        {
          wxFont font( GetFont() );

          if (m_hidden.find(wx_to_std_string(prefix)) != m_hidden.end())
            font.SetStyle( wxFONTSTYLE_ITALIC );

          wxListItem head;
          head.SetFont( font );
          head.SetText(prefix);
          head.SetBackgroundColour(s_field_prefix_colour);
          head.SetId(index);
          ++index;

          InsertItem(head);
          last_prefix = prefix;
        }

      if ( m_hidden.find(wx_to_std_string(prefix)) == m_hidden.end() )
        {
          wxListItem prop;
          prop.SetText(f);
          prop.SetId(index);
          ++index;
          InsertItem(prop);
        }
    }

  SetColumnWidth(0, wxLIST_AUTOSIZE);
  adjust_last_column_size();

  if (last_edited_index != wxNOT_FOUND )
    {
      Select(last_edited_index);
      EnsureVisible(last_edited_index);
    }
} // item_field_edit::show_fields()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of all fields of the current item.
 */
void bf::item_field_edit::update_values()
{
  for ( long i=0; i!=GetItemCount(); ++i )
    update_value(i);
} // item_field_edit::update_values()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the displayed value of a field.
 * \param index The index of the field in the view.
 */
void bf::item_field_edit::update_value( long index )
{
  std::string name;

  if( get_field_name(index, name) )
    {
      wxListItem prop;
      prop.SetId(index);
      GetItem(prop);

      const type_field& f = get_common_field(name);
      const bool has_val = group_has_value(f);
      prop.SetText( get_common_value_as_text(f) );

      prop.SetColumn(1);
      SetItem(prop);

      if ( f.get_required() )
        set_required_color(index, has_val);
      else
        set_default_value_color(index, has_val);
    }
} // item_field_edit::update_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the correct font for a required field.
 * \param i The line to change.
 * \param b Tell if the field has a value or not.
 */
void bf::item_field_edit::set_required_color( unsigned int i, bool b )
{
  wxListItem prop;
  prop.SetId(i);
  GetItem(prop);

  wxFont font( GetFont() );
  font.SetWeight( wxFONTWEIGHT_BOLD );
  prop.SetFont( font );

  if (b)
    prop.SetTextColour( *wxBLACK );
  else
    prop.SetTextColour( *wxRED );

  SetItem(prop);
} // item_field_edit::set_required_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the correct font for a non valued field.
 * \param i The line to change.
 * \param b Tell if the field has a value or not.
 */
void bf::item_field_edit::set_default_value_color( unsigned int i, bool b )
{
  wxListItem prop;
  prop.SetId(i);

  GetItem(prop);

  wxFont font( GetFont() );

  if (b)
    {
      font.SetStyle( wxFONTSTYLE_NORMAL );
      prop.SetTextColour( *wxBLACK );
    }
  else
    {
      font.SetStyle( wxFONTSTYLE_ITALIC );
      prop.SetTextColour( wxColour(wxT("DARK GREY")) );
    }

  prop.SetFont( font );
  SetItem(prop);
} // item_field_edit::set_default_value_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a field in a string.
 * \param item The item in which the value is taken.
 * \param f The field for which we want the value.
 */
wxString bf::item_field_edit::convert_value_to_text
( const item_instance& item, const type_field& f ) const
{
  if ( !item.has_value(f) )
    return std_to_wx_string( item.get_class().get_default_value(f.get_name()) );
  else
    {
      call_by_field_type<value_to_text_converter, wxString> call;
      return call(f, f.get_name(), item);
    }
} // item_field_edit::convert_value_to_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ajust the size of the last column so there is no empty space on the
 *        right.
 */
void bf::item_field_edit::adjust_last_column_size()
{
  SetColumnWidth( 1, GetSize().x - GetColumnWidth(0) );
} // item_field_edit::adjust_last_column_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the value in the selected field.
 */
void bf::item_field_edit::delete_selected_field()
{
  long index = GetFocusedItem();

  if ( index != wxNOT_FOUND )
    {
      std::string name;

      if ( get_field_name(index, name) )
        {
          m_last_edited_field = name;
          delete_item_field_event event
            ( name, delete_item_field_event::delete_field_event_type, GetId() );
          event.SetEventObject(this);

          if ( ProcessEvent(event) )
            update_value(index);
        }
    }
} // item_field_edit::delete_selected_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the adequate dialog for editing a given string field.
 * \param f The type of the field we are editing.
 */
void bf::item_field_edit::show_string_property_dialog( const type_field& f )
{
  switch ( f.get_range_type() )
    {
    case type_field::field_range_free:
      show_property_dialog< free_edit<string_type> >(f, _("string"));
      break;
    case type_field::field_range_set:
      show_property_dialog< set_edit<string_type> >(f, _("string"));
      break;
    default:
      {
        CLAW_ASSERT(false, "range type is not valid.");
      }
    }
} // item_field_edit::show_string_property_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the adequate dialog for editing a given item reference field.
 * \param f The type of the field we are editing.
 */
void
bf::item_field_edit::show_item_reference_property_dialog( const type_field& f )
{
  wxArrayString values;

  request_item_id_event event
    ( f, values, request_item_id_event::request_item_id_event_type, GetId() );
  event.SetEventObject(this);

  if ( ProcessEvent(event) )
    {
      values.Sort();

      if ( f.is_list() )
        edit_field< item_reference_edit, std::list<item_reference_type> >
          (f, _("Item"), values);
      else
        edit_field<item_reference_edit, item_reference_type>
          (f, _("List of items"), values);
    }
} // item_field_edit::show_item_reference_property_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the value of a boolean field.
 * \param f The type of the field we are editing.
 */
void bf::item_field_edit::toggle_boolean_field_value( const type_field& f )
{
  CLAW_PRECOND(!f.is_list());
  CLAW_PRECOND(f.get_field_type() == type_field::boolean_field_type);

  bool_type v;
  if ( !get_common_value(f, v) )
    v.set_value(true);
  else
    v.set_value(!v.get_value());

  set_field_value_event<bool_type> event
    ( f.get_name(), v,
      set_field_value_event<bool_type>::set_field_value_event_type, GetId() );
  event.SetEventObject(this);

  if( ProcessEvent(event) )
    update_values();
} // item_field_edit::toggle_boolean_field_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list of fields.
 */
void bf::item_field_edit::fill_fields()
{
  DeleteAllItems();

  if ( empty() )
    return;

  enumerate_properties();
  update_values();

  if ( m_last_selected_field != wxNOT_FOUND )
    if ( m_last_selected_field < GetItemCount() )
      {
        Select(m_last_selected_field);
        EnsureVisible(m_last_selected_field);
      }
} // item_field_edit::fill_fields()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all items in the edited group has a value for a given field.
 * \param f The field to test.
 */
bool bf::item_field_edit::group_has_value( const type_field& f ) const
{
  bool result( !empty() );

  for (item_iterator it=begin(); result && (it!=end()); ++it)
    result = it->has_value(f);

  return result;
} // item_field_edit::group_has_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value common to all edited items for a given field, or an
 *        empty string if two items have a different value for this field.
 * \param f The field to test.
 */
wxString
bf::item_field_edit::get_common_value_as_text( const type_field& f ) const
{
  CLAW_PRECOND(!empty());

  item_iterator it=begin();
  wxString result( convert_value_to_text(*it, f) );

  for ( ++it; !result.empty() && (it!=end()); ++it )
    if ( convert_value_to_text(*it, f) != result )
      result.clear();

  return result;
} // item_field_edit::get_common_value_as_text()

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
  typedef typename Control::value_type value_type;

  if ( f.is_list() )
    field_editor< Control, std::list<value_type>,
                  is_visual_type<value_type>::value >::open(*this, f, type);
  else
    field_editor< Control, value_type,
                  is_visual_type<value_type>::value >::open(*this, f, type);
} // item_field_edit::show_property_dialog()

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

  Type v;

  if ( !get_common_value(f, v) )
    v = default_value<Type>::get();

  dialog_type* const dlg = new dialog_type( *this, type, values, v );
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

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::item_field_edit::on_size( wxSizeEvent& event )
{
  adjust_last_column_size();
  event.Skip();
} // item_field_edit::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user starts to resize a column.
 * \param event The event.
 */
void bf::item_field_edit::on_column_begin_drag( wxListEvent& event )
{
  if ( event.GetColumn() + 1 == GetColumnCount() )
    event.Veto();
  else
    event.Skip();
} // item_field_edit::on_column_begin_drag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user ends resizing a column.
 * \param event The event.
 */
void bf::item_field_edit::on_column_end_drag( wxListEvent& WXUNUSED(event) )
{
  adjust_last_column_size();
} // item_field_edit::on_column_begin_drag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user activates an item.
 * \param event The event.
 */
void bf::item_field_edit::on_item_activated( wxListEvent& event )
{
  std::string name;

  if ( get_field_name(event.GetIndex(), name) )
    create_field_editor( name );
  else
    {
      if ( m_hidden.find(name) == m_hidden.end() )
        m_hidden.insert(name);
      else
        m_hidden.erase(name);

      DeleteAllItems();
      enumerate_properties();
      update_values();

      if ( event.GetIndex() < GetItemCount() )
        Select( event.GetIndex() );
    }
} // item_field_edit::on_item_activated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user presses a key.
 * \param event The keyboard event that occured.
 */
void bf::item_field_edit::on_key_up( wxKeyEvent& event )
{
  switch( event.GetKeyCode() )
    {
    case WXK_DELETE:
      delete_selected_field();
      break;
    default:
      event.Skip();
    }
} // item_field_edit::on_key_up()
