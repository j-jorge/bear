/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::layer_list_frame class.
 * \author Julien Jorge
 */
#include "bf/layer_list_frame.hpp"

#include "bf/gui_level.hpp"
#include "bf/ingame_view.hpp"
#include "bf/item_reference_edit.hpp"
#include "bf/layer_properties_frame.hpp"
#include "bf/value_editor_dialog.hpp"
#include "bf/wx_facilities.hpp"

#include "bf/history/action_add_layer.hpp"
#include "bf/history/action_deprioritize.hpp"
#include "bf/history/action_group.hpp"
#include "bf/history/action_move_layer_backward.hpp"
#include "bf/history/action_move_layer_forward.hpp"
#include "bf/history/action_prioritize.hpp"
#include "bf/history/action_remove_layer.hpp"
#include "bf/history/action_resize_layer.hpp"
#include "bf/history/action_set_priority.hpp"
#include "bf/history/action_set_item_position.hpp"

#include "bf/icon/adjust_horizontally.xpm"
#include "bf/icon/delete_filter.xpm"
#include "bf/icon/delete_filter_everywhere.xpm"
#include "bf/icon/edit.xpm"
#include "bf/icon/filter_all_layers.xpm"
#include "bf/icon/filter_current_layer.xpm"

#include "bf/item_filter/item_filter_by_class_name.hpp"
#include "bf/item_filter/item_filter_by_class_name_glob.hpp"

#include <wx/notebook.h>
#include <wx/artprov.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The frame owning this one.
 */
bf::layer_list_frame::layer_list_frame( wxWindow* parent )
  : wxPanel( parent ), m_level_view(NULL), m_layer_list(NULL)
{
  create_controls();
} // layer_list_frame::layer_list_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the level on which we work.
 * \param view The level view in which we take the layers.
 */
void bf::layer_list_frame::set_level_view( ingame_view* view )
{
  m_level_view = view;
  fill();
} // layer_list_frame::set_level_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh the content of the controls.
 */
void bf::layer_list_frame::refresh()
{
  // The priority list is the only part whose content that can change from
  // outside
  if ( m_level_view != NULL )
    if ( !m_level_view->empty() )
      fill_priority_list();
} // layer_list_frame::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::layer_list_frame::create_controls()
{
  wxNotebook* tabs =
    new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP );

  SetSizer( new wxBoxSizer(wxVERTICAL) );

  GetSizer()->Add( create_layer_list( this ), 1, wxEXPAND );

  tabs->AddPage( create_filters_page( tabs ), _("Filters") );
  tabs->AddPage( create_priority_page( tabs ), _("Priority") );

  GetSizer()->Add( tabs, 1, wxEXPAND );
} // layer_list_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the layer list.
 * \param parent The parent window of the resulting window.
 */
wxWindow* bf::layer_list_frame::create_layer_list( wxWindow* parent )
{
  wxWindow* result = new wxPanel( parent, wxID_ANY );

  m_layer_list = new wxCheckListBox( result, IDC_LAYER_LIST );

  wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  // the sizer for the buttons
  wxBoxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_NEW, wxArtProvider::GetBitmap(wxART_NEW), _("New layer")),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_DELETE, wxArtProvider::GetBitmap(wxART_DELETE),
        _("Delete selected layer") ),
      0, wxEXPAND | wxALL, 0 );

  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_CUT, wxArtProvider::GetBitmap(wxART_CUT),
        _("Cut the selected layer") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_COPY, wxArtProvider::GetBitmap(wxART_COPY),
        _("Copy the selected layer") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_PASTE, wxArtProvider::GetBitmap(wxART_PASTE),
        _("Paste the selected layer") ),
      0, wxEXPAND | wxALL, 0 );

  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_UP, wxArtProvider::GetBitmap(wxART_GO_UP),
        _("Move selected layer backward") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, wxID_DOWN, wxArtProvider::GetBitmap(wxART_GO_DOWN),
        _("Move selected layer forward") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      (result, wxID_PROPERTIES, wxBitmap(edit_xpm),
       _("Edit the properties of the selected layer") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      (result, IDC_STRETCH_LAYER, wxBitmap(adjust_horizontally_xpm),
       _("Change the size of the selected layer and adjust the position of its"
         " items") ),
      0, wxEXPAND | wxALL, 0 );

  sizer->Add( s_sizer, 0, wxEXPAND );
  sizer->Add( m_layer_list, 1, wxEXPAND );

  result->SetSizer( sizer );
  return result;
} // create_layer_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the filter list.
 * \param parent The parent window of the resulting window.
 */
wxWindow* bf::layer_list_frame::create_filters_page( wxWindow* parent )
{
  wxWindow* result = new wxPanel( parent, wxID_ANY );

  m_filter_list =
    new wxListBox
    ( result, IDC_FILTER_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL,
      wxLB_MULTIPLE | wxLB_NEEDED_SB );
  m_filter_entry =
    new wxTextCtrl
    ( result, IDC_FILTER_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
      wxTE_PROCESS_ENTER );

  m_filter_entry->SetToolTip
    ( _("Type the name of the only classes you want to see in the editor") );

  wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  wxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add
    ( new wxStaticText
      (result, wxID_ANY, _("Filter:")), 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add( m_filter_entry, 1, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, IDC_APPLY_TO_CURRENT_LAYER, wxBitmap(filter_current_layer_xpm),
        _("Apply filter to the selected layer") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, IDC_APPLY_TO_ALL_LAYERS, wxBitmap(filter_all_layers_xpm),
        _("Apply filter to all layers") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      (result, IDC_DELETE_FILTER, wxBitmap(delete_filter_xpm),
       _("Remove the selected filter from the current layer") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      (result, IDC_DELETE_FILTER_EVERYWHERE,
       wxBitmap(delete_filter_everywhere_xpm),
       _("Remove the selected filter from all layers") ),
      0, wxEXPAND | wxALL, 0 );

  sizer->Add( s_sizer, 0, wxEXPAND );
  sizer->Add( m_filter_list, 1, wxEXPAND );

  result->SetSizer( sizer );

  return result;
} // layer_list_frame::create_filters_page()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for the items to compile in priority.
 * \param parent The parent window of the resulting window.
 */
wxWindow* bf::layer_list_frame::create_priority_page( wxWindow* parent )
{
  wxWindow* result = new wxPanel( parent, wxID_ANY );

  m_priority_list =
    new wxListBox
    ( result, IDC_FILTER_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL,
      wxLB_SINGLE | wxLB_NEEDED_SB );

  wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
  wxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, IDC_NEW_PRIORITY, wxArtProvider::GetBitmap(wxART_NEW),
        _("Insert item")), 0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, IDC_DELETE_PRIORITY, wxArtProvider::GetBitmap(wxART_DELETE),
        _("Remove the selected item") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, IDC_INCREASE_PRIORITY, wxArtProvider::GetBitmap(wxART_GO_UP),
        _("Increase the priority") ),
      0, wxEXPAND | wxALL, 0 );
  s_sizer->Add
    ( wx_new_bitmap_button
      ( result, IDC_DECREASE_PRIORITY, wxArtProvider::GetBitmap(wxART_GO_DOWN),
        _("Decrease the priority") ),
      0, wxEXPAND | wxALL, 0 );

  sizer->Add( s_sizer, 0, wxEXPAND );
  sizer->Add( m_priority_list, 1, wxEXPAND );

  result->SetSizer(sizer);

  return result;
} // layer_list_frame::create_priority_page()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the list of layers.
 */
void bf::layer_list_frame::clear()
{
  m_layer_list->Clear();
  m_filter_list->Clear();
  m_filter_entry->Clear();
  m_priority_list->Clear();
} // layer_list_frame::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the list with the layers of the level.
 */
void bf::layer_list_frame::fill()
{
  clear();

  if ( m_level_view != NULL )
    if ( !m_level_view->empty() )
      {
        fill_layer_list();
        fill_filter_list();
        fill_priority_list();
    }
} // layer_list_frame::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the layer list with the layers of the level.
 */
void bf::layer_list_frame::fill_layer_list()
{
  CLAW_PRECOND( m_level_view != NULL );
  CLAW_PRECOND( !m_level_view->empty() );

  const gui_level& lvl = m_level_view->get_level();
  const std::size_t i = lvl.get_active_layer_index();

  for (unsigned int i=0; i!=lvl.layers_count(); ++i)
    {
      m_layer_list->Append
        ( std_to_wx_string( lvl.get_layer(i).get_displayed_name() ) );
      m_layer_list->Check( i, lvl.layer_is_visible(i) );
    }

  m_layer_list->SetSelection(i);
} // layer_list_frame::fill_layer_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the filter list with the filters of the level.
 */
void bf::layer_list_frame::fill_filter_list()
{
  CLAW_PRECOND( m_level_view != NULL );
  CLAW_PRECOND( !m_level_view->empty() );

  const gui_level& lvl = m_level_view->get_level();
  const std::size_t i = lvl.get_active_layer_index();

  for ( layer::const_filter_iterator it=lvl.get_layer(i).filter_begin();
        it!=lvl.get_layer(i).filter_end(); ++it )
    m_filter_list->Append( std_to_wx_string( it->get_description() ) );
} // layer_list_frame::fill_filter_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the layer list of the items to compile in priority.
 */
void bf::layer_list_frame::fill_priority_list()
{
  CLAW_PRECOND( m_level_view != NULL );
  CLAW_PRECOND( !m_level_view->empty() );

  const gui_level& lvl = m_level_view->get_level();
  std::size_t i = lvl.get_active_layer_index();
  std::vector<item_instance*> items(lvl.get_layer(i).get_prioritized_items());

  for ( i=0; (i!=items.size()) && (i<m_priority_list->GetCount()); ++i )
    m_priority_list->SetString( i, std_to_wx_string( items[i]->get_id() ) );

  if ( i != m_priority_list->GetCount() )
    while ( i!=m_priority_list->GetCount() )
      m_priority_list->Delete(i);
  else
    for ( ; i!=items.size(); ++i )
      m_priority_list->Append( std_to_wx_string( items[i]->get_id() ) );
} // layer_list_frame::fill_priority_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a filter corresponding to the text in the filter entry and
 *        clear the entry.
 */
bf::item_filter bf::layer_list_frame::create_filter_from_entry() const
{
  const std::string filter = wx_to_std_string( m_filter_entry->GetValue() );
  const std::string pattern_chars("*?.");

  m_filter_entry->Clear();

  if ( filter.empty() )
    return item_filter();
  else if ( filter.find_first_of( pattern_chars ) != std::string::npos )
    {
      std::string pattern(filter);

      if ( pattern_chars.find_first_of(filter[0]) == std::string::npos )
        pattern = '*' + pattern;

      if ( pattern_chars.find_first_of(filter[filter.length() - 1])
           == std::string::npos )
        pattern +=  '*';

      return item_filter_by_class_name_glob(pattern);
    }
  else
    return item_filter_by_class_name(filter);
} // layer_list_frame::create_filter_from_entry()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove selected filters from the list of layers, but not in the
 *        layers.
 */
void bf::layer_list_frame::remove_selected_filters()
{
  std::size_t i=m_filter_list->GetCount();

  while ( i!=0 )
    {
      --i;

      if ( m_filter_list->IsSelected(i) )
        m_filter_list->Delete(i);
    }
} // layer_list_frame::remove_selected_filters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Changes the size of a layer and adjust the position of its items.
 * \param index The index of the layer to resize.
 * \param fit_level Tells to assign the size of the level.
 * \param width The width of the layer, if fit_level is false.
 * \param height The height of the layer, if false is false.
 */
bf::level_action* bf::layer_list_frame::create_stretch_layer_action
( unsigned int index, bool fit_level, unsigned int width,
  unsigned int height ) const
{
  level& lvl( m_level_view->get_level() );

  const unsigned int w = fit_level ? lvl.get_width() : width;
  const unsigned int h = fit_level ? lvl.get_height() : height;

  layer& lay( lvl.get_layer(index) );

  const double rx = (double)w / lay.get_width();
  const double ry = (double)h / lay.get_height();

  action_group* result = new action_group( _("Stretch layer") );

  for ( layer::item_iterator it=lay.item_begin_no_filter();
        it!=lay.item_end_no_filter(); ++it )
    result->add_action
      ( new action_set_item_position
        ( &(*it), rx * it->get_rendering_parameters().get_left(),
          ry * it->get_rendering_parameters().get_bottom() ) );

  result->add_action
    ( new action_resize_layer
      ( fit_level, w, h, lay.get_class_name(), 
        lay.get_name(), lay.get_tag(), index ) );

  return result;
} // layer_list_frame::create_stretch_layer_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the selected layer from its containing level.
 */
void bf::layer_list_frame::delete_selected_layer()
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  m_level_view->do_action
    ( new action_remove_layer
      ( m_level_view->get_level().get_active_layer_index() ) );
  fill();
} // layer_list_frame::delete_selected_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the selected layer in the clipboard.
 */
void bf::layer_list_frame::copy_selected_layer() const
{
  if ( !m_level_view->empty() )
    m_level_view->get_clipboard().copy_layer
      ( m_level_view->get_active_layer() );
} // layer_list_frame::copy_selected_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Applies the visibility of a given layer to all layers with the same
 *        tag.
 * \param index The selected item.
 */
void bf::layer_list_frame::diffuse_tag_visibility( int index )
{
  if ( m_level_view == NULL )
    return;

  const std::string tag( m_level_view->get_level().get_layer(index).get_tag() );
  const bool visible( m_layer_list->IsChecked(index) );
  
  for (unsigned int i=0; i!=m_layer_list->GetCount(); ++i)
    if ( m_level_view->get_level().get_layer(i).get_tag() == tag )
      {
        m_layer_list->Check( i, visible );
        m_level_view->get_level().set_layer_visibility( i, visible );
      }

  m_level_view->Refresh();
} // layer_list_frame::diffuse_tag_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on a layer menu entry.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_new_layer( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  layer_properties_frame dlg(this);

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_level_view->do_action
        ( new action_add_layer
          ( new layer
            ( dlg.get_layer_class_name(), dlg.get_layer_name(),
              dlg.get_layer_fits_level(),
              dlg.get_layer_width(), dlg.get_layer_height(),
              dlg.get_tag() ),
            m_level_view->get_level().layers_count() ) );

      fill();
    }
} // layer_list_frame::on_new_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "delete".
 * \param event This event occured.
 */
void bf::layer_list_frame::on_delete( wxCommandEvent& WXUNUSED(event) )
{
  delete_selected_layer();
} // layer_list_frame::on_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "Properties".
 * \param event This event occured.
 */
void bf::layer_list_frame::on_show_properties( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_layer_list->GetSelection();

  if (index != wxNOT_FOUND)
    {
      layer_properties_frame dlg(this);
      dlg.fill_from( m_level_view->get_level().get_active_layer() );
          
      if ( dlg.ShowModal() == wxID_OK )
        {
          m_level_view->do_action
            ( new action_resize_layer
              ( dlg.get_layer_fits_level(), dlg.get_layer_width(),
                dlg.get_layer_height(), dlg.get_layer_class_name(),
                dlg.get_layer_name(), dlg.get_tag(),
                m_level_view->get_level().get_active_layer_index() ) );
              
          m_level_view->Refresh();
          fill();
        }
    }
} // layer_list_frame::on_show_properties()

/*----------------------------------------------------------------------------*/
/**
 * \brief Changes the size of a layer and adjust the position of its items.
 * \param event The event that triggers the stretching.
 */
void bf::layer_list_frame::on_stretch_layer( wxCommandEvent& WXUNUSED(event) )
{
  int index = m_layer_list->GetSelection();

  if (index != wxNOT_FOUND)
    {
      layer_properties_frame dlg(this);
      dlg.fill_from( m_level_view->get_level().get_active_layer() );

      if ( dlg.ShowModal() == wxID_OK )
        {
          m_level_view->do_action
            ( create_stretch_layer_action
              ( m_level_view->get_level().get_active_layer_index(),
                dlg.get_layer_fits_level(), dlg.get_layer_width(),
                dlg.get_layer_height() ) );

          m_level_view->Refresh();

          if ( ! m_level_view->get_level().check_item_position() )
            m_level_view->show_item_position_error(this);
        }
    }
} // layer_list_frame::on_stretch_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the visibility of a layer.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_toggle_layer( wxCommandEvent& event )
{
  const int index( event.GetSelection() );

  if ( wxGetKeyState(WXK_F2) )
    diffuse_tag_visibility(index);
  else
    m_level_view->get_level().set_layer_visibility
      ( index, m_layer_list->IsChecked(index) );

  m_level_view->Refresh();
} // layer_list_frame::on_toggle_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform all windows about the selected layer.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_select_layer( wxCommandEvent& event )
{
  if ( event.IsSelection() )
    if ( m_level_view != NULL )
      m_level_view->set_active_index(m_layer_list->GetSelection());
} // layer_list_frame::on_select_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "up".
 * \param event This event occured.
 */
void bf::layer_list_frame::on_move_up( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  m_level_view->do_action
    ( new action_move_layer_backward
      ( m_level_view->get_level().get_active_layer_index() ) );
  fill();
} // layer_list_frame::on_move_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the button "down".
 * \param event This event occured.
 */
void bf::layer_list_frame::on_move_down( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  m_level_view->do_action
    ( new action_move_layer_forward
      ( m_level_view->get_level().get_active_layer_index() ) );
  fill();
} // layer_list_frame::on_move_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_close(wxCloseEvent& event)
{
  if ( event.CanVeto() )
    {
      Hide();
      event.Veto();
    }
} // layer_list_frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a filter in the current layer.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_insert_filter( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  item_filter filter = create_filter_from_entry();

  if ( filter != item_filter() )
    {
      m_level_view->get_level().get_active_layer().add_filter(filter);
      fill();
      m_level_view->Refresh();
    }
} // layer_list_frame::on_insert_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a filter in all layers.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_multiple_insert_filter
( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  item_filter filter = create_filter_from_entry();

  if ( filter != item_filter() )
    {
      std::size_t i;

      for ( i=0; i!=m_level_view->get_level().layers_count(); ++i )
        m_level_view->get_level().get_layer(i).add_filter(filter);

      fill();
      m_level_view->Refresh();
    }
} // layer_list_frame::on_multiple_insert_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove selected filters from the layers.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_delete_filter( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  const layer& lay( m_level_view->get_level().get_active_layer() );

  layer::const_filter_iterator it=lay.filter_begin();
  const layer::const_filter_iterator eit=lay.filter_end();
  std::size_t i=0;

  while ( it!=eit )
    {
      const item_filter f(*it);
      const bool erase( m_filter_list->IsSelected(i) );
      ++it; ++i;

      if ( erase )
        m_level_view->get_level().get_active_layer().remove_filter(f);
    }

  remove_selected_filters();

  fill();
  m_level_view->Refresh();
} // layer_list_frame::on_delete_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove selected filters from the layers.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_delete_filter_everywhere
( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  const layer& lay( m_level_view->get_level().get_active_layer() );

  layer::const_filter_iterator it=lay.filter_begin();
  const layer::const_filter_iterator eit=lay.filter_end();
  std::size_t i=0;

  while ( it!=eit )
    {
      const item_filter f(*it);
      const bool erase( m_filter_list->IsSelected(i) );
      ++it; ++i;

      if ( erase )
        for ( std::size_t j=0; j!=m_level_view->get_level().layers_count();
              ++j )
          m_level_view->get_level().get_layer(j).remove_filter(f);
    }

  remove_selected_filters();

  fill();
  m_level_view->Refresh();
} // layer_list_frame::on_delete_filter_everywhere()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new item in the priority list.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_new_priority( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  const layer& lay( m_level_view->get_level().get_active_layer() );
  std::list<item_instance*> items = lay.get_identified_items();
  std::list<item_instance*>::const_iterator it;
  wxArrayString id;

  for ( it=items.begin(); it!=items.end(); ++it )
    if ( !(*it)->get_id().empty() && !lay.is_prioritized(*it) )
      id.Add( std_to_wx_string((*it)->get_id()) );

  value_editor_dialog<item_reference_edit> dlg
    (*this, _("Item reference"), id, item_reference_type() );

  if ( dlg.ShowModal() == wxID_OK )
    {
      const layer::item_iterator it =
        lay.find_item_by_id( dlg.get_value().get_value() );

      if ( it != lay.item_end_no_filter() )
        {
          m_level_view->do_action
            ( new action_prioritize
              ( &(*it), m_level_view->get_level().get_active_layer_index() ) );
          fill_priority_list();
        }
    }
} // layer_list_frame::on_new_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reove an item from the priority list.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_delete_priority( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  const int index=m_priority_list->GetSelection();

  if ( index == wxNOT_FOUND )
    return;

  const layer& lay( m_level_view->get_level().get_active_layer() );

  m_level_view->do_action
    ( new action_deprioritize
      ( lay.get_prioritized_items()[index],
        m_level_view->get_level().get_active_layer_index() ) );
  fill_priority_list();
} // layer_list_frame::on_delete_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new item in the priority list.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_decrease_priority
( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  const int index=m_priority_list->GetSelection();

  if ( (index == wxNOT_FOUND)
       || (index == (int)m_priority_list->GetCount() - 1) )
    return;

  const layer& lay( m_level_view->get_level().get_active_layer() );

  m_level_view->do_action
    ( new action_set_priority
      ( lay.get_prioritized_items()[index],
        m_level_view->get_level().get_active_layer_index(), index+1 ) );
  fill_priority_list();
  m_priority_list->SetSelection(index+1);
} // layer_list_frame::on_decrease_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new item in the priority list.
 * \param event This event occured.
 */
void bf::layer_list_frame::on_increase_priority
( wxCommandEvent& WXUNUSED(event) )
{
  if (m_level_view == NULL)
    return;

  if ( m_level_view->empty() )
    return;

  const int index=m_priority_list->GetSelection();

  if ( (index == wxNOT_FOUND) || (index == 0) )
    return;

  const layer& lay( m_level_view->get_level().get_active_layer() );

  m_level_view->do_action
    ( new action_set_priority
      ( lay.get_prioritized_items()[index],
        m_level_view->get_level().get_active_layer_index(), index-1 ) );
  fill_priority_list();
  m_priority_list->SetSelection(index-1);
} // layer_list_frame::on_increase_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the selected layer in the clipboard and remove it from the level.
 * \param event The event that triggers the operation.
 */
void bf::layer_list_frame::on_cut( wxCommandEvent& event )
{
  copy_selected_layer();
  delete_selected_layer();
} // layer_list_frame::on_cut()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the selected layer in the clipboard.
 * \param event The event that triggers the operation.
 */
void bf::layer_list_frame::on_copy( wxCommandEvent& event )
{
  copy_selected_layer();
} // layer_list_frame::on_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Paste a previously copied layer in the cuurrent level.
 * \param event The event that triggers the operation.
 */
void bf::layer_list_frame::on_paste( wxCommandEvent& event )
{
  if ( m_level_view->get_clipboard().has_layer() )
    {
      m_level_view->do_action
        ( new action_add_layer
          ( new layer( m_level_view->get_clipboard().get_layer() ),
            m_level_view->get_level().layers_count() ) );

      fill();
    }
} // layer_list_frame::on_paste()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::layer_list_frame, wxPanel)
  EVT_BUTTON( wxID_NEW, bf::layer_list_frame::on_new_layer )
  EVT_BUTTON( wxID_DELETE, bf::layer_list_frame::on_delete )
  EVT_BUTTON( wxID_CUT, bf::layer_list_frame::on_cut )
  EVT_BUTTON( wxID_COPY, bf::layer_list_frame::on_copy )
  EVT_BUTTON( wxID_PASTE, bf::layer_list_frame::on_paste )
  EVT_BUTTON( wxID_UP, bf::layer_list_frame::on_move_up )
  EVT_BUTTON( wxID_DOWN, bf::layer_list_frame::on_move_down )
  EVT_BUTTON( wxID_PROPERTIES, bf::layer_list_frame::on_show_properties )
  EVT_BUTTON( bf::layer_list_frame::IDC_STRETCH_LAYER,
              bf::layer_list_frame::on_stretch_layer )

  EVT_LISTBOX_DCLICK( bf::layer_list_frame::IDC_LAYER_LIST,
                      bf::layer_list_frame::on_show_properties )
  EVT_CHECKLISTBOX( bf::layer_list_frame::IDC_LAYER_LIST,
                    bf::layer_list_frame::on_toggle_layer )
  EVT_LISTBOX( bf::layer_list_frame::IDC_LAYER_LIST,
               bf::layer_list_frame::on_select_layer )

  EVT_CLOSE( bf::layer_list_frame::on_close )

  EVT_TEXT_ENTER( bf::layer_list_frame::IDC_FILTER_ENTRY,
                  bf::layer_list_frame::on_insert_filter )
  EVT_BUTTON( bf::layer_list_frame::IDC_APPLY_TO_CURRENT_LAYER,
              bf::layer_list_frame::on_insert_filter )
  EVT_BUTTON( bf::layer_list_frame::IDC_APPLY_TO_ALL_LAYERS,
              bf::layer_list_frame::on_multiple_insert_filter )
  EVT_BUTTON( bf::layer_list_frame::IDC_DELETE_FILTER,
              bf::layer_list_frame::on_delete_filter )
  EVT_BUTTON( bf::layer_list_frame::IDC_DELETE_FILTER_EVERYWHERE,
              bf::layer_list_frame::on_delete_filter_everywhere )

  EVT_BUTTON( bf::layer_list_frame::IDC_NEW_PRIORITY,
              bf::layer_list_frame::on_new_priority )
  EVT_BUTTON( bf::layer_list_frame::IDC_DELETE_PRIORITY,
              bf::layer_list_frame::on_delete_priority )
  EVT_BUTTON( bf::layer_list_frame::IDC_DECREASE_PRIORITY,
              bf::layer_list_frame::on_decrease_priority )
  EVT_BUTTON( bf::layer_list_frame::IDC_INCREASE_PRIORITY,
              bf::layer_list_frame::on_increase_priority )
END_EVENT_TABLE()
