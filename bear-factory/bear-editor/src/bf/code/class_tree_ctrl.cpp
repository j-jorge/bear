/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::class_tree_ctrl class.
 * \author Julien Jorge
 */
#include "bf/class_tree_ctrl.hpp"

#include "bf/workspace_environment.hpp"
#include "bf/tree_builder.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/dnd.h>
#include <wx/sizer.h>

/*----------------------------------------------------------------------------*/
const wxEventType
bf::class_selected_event::class_selected_event_type = wxNewEventType();

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param class_name The name of the selected class.
 * \param t The type of the event.
 * \param id The id of the window that generates the event.
 */
bf::class_selected_event::class_selected_event
( const std::string& class_name, wxEventType t, wxWindowID id )
  : wxNotifyEvent(t, id), m_class_name(class_name)
{

} // class_selected_event::class_selected_event()

/*----------------------------------------------------------------------------*/
/**
 * \brief Allocate a copy of this instance.
 */
wxEvent* bf::class_selected_event::Clone() const
{
  return new class_selected_event(*this);
} // class_selected_event::Clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the selected class.
 */
const std::string& bf::class_selected_event::get_class_name() const
{
  return m_class_name;
} // class_selected_event::get_class_name()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param env The workspace environment used.
 * \param parent Pointer to the owner.
 * \param id Identifier of the control.
 * \remark \a pool must live longer than this instance of class_tree_ctrl.
 */
bf::class_tree_ctrl::class_tree_ctrl
( workspace_environment* env, wxWindow* parent, int id )
  : wxPanel( parent, id, wxDefaultPosition, wxDefaultSize ), m_workspace(env)
{
  create_controls();
  fill_tree();
  fill_recent_used_classes_list();
} // class_tree_ctrl::class_tree_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selected item in the tree.
 */
wxTreeItemId bf::class_tree_ctrl::GetSelection() const
{
  return m_tree->GetSelection();
} // class_tree_ctrl::GetSelection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item of the tree has children nodes.
 * \param item The item we're looking for.
 */
bool bf::class_tree_ctrl::ItemHasChildren( const wxTreeItemId& item ) const
{
  return m_tree->ItemHasChildren(item);
} // class_tree_ctrl::ItemHasChildren()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the text associated with an item of the tree.
 * \param item The item we're looking for.
 */
wxString bf::class_tree_ctrl::GetItemText( const wxTreeItemId& item ) const
{
  return m_tree->GetItemText(item);
} // class_tree_ctrl::GetItemText()

/*----------------------------------------------------------------------------*/
/**
 * \brief A class is used.
 * \param c The class to add.
 */
void bf::class_tree_ctrl::use_class( const std::string & c )
{
  add_recent_used_class( c );
  m_recent_used_classes_list->Select(0);
} // class_tree_ctrl::use_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a recent used class.
 * \param c The class to add.
 */
void bf::class_tree_ctrl::add_recent_used_class( const std::string & c )
{
  // delete used class
  bool found = false;
  std::list< std::string >::iterator it;

  for ( it = m_recent_used_classes.begin(); 
        it != m_recent_used_classes.end() && ! found ; )
    if ( *it == c )
      found = true;
    else
      ++it;

  if ( found )
    m_recent_used_classes.erase(it);
 
  // delete last class if the list is full
  if ( m_recent_used_classes.size() >= 10 )
    m_recent_used_classes.pop_back();

  // add used class
  m_recent_used_classes.push_front(c);

  fill_recent_used_classes_list();
} // bf::class_tree_ctrl::add_recent_used_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 */
void bf::class_tree_ctrl::create_controls()
{
  m_tree = new wxTreeCtrl
    ( this, IDC_TREE, wxDefaultPosition, wxDefaultSize,
      wxTR_DEFAULT_STYLE | wxTR_FULL_ROW_HIGHLIGHT | wxTR_HIDE_ROOT );

  m_recent_used_classes_list = 
    new wxListView
    ( this, IDC_RECENT_USED_CLASSES, wxDefaultPosition,
      wxDefaultSize, wxLC_REPORT | wxLC_VRULES | wxLC_SINGLE_SEL );
  m_recent_used_classes_list->InsertColumn(0, _("Recent used classes :"));

  m_pattern = new wxTextCtrl( this, IDC_PATTERN );
  m_pattern->SetToolTip
    ( _("Type a pattern to filter the classes in the tree. "
        "For example: typing \"b*r\" will display the classes whose name "
        "contains the letter b followed by the letter r, possibly with "
        "letters between.") );
  m_class_description =
    new wxStaticText(this, wxID_ANY, _("Description:"));

  wxSizer* h_sizer = new wxBoxSizer( wxHORIZONTAL );
  wxSizer* v_sizer = new wxBoxSizer( wxVERTICAL );

  h_sizer->Add( new wxStaticText(this, wxID_ANY, _("Search:")), 0, wxEXPAND );
  h_sizer->Add( m_pattern, 1, wxEXPAND );
  
  v_sizer->Add(m_recent_used_classes_list, 1, wxEXPAND);
  v_sizer->AddSpacer(5);  
  v_sizer->Add(h_sizer, 0, wxEXPAND);
  v_sizer->Add(m_tree, 2, wxEXPAND);
  v_sizer->AddSpacer(5);
  v_sizer->Add(m_class_description, 0, wxEXPAND);

  SetSizer(v_sizer);

  m_tree->Connect
    ( wxEVT_LEFT_DCLICK,
      wxMouseEventHandler( class_tree_ctrl::on_double_click ), NULL, this );
  m_tree->Connect
    ( wxEVT_KEY_UP, wxKeyEventHandler( class_tree_ctrl::on_key_up ), NULL,
      this );
  m_tree->Connect
    ( wxEVT_MOTION, wxMouseEventHandler( class_tree_ctrl::on_mouse_move ), NULL,
      this );

  m_recent_used_classes_list->Connect
    ( wxEVT_MOTION, wxMouseEventHandler( class_tree_ctrl::on_mouse_move ), NULL,
      this );
} // class_tree_ctrl::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the tree of classes.
 */
void bf::class_tree_ctrl::fill_tree()
{
  m_tree->DeleteAllItems();
  m_tree->AddRoot( _("Item classes") );

  tree_builder tb;
  
  if ( m_workspace )
    create_categories_tree(tb);
  tb.create_wxTree( *m_tree );
  m_tree->ExpandAll();
} // class_tree_ctrl::fill_tree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the list of recent used classes.
 */
void bf::class_tree_ctrl::fill_recent_used_classes_list()
{
  std::list< std::string >::const_iterator it;
  unsigned int i = 0;

  m_recent_used_classes_list->DeleteAllItems();

  for ( it = m_recent_used_classes.begin(); 
        it != m_recent_used_classes.end(); ++i, ++it )
    m_recent_used_classes_list->InsertItem(i, std_to_wx_string(*it));
} // class_tree_ctrl::fill_recent_used_classes_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the tree of classes.
 * \param tb (out) The tree builder.
 */
void bf::class_tree_ctrl::create_categories_tree( tree_builder& tb ) const
{
  const wxString pattern( make_pattern() );

  item_class_pool::const_iterator it = 
    m_workspace->get_item_class_pool().begin();
  const item_class_pool::const_iterator eit = 
    m_workspace->get_item_class_pool().end();

  for ( ; it!=eit; ++it )
    if ( it->get_category() != "-abstract-" )
      {
        const wxString description( std_to_wx_string( it->get_description() ) );
        const wxString class_name( std_to_wx_string( it->get_class_name() ) );
        const wxString category( std_to_wx_string( it->get_category() ) );

        if ( class_name.Matches(pattern) || description.Matches(pattern)
             || category.Matches(pattern) )
          {
            const std::string cat( it->get_category() );

            if ( cat.empty() )
              tb.add_entries( it->get_class_name(), '/' );
            else
              tb.add_entries( cat + '/' + it->get_class_name(), '/' );
          }
      }
} // class_tree_ctrl::create_categories_tree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a pattern from the pattern in the search box, that will match
 *        the original pattern anywhere in the string.
 */
wxString bf::class_tree_ctrl::make_pattern() const
{
  wxString pattern( m_pattern->GetValue() );

  if ( pattern.IsEmpty() )
    pattern = wxT("*");

  if ( (pattern[0] != wxT('*')) && (pattern[0] != wxT('?')) )
    pattern = wxT("*") + pattern;

  if ( (pattern[pattern.length() - 1] != wxT('*'))
       && (pattern[pattern.length() - 1] != wxT('?')) )
    pattern += wxT("*");

  return pattern;
} // class_tree_ctrl::make_pattern()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select a class in the tree.
 * \param all_children Tell if we expand the whole subtree (if selected item is
 *        not a leaf).
 */
void bf::class_tree_ctrl::select_class( bool all_children )
{
  wxTreeItemId item = m_tree->GetSelection();

  if ( item.IsOk() )
    {
      if ( m_tree->ItemHasChildren(item) )
        {
          if ( m_tree->IsExpanded(item) )
            {
              if ( all_children )
                m_tree->CollapseAllChildren(item);
              else
                m_tree->Collapse(item);
            }
          else
            {
              if ( all_children )
                m_tree->ExpandAllChildren(item);
              else
                m_tree->Expand(item);
            }
        }
      else
        {
          select_class( wx_to_std_string( m_tree->GetItemText(item) ) );

          wxTreeItemId item = m_tree->GetSelection();
          
          if ( item.IsOk() )
            {
              if ( !m_tree->ItemHasChildren(item) )
                {
                  m_selected_class.SetText( m_tree->GetItemText(item) );
                  unselect_recent_class();
                }
              else
                m_selected_class.SetText( _("") );
            }
          else
            m_selected_class.SetText( _("") );
        }

      show_class_description();
    }
} // class_tree_ctrl::select_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select a class.
 * \param c The class to select.
 */
void bf::class_tree_ctrl::select_class( const std::string& c )
{
  add_recent_used_class( c );
  
  class_selected_event event
    ( c, class_selected_event::class_selected_event_type, GetId() );

  event.SetEventObject(this);
  ProcessEvent(event);
} // class_tree_ctrl::select_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unselect the recent class.
 */
void bf::class_tree_ctrl::unselect_class_in_tree()
{
  wxTreeItemId item = m_tree->GetSelection();
  
  if ( item.IsOk() )
    if ( !m_tree->ItemHasChildren(item) )
      m_tree->UnselectAll();
} // class_tree_ctrl::unselect_class_in_tree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unselect recent class.
 */
void bf::class_tree_ctrl::unselect_recent_class()
{
  if ( m_recent_used_classes_list->GetFirstSelected() != 
       wxNOT_FOUND )
    m_recent_used_classes_list->Select
      (m_recent_used_classes_list->GetFirstSelected(), false);
} // class_tree_ctrl::unselect_recent_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show class description.
 */
void bf::class_tree_ctrl::show_class_description()
{
  if ( ! m_selected_class.GetText().IsEmpty() && m_workspace )
    {
      std::string class_name( wx_to_std_string( m_selected_class.GetText() ) );
      const item_class* cl = 
        m_workspace->get_item_class_pool().get_item_class_ptr(class_name);

      m_class_description->SetLabel( std_to_wx_string(cl->get_description()) );
    }
  else
    m_class_description->SetLabel(wxEmptyString);

  GetSizer()->Layout();
  m_class_description->SetToolTip(m_class_description->GetLabel());
} // class_tree_ctrl::show_class_descrition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when a double click occurs on a tree.
 * \param event This event occured.
 */
void bf::class_tree_ctrl::on_double_click( wxMouseEvent& event )
{
  select_class(event.ShiftDown());
} // class_tree_ctrl::on_double_click()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected class changes.
 * \param event The event that occured.
 */
void bf::class_tree_ctrl::on_class_change( wxTreeEvent& event )
{
  wxTreeItemId item = m_tree->GetSelection();
  
  if ( item.IsOk() )
    {
      m_selected_class.SetText( m_tree->GetItemText(item) );
      unselect_recent_class();
      show_class_description();
    }
} // class_tree_ctrl::on_class_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected class in recent classes list changes.
 * \param event The event that occured.
 */
void bf::class_tree_ctrl::on_recent_class_change( wxListEvent& event )
{
  m_selected_class.SetText( event.GetLabel() );
  unselect_class_in_tree();

  show_class_description();
} // class_tree_ctrl::on_recent_class_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief A class in recent classes list is activated.
 * \param event The event that occured.
 */
void bf::class_tree_ctrl::on_recent_class_activated( wxListEvent& event )
{
  m_selected_class.SetText( event.GetLabel() );
  unselect_class_in_tree();
  select_class( wx_to_std_string( event.GetLabel() ) );
  
  show_class_description();
} // class_tree_ctrl::on_recent_class_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user pressed a keybord key.
 * \param event The keyboard event that occured.
 */
void bf::class_tree_ctrl::on_key_up( wxKeyEvent& event )
{
  switch( event.GetKeyCode() )
    {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
      select_class( event.ShiftDown() );
      break;
    default:
      event.Skip();
    }
} // class_tree_ctrl::on_key_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when the mouse moves over the control.
 * \param event This event occured.
 */
void bf::class_tree_ctrl::on_mouse_move( wxMouseEvent& event )
{
  if ( event.LeftIsDown() )
    {
      if ( ! m_selected_class.GetText().IsEmpty() )
        {
          wxDropSource drag_source( this );
          
          drag_source.SetData( m_selected_class );
          drag_source.DoDragDrop( true );
        }
    }
  else
    {
      wxTreeItemId item =
        m_tree->HitTest( event.GetPosition() );

      if ( item.IsOk() )
        {
          if ( !m_tree->ItemHasChildren(item) && m_workspace )
            {
              const wxString wx_class_name( m_tree->GetItemText(item) );
              const std::string class_name( wx_to_std_string(wx_class_name) );

              const item_class* cl = 
                m_workspace->get_item_class_pool().get_item_class_ptr
                (class_name);

              if ( cl->get_description().empty() )
                m_tree->SetToolTip
                  ( wxString::Format
                    ( _("%s: no description available"),
                      wx_class_name.c_str() ) );
              else
                {
                  const wxString desc = std_to_wx_string(cl->get_description());
                  m_tree->SetToolTip
                    ( wxString::Format
                      ( _("%s: %s"), wx_class_name.c_str(), desc.c_str() ) );
                }
            }
          else
            m_tree->SetToolTip(wxEmptyString);
        }
      else
        m_tree->SetToolTip(wxEmptyString);

      event.Skip();
    }
} // class_tree_ctrl::on_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief The text of the pattern has changed.
 * \param event This event occured.
 */
void bf::class_tree_ctrl::on_pattern_change( wxCommandEvent& WXUNUSED(event) )
{
  fill_tree();
} // class_tree_ctrl::on_pattern_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::class_tree_ctrl::on_size(wxSizeEvent& event)
{
  m_recent_used_classes_list->SetColumnWidth
    ( 0, m_recent_used_classes_list->GetSize().x );

  event.Skip();
} // class_tree_ctrl::on_size()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::class_tree_ctrl, wxPanel) 
  EVT_SIZE( bf::class_tree_ctrl::on_size )
  EVT_TEXT( bf::class_tree_ctrl::IDC_PATTERN,
            bf::class_tree_ctrl::on_pattern_change )
  EVT_TREE_SEL_CHANGED( bf::class_tree_ctrl::IDC_TREE,
                        bf::class_tree_ctrl::on_class_change )
  EVT_LIST_ITEM_ACTIVATED( bf::class_tree_ctrl::IDC_RECENT_USED_CLASSES,
                           bf::class_tree_ctrl::on_recent_class_activated )
  EVT_LIST_ITEM_SELECTED( bf::class_tree_ctrl::IDC_RECENT_USED_CLASSES,
                          bf::class_tree_ctrl::on_recent_class_change )
END_EVENT_TABLE()
