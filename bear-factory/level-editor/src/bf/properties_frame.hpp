/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window showing the properties of an item.
 * \author Julien Jorge
 */
#ifndef __BF_PROPERTIES_FRAME_HPP__
#define __BF_PROPERTIES_FRAME_HPP__

#include <wx/checkbox.h>
#include <wx/hyperlink.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <string>
#include <vector>

namespace bf
{
  class level_action;
  class item_field_edit;
  class item_instance;
  class type_field;
  class windows_layout;
  class delete_item_field_event;
  class request_item_id_event;
  template<typename T>
  class set_field_value_event;

  /**
   * \brief The properties window of our program.
   * \author Julien Jorge
   */
  class properties_frame:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_ITEM_PROPERTIES,
        IDC_FIXED_STATE,
        IDC_TEXT_IDENTIFIER,
        IDC_CHANGE_ITEM_CLASS,
        IDC_ITEM_CLASS_NAME
      }; // enum control_id

  private:
    template<typename T>
    struct field_deleter
    {
      level_action* operator()
        ( const item_field_edit& edit, const std::string& name ) const;
    }; // struct field_deleter

  public:
    properties_frame( wxWindow* parent );

    void set_window_layout( windows_layout& layout );

    void clear();
    void add_item( item_instance* item );
    void add_items( const std::vector<item_instance*>& items );
    void remove_item( item_instance* item );
    void edit_field(const std::string& field_name);

    void refresh();

    bool has_last_edited_field_name() const;
    const std::string& get_last_edited_field_name() const;
    void set_last_edited_field_name(const std::string& field_name);

  private:
    void fill_controls();
    void update_controls();
    void create_controls();

    void get_item_identifiers( wxArrayString& id, const type_field& f );

    void clear_items_id();
    void set_item_id();

    template<typename Type>
    void on_set_field_value( set_field_value_event<Type>& e );

    void on_close(wxCloseEvent& event);
    void on_item_focused(wxListEvent& event);
    void on_change_fixed(wxCommandEvent& event);
    void on_change_id(wxCommandEvent& event);
    void on_validate_id(wxCommandEvent& event);
    void on_change_item_class(wxCommandEvent& event);
    void on_item_class_click(wxHyperlinkEvent& event);
    void on_delete_field( delete_item_field_event& e );
    void on_request_item_id( request_item_id_event& e );

  private:
    /** \brief The control in which we display the class of the item. */
    wxHyperlinkCtrl* m_item_class;

    /** \brief The list of properties available for this item. */
    item_field_edit* m_prop;

    /** \brief The control for configure the fixed option. */
    wxCheckBox* m_fixed_box;

    /** \brief The control in which we display the identifiant. */
    wxTextCtrl* m_id_text;

    /** \brief The control in which we display the description of the selected
        field. */
    wxStaticText* m_description;

    /** \brief The windows of the program. */
    windows_layout* m_windows_layout;

    /** \brief The las edited field name. */
    std::string m_last_edited_field_name;

    DECLARE_EVENT_TABLE()

  }; // class properties_frame
} // namespace bf

#endif // __BF_PROPERTIES_FRAME_HPP__
