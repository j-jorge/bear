/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::properties_frame class.
 * \author Julien Jorge
 */
#include "bf/properties_frame.hpp"

#include "bf/call_by_field_type.hpp"
#include "bf/gui_level.hpp"
#include "bf/ingame_view.hpp"
#include "bf/ingame_view_frame.hpp"
#include "bf/item_class_pool.hpp"
#include "bf/item_class_selection_dialog.hpp"
#include "bf/item_field_edit.hpp"
#include "bf/windows_layout.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/history/action_group.hpp"
#include "bf/history/action_set_item_class.hpp"
#include "bf/history/action_set_item_field.hpp"
#include "bf/history/action_set_item_fixed_attribute.hpp"
#include "bf/history/action_set_item_id.hpp"

#include <list>

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the action group of the deletion of the field.
 * \param edit The control in which the item instances are taken.
 * \param name The name of the field.
 */
template<typename T>
bf::level_action* bf::properties_frame::field_deleter<T>::operator()
  ( const item_field_edit& edit, const std::string& name ) const
{
  action_group* action =
    new action_group( action_set_item_field<T>::get_action_description() );

  item_field_edit::item_iterator it;
  for ( it=edit.begin(); it!=edit.end(); ++it )
    action->add_action( new action_set_item_field<T>(&(*it), name) );

  return action;
} // properties_frame::field_deleter::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent Pointer to the owner.
 */
bf::properties_frame::properties_frame( wxWindow* parent )
  : wxPanel( parent ), m_windows_layout(NULL), m_last_edited_field_name("")
{
  create_controls();

  Fit();
} // properties_frame::properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the windows layout of the program.
 * \param layout The layout.
 */
void bf::properties_frame::set_window_layout( windows_layout& layout )
{
  m_windows_layout = &layout;
} // properties_frame::set_window_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all edited items
 */
void bf::properties_frame::clear()
{
  add_item(NULL);
} // properties_frame::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item for which we want the properties.
 * \param item The item instance concerned by this window.
 */
void bf::properties_frame::add_item( item_instance* item )
{
  if ( !m_prop->add_item( item ) )
    return;

  fill_controls();
} // properties_frame::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add several items for which we want the properties.
 * \param items The item instances concerned by this window.
 */
void bf::properties_frame::add_items( const std::vector<item_instance*>& items )
{
  if ( !m_prop->add_items( items ) )
    return;

  fill_controls();
} // properties_frame::add_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item for which we want the properties.
 * \param item The item instance to remove.
 */
void bf::properties_frame::remove_item( item_instance* item )
{
  if ( !m_prop->remove_item( item ) )
    return;

  fill_controls();
} // properties_frame::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Edit a field.
 * \param field_name The name of the field to edit.
 */
void  bf::properties_frame::edit_field(const std::string& field_name)
{
  m_prop->create_field_editor(field_name);
} // properties_frame::edit_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh the content of the controls.
 */
void bf::properties_frame::refresh()
{
  m_prop->refresh();
  fill_controls();
} // properties_frame::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there exists a last edited field name.
 */
bool bf::properties_frame::has_last_edited_field_name() const
{
  return !m_last_edited_field_name.empty();
} // properties_frame::has_last_edited_field_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the last edited field name.
 */
const std::string& bf::properties_frame::get_last_edited_field_name() const
{
  return m_last_edited_field_name;
} // properties_frame::get_last_edited_field_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the last edited field name.
 * \param field_name The last edited field name.
 */
void bf::properties_frame::set_last_edited_field_name
( const std::string& field_name )
{
  m_last_edited_field_name = field_name;
} // properties_frame::set_last_edited_field_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the values of the controls.
 */
void bf::properties_frame::fill_controls()
{
  const item_class* c = NULL;

  if ( !m_prop->empty() )
    {
      c = m_prop->get_common_class();

      m_fixed_box->Enable();
      m_id_text->Enable();
      update_controls();
    }
  else
    {
      m_fixed_box->Disable();
      m_id_text->Disable();
    }

  m_id_text->SetBackgroundColour( wxNullColour );

  if ( c!=NULL )
    {
      m_item_class->SetURL( std_to_wx_string( c->get_url()) );
      m_item_class->SetLabel( std_to_wx_string( c->get_class_name()) );

      const std::string desc(c->get_description());

      if ( !desc.empty() )
        m_item_class->SetToolTip( std_to_wx_string(desc) );
      else
        m_item_class->SetToolTip( _("No description available") );

      m_item_class->Refresh();
      FindWindow( IDC_CHANGE_ITEM_CLASS )->Enable();
    }
  else
    {
      m_item_class->SetURL(wxEmptyString);
      m_item_class->SetLabel(wxEmptyString);
      m_item_class->SetToolTip(wxEmptyString);
      m_item_class->Refresh();
      FindWindow( IDC_CHANGE_ITEM_CLASS )->Disable();
    }
} // properties_frame::fill_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the controls.
 */
void bf::properties_frame::update_controls()
{
  CLAW_PRECOND( !m_prop->empty() );

  item_field_edit::item_iterator it=m_prop->begin();
  bool can_be_fixed(it->get_class().get_fixable());
  wxCheckBoxState fixed( it->get_fixed() ? wxCHK_CHECKED : wxCHK_UNCHECKED );

  for (++it; it!=m_prop->end(); ++it)
    {
      if ( can_be_fixed )
        can_be_fixed = it->get_class().get_fixable();

      if ( ( (fixed==wxCHK_UNCHECKED) && it->get_fixed() )
           || ( (fixed==wxCHK_CHECKED) && !it->get_fixed() ) )
        fixed = wxCHK_UNDETERMINED;
    }

  if ( can_be_fixed )
    m_fixed_box->Set3StateValue( fixed );
  else
    m_fixed_box->Disable();

  if ( m_prop->has_single_item() )
    m_id_text->ChangeValue
      ( std_to_wx_string(m_prop->get_single_item().get_id()) );
  else
    m_id_text->ChangeValue( _("Multiple selection") );

  m_id_text->SetBackgroundColour( wxNullColour );
} // properties_frame::update_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::properties_frame::create_controls()
{
  m_item_class =
    new wxHyperlinkCtrl
    ( this, IDC_ITEM_CLASS_NAME, wxEmptyString, wxEmptyString );

  m_prop = new item_field_edit( this, IDC_ITEM_PROPERTIES );
  m_fixed_box =
    new wxCheckBox
    ( this, IDC_FIXED_STATE, _("Fixed"), wxDefaultPosition, wxDefaultSize,
      wxCHK_3STATE );
  m_id_text =
    new wxTextCtrl( this, IDC_TEXT_IDENTIFIER, wxEmptyString, wxDefaultPosition,
                    wxDefaultSize, wxTE_PROCESS_ENTER );
  m_description = new wxStaticText(this, wxID_ANY, wxEmptyString);

  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );
  wxButton* change_item_class =
    new wxButton( this, IDC_CHANGE_ITEM_CLASS, wxT("..."), wxDefaultPosition,
                  wxSize(30, -1) );
  change_item_class->SetToolTip
    ( _("Click to change the class of the selected items. The value of the "
        " fields will be conserved for the ones existing also in the new "
        "class.") );

  s_sizer->Add( m_item_class, 1, wxEXPAND );
  s_sizer->Add( change_item_class, 0, wxEXPAND );

  sizer->Add( s_sizer, 0, wxEXPAND );
  sizer->Add( m_prop, 1, wxEXPAND );
  sizer->AddSpacer(5);

  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add( new wxStaticText(this, wxID_ANY, _("Id:")), 0,
                wxALIGN_CENTRE_VERTICAL | wxALL );
  s_sizer->Add( m_id_text, 1, wxEXPAND );
  s_sizer->AddSpacer(5);
  s_sizer->Add( m_fixed_box, 0, wxEXPAND );

  sizer->Add( s_sizer, 0, wxEXPAND );

  sizer->AddSpacer(5);
  sizer->Add( m_description, 0, wxEXPAND );

  m_id_text->Disable();
  m_fixed_box->Disable();
  change_item_class->Disable();

  SetSizer(sizer);
} // properties_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the identifier of the selected items.
 */
void bf::properties_frame::clear_items_id()
{
  ingame_view* view =
    m_windows_layout->get_current_level_view()->get_ingame_view();
  bool ok=true;

  item_field_edit::item_iterator it;
  for ( it=m_prop->begin(); ok && (it!=m_prop->end()); ++it )
    ok = !view->get_level().get_active_layer().is_prioritized(&(*it));

  if ( !ok )
    {
      wxMessageDialog dlg
        ( this, _("Cannot remove the identifier of a prioritized item."),
          _("Action not allowed"), wxID_OK );

      dlg.ShowModal();
    }
  else
    {
      action_set_item_id::item_collection items;

      for ( it=m_prop->begin(); it!=m_prop->end(); ++it )
        items.push_back( &(*it) );

      view->do_action( new action_set_item_id( items, std::string() ) );
    }
} // properties_frame::clear_items_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the identifier of the selected items.
 */
void bf::properties_frame::set_item_id()
{
  CLAW_PRECOND( !m_id_text->IsEmpty() );

  const std::string id = wx_to_std_string( m_id_text->GetValue() );
  ingame_view* view =
    m_windows_layout->get_current_level_view()->get_ingame_view();

  if ( m_prop->has_single_item() && (id == m_prop->get_single_item().get_id()) )
    return;

  action_set_item_id::item_collection items;

  for ( item_field_edit::item_iterator it=m_prop->begin();
        it!=m_prop->end(); ++it )
    items.push_back( &(*it) );

  view->do_action( new action_set_item_id( items, id ) );
} // properties_frame::set_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a field.
 * \param e The event of changing the value of the field.
 */
template<typename Type>
void bf::properties_frame::on_set_field_value( set_field_value_event<Type>& e )
{
  action_group* action =
    new action_group
    ( action_set_item_field<Type>::get_action_description() );

  item_field_edit::item_iterator it;
  for ( it=m_prop->begin(); it!=m_prop->end(); ++it )
    {
      action->add_action
        ( new action_set_item_field<Type>
          (&(*it), e.get_field_name(), e.get_value()) );


      const type_field& f = m_prop->get_common_field(e.get_field_name());

      if ( f.get_field_type() != type_field::boolean_field_type )
        m_last_edited_field_name = e.get_field_name();
    }

  ingame_view* view =
    m_windows_layout->get_current_level_view()->get_ingame_view();
  view->do_action(action);
} // properties_frame::on_set_field_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::properties_frame::on_close(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
} // properties_frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when an item of a list get the focus.
 * \param event The event.
 */
void bf::properties_frame::on_item_focused(wxListEvent& event)
{
  if ( !m_prop->empty() && (event.GetIndex() != -1) )
    {
      std::string name;

      if( m_prop->get_field_name(event.GetIndex(), name) )
        {
          const type_field& f = m_prop->get_common_field(name);

          m_description->SetLabel( std_to_wx_string(f.get_description()) );
        }
    }
  else
    m_description->SetLabel( wxEmptyString );

  m_description->SetToolTip( m_description->GetLabel() );
} // properties_frame::on_item_focused()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent when the user chek or unchek the fixed box.
 * \param event The event.
 */
void bf::properties_frame::on_change_fixed( wxCommandEvent& WXUNUSED(event) )
{
  ingame_view* view =
    m_windows_layout->get_current_level_view()->get_ingame_view();

  action_group* action =
    new action_group
    ( action_set_item_fixed_attribute::get_action_description() );

  item_field_edit::item_iterator it;
  for ( it=m_prop->begin(); it!=m_prop->end(); ++it )
    action->add_action
      ( new action_set_item_fixed_attribute(&(*it), m_fixed_box->IsChecked()) );

  view->do_action(action);
} // properties_frame::on_change_fixed()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user is changing the identifier of the selecetd item. We change
 *        the background color of the text component to show that the changes
 *        are not validated yet.
 * \param event The event.
 */
void bf::properties_frame::on_change_id( wxCommandEvent& WXUNUSED(event) )
{
  bool modified( true );

  if ( m_prop->has_single_item() )
    modified =
      wx_to_std_string( m_id_text->GetValue() )
      != m_prop->get_single_item().get_id();

  if ( modified )
    m_id_text->SetBackgroundColour( wxColour( wxT("#F0C000") ) );
  else
    m_id_text->SetBackgroundColour( wxNullColour );
} // properties_frame::on_change_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user pressed the "Enter" key in the identifier text field.
 * \param event The event.
 */
void bf::properties_frame::on_validate_id( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_id_text->IsEmpty() )
    clear_items_id();
  else
    set_item_id();
} // properties_frame::on_validate_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user wants to change the class of the item.
 * \param event The event.
 */
void bf::properties_frame::on_change_item_class
( wxCommandEvent& WXUNUSED(event) )
{
  item_class_selection_dialog dlg
    (m_windows_layout->get_item_class_pool(), this);

  if ( dlg.ShowModal() != wxID_OK )
    return;

  action_group* action =
    new action_group( action_set_item_class::get_action_description() );
  bool empty_action(false);

  // remember the items for the upcoming refresh
  std::list<item_instance*> items;

  item_field_edit::item_iterator it;
  for ( it=m_prop->begin(); it!=m_prop->end(); ++it )
    {
      items.push_back(&(*it));

      if ( it->get_class().get_class_name() != dlg.get_class_name() )
        {
          empty_action = false;
          action->add_action
            ( new action_set_item_class
              (&(*it),
               m_windows_layout->get_item_class_pool().get_item_class_ptr
               (dlg.get_class_name()) ) );
        }
    }

  if (empty_action)
    delete action;
  else
    {
      ingame_view* view =
        m_windows_layout->get_current_level_view()->get_ingame_view();

      // force a full refresh
      clear();

      view->do_action(action);

      for ( ; !items.empty(); items.pop_front() )
        add_item(items.front());
    }
} // properties_frame::on_change_item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user clicked on the item class name.
 * \param event The event.
 */
void bf::properties_frame::on_item_class_click( wxHyperlinkEvent& event )
{
  event.Skip(!m_item_class->GetURL().IsEmpty());
} // properties_frame::on_item_class_click()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the action of deleting a field.
 * \param e The deletion event.
 */
void bf::properties_frame::on_delete_field( delete_item_field_event& e )
{
  const type_field& f = m_prop->get_common_field(e.get_field_name());
  call_by_field_type<field_deleter, level_action*> call;

  ingame_view* view =
    m_windows_layout->get_current_level_view()->get_ingame_view();
  view->do_action( call( f, *m_prop, f.get_name() ) );
} // properties_frame::on_delete_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifiers that can be given to a field of type
 *        item_reference_tpe of the edited item.
 * \param e (in/out) The event of the request.
 */
void bf::properties_frame::on_request_item_id( request_item_id_event& e )
{
  const ingame_view* view =
    m_windows_layout->get_current_level_view()->get_ingame_view();
  layer::item_iterator it;
  const layer::item_iterator eit(view->get_active_layer().item_end());
  std::list<std::string> valid_classes;
  e.get_type_field().get_set(valid_classes);

  for (it=view->get_active_layer().item_begin(); it!=eit; ++it)
    if ( !it->get_id().empty() )
      {
        bool in_use(false);

        item_field_edit::item_iterator iti;
        for ( iti=m_prop->begin(); !in_use && (iti!=m_prop->end()); ++iti )
          in_use = (it->get_id() == iti->get_id());

        if (!in_use)
          {
            bool ok(false);
            std::list<std::string>::iterator itv;
            for (itv=valid_classes.begin(); itv!=valid_classes.end(); ++itv)
              ok = ok || (it->get_class().get_class_name() == *itv ) ||
                it->get_class().inherits_from(*itv);

            if ( ok || valid_classes.empty() )
              e.add_id( std_to_wx_string(it->get_id()) );
          }
      }
} // properties_frame::on_request_item_id()

/*----------------------------------------------------------------------------*/
#define ADD_EVENT_SET_FIELD_VALUE(T)                                    \
  EVT_SET_FIELD_VALUE_TEMPLATE( bf::properties_frame::IDC_ITEM_PROPERTIES, \
                                bf::properties_frame,                   \
                                on_set_field_value,                     \
                                T )                                     \
  EVT_SET_FIELD_VALUE_TEMPLATE( bf::properties_frame::IDC_ITEM_PROPERTIES, \
                                bf::properties_frame,                   \
                                on_set_field_value,                     \
                                std::list<T> )

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::properties_frame, wxPanel)
  EVT_CLOSE( bf::properties_frame::on_close )
  EVT_LIST_ITEM_FOCUSED( bf::properties_frame::IDC_ITEM_PROPERTIES,
                         bf::properties_frame::on_item_focused )
  EVT_CHECKBOX( bf::properties_frame::IDC_FIXED_STATE,
                bf::properties_frame::on_change_fixed )
  EVT_TEXT( bf::properties_frame::IDC_TEXT_IDENTIFIER,
            bf::properties_frame::on_change_id )
  EVT_TEXT_ENTER( bf::properties_frame::IDC_TEXT_IDENTIFIER,
                  bf::properties_frame::on_validate_id )
  EVT_BUTTON( bf::properties_frame::IDC_CHANGE_ITEM_CLASS,
              bf::properties_frame::on_change_item_class )
  EVT_HYPERLINK( bf::properties_frame::IDC_ITEM_CLASS_NAME,
                 bf::properties_frame::on_item_class_click )
  EVT_DELETE_ITEM_FIELD( bf::properties_frame::IDC_ITEM_PROPERTIES,
                         bf::properties_frame::on_delete_field )
  EVT_REQUEST_ITEM_IDENTIFIER( bf::properties_frame::IDC_ITEM_PROPERTIES,
                               bf::properties_frame::on_request_item_id )

  ADD_EVENT_SET_FIELD_VALUE( bf::integer_type )
  ADD_EVENT_SET_FIELD_VALUE( bf::u_integer_type )
  ADD_EVENT_SET_FIELD_VALUE( bf::real_type )
  ADD_EVENT_SET_FIELD_VALUE( bf::bool_type )
  ADD_EVENT_SET_FIELD_VALUE( bf::string_type )
  ADD_EVENT_SET_FIELD_VALUE( bf::sprite )
  ADD_EVENT_SET_FIELD_VALUE( bf::any_animation )
  ADD_EVENT_SET_FIELD_VALUE( bf::item_reference_type )
  ADD_EVENT_SET_FIELD_VALUE( bf::font )
  ADD_EVENT_SET_FIELD_VALUE( bf::sample )
  ADD_EVENT_SET_FIELD_VALUE( bf::color )
  ADD_EVENT_SET_FIELD_VALUE( bf::easing_type )
END_EVENT_TABLE()
