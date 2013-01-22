/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A wxTreeCtrl of the available item classes.
 * \author Julien Jorge
 */
#ifndef __BF_CLASS_TREE_CTRL_HPP__
#define __BF_CLASS_TREE_CTRL_HPP__

#include <wx/treectrl.h>
#include <wx/stattext.h>
#include <wx/listctrl.h>
#include <wx/dnd.h>

#include <string>
#include <list>

#include "bf/libeditor_export.hpp"

#define class_selected_event_handler(func)                              \
  (wxObjectEventFunction)(wxEventFunction)                              \
  wxStaticCastEvent(bf::class_selected_event::function_type, &func)

#define EVT_CLASS_SELECTED(id, func)                                    \
  wx__DECLARE_EVT1( bf::class_selected_event::class_selected_event_type, \
                    id, class_selected_event_handler(func) )

namespace bf
{
  class item_class_pool;
  class tree_builder;

  /**
   * \brief Event sent when an item class is selected.
   * \author Julien Jorge
   */
  class class_selected_event:
    public wxNotifyEvent
  {
  public:
    typedef void (wxEvtHandler::*function_type)(class_selected_event&);

  public:
    explicit class_selected_event
    ( const std::string& class_name, wxEventType t = wxEVT_NULL,
      wxWindowID id = wxID_ANY );

    wxEvent* Clone() const;

    const std::string& get_class_name() const;

  public:
    static const wxEventType class_selected_event_type;

  private:
    /** \brief The name of the selected class. */
    const std::string m_class_name;

  }; // class class_selected_event

  /**
   * \brief A wxTreeCtrl of the available item classes.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT class_tree_ctrl:
    public wxPanel
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_PATTERN,
        IDC_TREE,
        IDC_RECENT_USED_CLASSES
      }; // enum control_id

  public:
    class_tree_ctrl
    ( const item_class_pool& pool, wxWindow* parent, int id = wxID_ANY );

    wxTreeItemId GetSelection() const;
    bool ItemHasChildren( const wxTreeItemId& item ) const;
    wxString GetItemText( const wxTreeItemId& item ) const;
    void use_class( const std::string & c );
    
  private:
    void add_recent_used_class( const std::string & c );
    void create_controls();

    void fill_tree();
    void fill_recent_used_classes_list();
    void create_categories_tree( tree_builder& tb ) const;

    wxString make_pattern() const;

    void select_class(bool shift_down);
    void select_class( const std::string& c );
    void unselect_class_in_tree();
    void unselect_recent_class();
    void show_class_description();    

    void on_double_click(wxMouseEvent& event);
    void on_class_change(wxTreeEvent& event);
    void on_recent_class_change( wxListEvent& event );
    void on_recent_class_activated( wxListEvent& event );
    void on_key_up(wxKeyEvent& event);
    void on_mouse_move(wxMouseEvent& event);
    void on_pattern_change(wxCommandEvent& event);
    void on_size(wxSizeEvent& event);

  private:
    /** \brief The tree in which the class are stored. */
    wxTreeCtrl* m_tree;

    /** \brief The control in which we can type a search pattern. */
    wxTextCtrl* m_pattern;

    /** \brief The control in which we show the description of current class. */
    wxStaticText* m_class_description;

    /** \brief The classes for which we build the tree. */
    const item_class_pool& m_pool;

     /** \brief The control in which we show recent used classes. */
    wxListView* m_recent_used_classes_list;

    /** \brief List of recent used classes. */
    std::list< std::string > m_recent_used_classes;

    /** \brief Current selected class. */
    wxTextDataObject m_selected_class;

    DECLARE_EVENT_TABLE()

  }; // class class_tree_ctrl
} // namespace bf

#endif // __BF_CLASS_TREE_CTRL_HPP__
