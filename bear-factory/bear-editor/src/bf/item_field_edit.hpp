/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A frame to edit the fields of an item.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_FIELD_EDIT_HPP__
#define __BF_ITEM_FIELD_EDIT_HPP__

#include <wx/panel.h>
#include <wx/listctrl.h>

#include "bf/item_instance.hpp"
#include "bf/type_field.hpp"

#include <vector>

#include <claw/iterator.hpp>
#include <claw/functional.hpp>

#define delete_item_field_event_handler(func)                           \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(bf::delete_item_field_event::function_type, &func)

#define EVT_DELETE_ITEM_FIELD(id, func)                                 \
  wx__DECLARE_EVT1( bf::delete_item_field_event::delete_field_event_type, \
                    id, delete_item_field_event_handler(func) )

#define set_field_value_event_handler(func, T)                          \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(bf::set_field_value_event<T>::function_type, &func)

#define EVT_SET_FIELD_VALUE(id, func, T)                                \
  wx__DECLARE_EVT1( bf::set_field_value_event<T>::set_field_value_event_type, \
                    id, set_field_value_event_handler(func, T) )

#define set_field_value_event_handler_by_addr(func_addr, T)             \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(set_field_value_event<T>::function_type, func_addr)

#define EVT_SET_FIELD_VALUE_TEMPLATE(id, class_name, func, T)            \
  wx__DECLARE_EVT1( bf::set_field_value_event<T>::set_field_value_event_type, \
                    id,                                                 \
                    set_field_value_event_handler_by_addr(              \
                      (void                                             \
                       (class_name::*)( bf::set_field_value_event<T>& ) ) \
                      &class_name::func<T>, T) )

#define request_item_id_event_handler(func)                             \
  (wxObjectEventFunction)(wxEventFunction)                              \
    wxStaticCastEvent(bf::request_item_id_event::function_type, &func)

#define EVT_REQUEST_ITEM_IDENTIFIER(id, func)                           \
  wx__DECLARE_EVT1( bf::request_item_id_event::request_item_id_event_type, \
                    id, request_item_id_event_handler(func) )

namespace bf
{
  /**
   * \brief Event sent when the value of a field is cleared.
   * \author Julien Jorge
   */
  class delete_item_field_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(delete_item_field_event&);

  public:
    explicit delete_item_field_event
    ( const std::string& field_name, wxEventType t = wxEVT_NULL,
      wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

    const std::string& get_field_name() const;

  public:
    static const wxEventType delete_field_event_type;

  private:
    /** \brief The field whose value is cleared. */
    const std::string m_field_name;

  }; // class delete_item_field_event

  /**
   * \brief Event sent to request the valid item identifiers for a field.
   * \author Julien Jorge
   */
  class request_item_id_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(request_item_id_event&);

  public:
    request_item_id_event
    ( const type_field& field, wxArrayString& val, wxEventType t = wxEVT_NULL,
      wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

    const type_field& get_type_field() const;
    void add_id( const wxString& id ) const;

  public:
    static const wxEventType request_item_id_event_type;

  private:
    /** \brief The field to which the id will be given. */
    const type_field& m_field;

    /** \brief The valid identifiers for this field. */
    wxArrayString& m_values;

  }; // class request_item_id_event

  /**
   * \brief Event sent when a field receives a value.
   * \author Julien Jorge
   */
  template<typename T>
  class set_field_value_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(set_field_value_event<T>&);

  public:
    set_field_value_event
    ( const std::string& field_name, const T& val, wxEventType t = wxEVT_NULL,
      wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

    const std::string& get_field_name() const;
    const T& get_value() const;

  public:
    static const wxEventType set_field_value_event_type;

  private:
    /** \brief The field whose value is set. */
    const std::string m_field_name;

    /** \brief The new value of the field. */
    const T m_value;

  }; // class set_field_value_event

  /**
   * \brief A frame to edit the fields of an item.
   * \author Julien Jorge
   */
  class item_field_edit:
    public wxListView
  {
  public:
    /** \brief This function object converts the value of a field of an item to
        its text representation. */
    template<typename Type>
    class value_to_text_converter
    {
    public:
      wxString operator()
        ( const std::string& field_name, const item_instance& item ) const;
    }; // class value_to_text_converter

  private:
    typedef std::set<item_instance*> item_set;

    /**
     * \brief The field_editor class opens a dialog to edit a field.
     */
    template<typename Control, typename Type, bool IsVisual>
    struct field_editor;

    /**
     * \brief Specialization of the field_editor class for the case where the
     *        image pool is needed.
     */
    template<typename Control, typename Type>
    struct field_editor<Control, Type, true>
    {
      static void open
      ( item_field_edit& self, const type_field& f, const wxString& type );
    }; // struct field_editor [visual]

    /**
     * \brief Specialization of the field_editor class for the case where the
     *        image pool is not needed.
     */
    template<typename Control, typename Type>
    struct field_editor<Control, Type, false>
    {
      static void open
      ( item_field_edit& self, const type_field& f, const wxString& type );
    }; // struct field_editor [not visual]

  public:
    typedef claw::wrapped_iterator
    < item_instance,
      item_set::const_iterator,
      claw::dereference<item_instance> >
    ::iterator_type item_iterator;

  public:
    item_field_edit( wxWindow* parent, wxWindowID id = wxID_ANY );

    void refresh();

    bool add_item( item_instance* item );
    bool add_items( const std::vector<item_instance*>& items );
    bool remove_item( item_instance* item );
    bool clear();
    bool empty() const;

    bool has_single_item() const;
    item_instance& get_single_item() const;

    item_iterator begin() const;
    item_iterator end() const;

    const item_class* get_common_class() const;
    const type_field& get_common_field( const std::string& name ) const;
    bool get_field_name( unsigned int i, std::string& name ) const;
    void create_field_editor( const std::string& name );

    void set_active_image_pool( image_pool* pool );

  private:
    void enumerate_properties();
    std::list<const item_class*> get_common_classes() const;

    void get_fields_of
    ( std::vector<std::string>& fields, const item_class& item ) const;
    void show_fields( const std::list<std::string>& fields );

    void update_values();
    void update_value( long index );
    void set_required_color( unsigned int i, bool b );
    void set_default_value_color( unsigned int i, bool b );

    wxString convert_value_to_text
    ( const item_instance& item, const type_field& f ) const;

    void adjust_last_column_size();

    void delete_selected_field();
    void show_string_property_dialog( const type_field& f );
    void show_item_reference_property_dialog( const type_field& f );
    void toggle_boolean_field_value( const type_field& f );

    void fill_fields();
    bool group_has_value( const type_field& f ) const;
    wxString get_common_value_as_text( const type_field& f ) const;

    template<typename Type>
    bool get_common_value( const type_field& f, Type& val ) const;

    template<typename Type>
    void
    show_simple_property_dialog( const type_field& f, const wxString& type );

    template<typename Control>
    void show_property_dialog( const type_field& f, const wxString& type );
    template<typename Control, typename Type>
    void edit_field
    ( const type_field& f, const wxString& type, const wxArrayString& values );

    template<typename DialogType>
    void show_dialog( const std::string& field_name, DialogType& dlg );

    void on_size(wxSizeEvent& event);
    void on_column_begin_drag(wxListEvent& event);
    void on_column_end_drag(wxListEvent& event);
    void on_item_activated(wxListEvent& event);
    void on_key_up(wxKeyEvent& event);

  private:
    /** \brief The items of which we modify the fields. */
    item_set m_group;

    /** \brief The prefixes of the not displayed fields. */
    std::set<std::string> m_hidden;

    /** \brief The name of the last edited field. */
    std::string m_last_edited_field;

    /** \brief The index of the last selected field. */
    int m_last_selected_field;

    /** \brief The image pool from which the images are taken. */
    image_pool* m_image_pool;

    /** \brief The backround color of the prefix of the fields. */
    static const wxColour s_field_prefix_colour;

    /** \brief The text used for prefix for fields without prefix. */
    static const std::string s_no_prefix;

  }; // class item_field_edit
} // namespace bf

#include "bf/impl/item_field_edit.tpp"

#endif // __BF_ITEM_FIELD_EDIT_HPP__
