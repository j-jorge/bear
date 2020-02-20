/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_frame class.
 * \author Julien Jorge
 */
#include "bf/model_frame.hpp"

#include "bf/gui_model.hpp"
#include "bf/mark.hpp"
#include "bf/model_file_compiler.hpp"
#include "bf/model_view.hpp"
#include "bf/model_view_ctrl.hpp"
#include "bf/path_configuration.hpp"
#include "bf/error_check_model_dialog.hpp"
#include "bf/windows_layout.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/history/action_add_snapshot.hpp"
#include "bf/history/action_remove_snapshot.hpp"
#include "bf/history/action_move_snapshot.hpp"
#include "bf/history/action_group.hpp"
#include "bf/history/action_modify_placement.hpp"
#include "bf/history/action_modify_snapshot_size.hpp"
#include "bf/xml/model_file.hpp"

#include "bf/icon/compile.xpm"
#include "bf/icon/graphism.xpm"
#include "bf/icon/text.xpm"
#include "bf/icon/wireframe.xpm"
#include "bf/icon/zoom_fit.xpm"
#include "bf/icon/zoom_in.xpm"
#include "bf/icon/zoom_out.xpm"
#include "bf/icon/zoom_original.xpm"

#include "bf/icons.hpp"

#include "bf/xml/model_snapshot_node.hpp"

#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/sstream.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>

#include <fstream>
#include <limits>

/*----------------------------------------------------------------------------*/
const unsigned int bf::model_frame::s_button_height = 25;
const unsigned int bf::model_frame::s_button_width = 25;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param layout The layout of the windows of the program.
 * \param mdl The model.
 * \param model_file The path of the file from which a model was read.
 * The model will be deleted in the destructor.
 */
bf::model_frame::model_frame
( windows_layout& layout, gui_model* mdl, const wxString& model_file )
  : wxFrame( layout.get_root_window(), wxID_ANY, _("New model") ),
    m_layout(layout), m_history(mdl),  m_model_file(model_file),
    m_changed(false), m_slider(NULL), m_new_button(NULL), m_delete_button(NULL),
    m_snapshot_copy_button(NULL), m_snapshot_paste_button(NULL),
    m_next_change_button(NULL), m_previous_change_button(NULL),
    m_timer(this, ID_TIMER)
{
  CLAW_PRECOND(mdl != NULL);

  std::string w =
    path_configuration::get_instance().search_workspace
    ( wx_to_std_string( model_file ) );
  m_workspace = workspace_environment( w );

  m_layout.add_model_frame(*this);

  create_controls();
  fill();

  set_changed(false);
} // model_frame::model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path of the file in which the model is saved.
 */
const wxString& bf::model_frame::get_model_file() const
{
  return m_model_file;
} // model_frame::get_model_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the model is changed or not.
 * \param b Tell if the model is changed or not.
 */
void bf::model_frame::set_changed( bool b )
{
  m_changed = b;
  make_title();
} // model_frame::set_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the model has changes that are not saved.
 */
bool bf::model_frame::is_changed() const
{
  return m_changed;
} // model_frame::is_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the model contains an action.
 */
bool bf::model_frame::empty() const
{
  return m_history.get_model().empty();
} // model_frame::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an action.
 * \param action The action to do.
 */
void bf::model_frame::do_action( model_action* action )
{
  CLAW_PRECOND( action != NULL );

  if ( m_history.do_action(action) )
    {
      set_changed( true );
      update_layout();
      fill();
    }
} // model_frame::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the active action.
 */
void bf::model_frame::set_active_action( const std::string& action_name )
{
  CLAW_PRECOND( m_history.get_model().has_action(action_name) );

  m_history.get_model().set_active_action(action_name);
  set_date(0);

  update_layout();
  m_layout.update_action();

  make_title();
} // model_frame::set_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the name of the active action.
 */
void bf::model_frame::reset_active_action()
{
  m_history.get_model().reset_active_action();
  m_history.get_model().clear_selection();
  set_date(0);

  update_layout();

  make_title();
  fill();
} // model_frame::set_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selected mark.
 * \param label The label of the selected mark.
 */
void bf::model_frame::set_mark_selection( const std::string& label)
{
  m_history.get_model().set_bounding_box_selection(false);

  if ( label.empty() )
    m_history.get_model().clear_selection();
  else
    m_history.get_model().set_mark_selection(label);

  m_layout.update_mark();
  fill();
} // model_frame::set_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selection on the bounding box.
 */
void bf::model_frame::set_bounding_box_selection()
{
  m_history.get_model().clear_selection();
  m_history.get_model().set_bounding_box_selection(true);

  m_layout.update_mark();
  fill();
} // model_frame::set_bounding_box_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the selected mark.
 */
void bf::model_frame::clear_selection()
{
  m_history.get_model().clear_selection();
  m_layout.update_mark();
  fill();
} // model_frame::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current date.
 * \param date The new date.
 */
void bf::model_frame::set_date( double date )
{
  get_model().set_date(date);
  m_slider->set_value(date);
  fill();
} // model_frame::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the model.
 */
bool bf::model_frame::save()
{
  bool result = false;

  if ( m_model_file.empty() )
    result = save_as();
  else
    result = effective_save();

  return result;
} // model_frame::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the model and ask the file to the user.
 */
bool bf::model_frame::save_as()
{
  bool result = false;
  wxFileDialog dlg( this, _("Choose a file"), wxEmptyString, m_model_file,
                    _("Model files (*.mdl)|*.mdl"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_model_file = dlg.GetPath();
      result = effective_save();
    }

  return result;
} // model_frame::save_as()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the model on which we are working.
 */
const bf::gui_model& bf::model_frame::get_model() const
{
  return m_history.get_model();
} // model_frame::get_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the model on which we are working.
 */
bf::gui_model& bf::model_frame::get_model()
{
  return m_history.get_model();
} // model_frame::get_model()


/*----------------------------------------------------------------------------*/
/**
 * \brief Get the workspace environment.
 */
bf::workspace_environment& bf::model_frame::get_workspace()
{
  return m_workspace;
} // model_frame::get_workspace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Undo the last change.
 */
void bf::model_frame::undo()
{
  if ( m_history.can_undo() )
    {
      m_history.undo();
      set_changed( !m_history.model_is_saved() );
      update_layout();
    }
} // model_frame::undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cancel an undo.
 */
void bf::model_frame::redo()
{
  if ( m_history.can_redo() )
    {
      m_history.redo();
      set_changed( !m_history.model_is_saved() );
      update_layout();
    }
} // model_frame::redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make the title of the window.
 */
void bf::model_frame::make_title()
{
  if ( m_model_file.empty() )
    SetTitle( _("New model") );
  else
    SetTitle( wxFileName(m_model_file).GetName() );

  if ( m_history.get_model().has_active_action() )
    SetTitle
      ( GetTitle() + wxT(" (") +
        wxFileName
        ( std_to_wx_string
          ( m_history.get_model().get_active_action().get_name())).GetName()
        + wxT(")") );

  if ( m_changed )
    SetTitle( GetTitle() + wxT("*") );
} // model_frame::make_title()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the model without asking anything.
 */
bool bf::model_frame::effective_save()
{
  bool result = false;

  std::string std_path( wx_to_std_string(m_model_file) );
  std::ofstream f( std_path.c_str() );

  if (f)
    {
      result = true;
      m_changed = false;

      xml::model_file writer;
      writer.save(get_model(), f);

      make_title();
    }
  else
    {
      wxMessageDialog dlg
        ( this, _("Error"), _("Can't open the model file."), wxOK );

      dlg.ShowModal();
    }

  return result;
} // model_frame::effective_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 */
void bf::model_frame::create_controls()
{
  create_member_controls();
  create_sizer_controls();
  create_accelerators();
  create_menu();
  create_toolbar();

  const int bar_sizes[3] = { -1, 100, 100 };

  wxStatusBar* bar = new wxStatusBar(this, wxID_ANY);
  bar->SetFieldsCount(3, bar_sizes);
  SetStatusBar(bar);
} // model_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 */
void bf::model_frame::create_member_controls()
{
  m_slider = new slider_with_ticks(this, ID_SLIDER, 0, 0, 100);

  m_new_button = new wxBitmapButton
    ( this, ID_NEW_SNAPSHOT,
      wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU, wxSize(16, 16)) );

  m_delete_button = new wxBitmapButton
    ( this, ID_DELETE_SNAPSHOT,
      wxArtProvider::GetBitmap(wxART_DELETE, wxART_MENU, wxSize(16, 16)) );

  m_snapshot_copy_button = new wxBitmapButton
    ( this, ID_COPY_SNAPSHOT,
      wxArtProvider::GetBitmap(wxART_COPY, wxART_MENU, wxSize(16, 16)) );

  m_snapshot_paste_button = new wxBitmapButton
    ( this, ID_PASTE_SNAPSHOT,
      wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU, wxSize(16, 16)) );

  m_next_change_button = new wxBitmapButton
    ( this, ID_NEXT_CHANGE,
      wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_MENU, wxSize(16, 16)) );

  m_previous_change_button = new wxBitmapButton
    ( this, ID_PREVIOUS_CHANGE,
      wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_MENU, wxSize(16, 16)) );

  m_play_button = new wxBitmapButton
    (this, ID_PLAY_STOP, wxBitmap(player_play_xpm));

  m_model_view = 
    new model_view_ctrl(*this, ID_MODEL_VIEW, get_model(), m_workspace);
} // model_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::model_frame::create_sizer_controls()
{
  wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);

  wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);

  h_sizer->Add( m_play_button );
  h_sizer->Add
    ( new wxBitmapButton(this, ID_START, wxBitmap(player_start_xpm)) );

  h_sizer->Add( m_slider, 1, wxEXPAND);
  h_sizer->Add( m_new_button );
  h_sizer->Add( m_delete_button );
  h_sizer->Add( m_snapshot_copy_button );
  h_sizer->Add( m_snapshot_paste_button );
  h_sizer->Add( m_previous_change_button );
  h_sizer->Add( m_next_change_button );

  v_sizer->Add( m_model_view, 1, wxEXPAND );
  v_sizer->Add( h_sizer, 0, wxEXPAND );

  SetSizer(v_sizer);
  SetAutoLayout(true);
} // model_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the popup menu.
 */
void bf::model_frame::create_menu()
{
  wxMenuBar* bar = new wxMenuBar();

  m_popup_menu.Append
    ( wxID_ANY, _("&Model"), create_model_menu(), _("Options of the model.") );
  bar->Append( create_model_menu(), _("&Model") );

  m_popup_menu.Append
    ( wxID_ANY, _("&Edit"), create_edit_menu(), _("Edit options.") );
  bar->Append( create_edit_menu(), _("&Edit") );

  m_popup_menu.Append
    ( wxID_ANY, _("&View"), create_view_menu(), _("Options of the view.") );
  bar->Append( create_view_menu(), _("&View") );

  SetMenuBar(bar);
} // model_frame::create_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the model menu.
 */
wxMenu* bf::model_frame::create_model_menu() const
{
  wxMenu* result = new wxMenu();

  result->Append( wxID_SAVE, _("&Save"), _("Save the current model.") );
  result->Append
    ( wxID_SAVEAS, _("Save &as...\tCTRL+SHIFT+S"),
      _("Save the current model under a new name.") );
  result->AppendSeparator();
  result->Append
    ( ID_CHECK_MODEL, _("Check the &validty of the model\tF8"),
      _("Check the validity of the model."));
  result->Append
    ( ID_COMPILE_MODEL, _("C&ompile the model\tF9"), _("Compile the model."));
  result->AppendSeparator();
  result->Append( wxID_CLOSE, _("&Close"), _("Close the model.") );

  return result;
} // model_frame::create_model_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the edit menu.
 */
wxMenu* bf::model_frame::create_edit_menu() const
{
  wxMenu* result = new wxMenu();

  result->Append
    ( wxID_UNDO, _("&Undo\tCTRL+Z"), _("Undo the last operation.") );
  result->Append
    ( wxID_REDO, _("&Redo\tCTRL+Y"), _("Redo the last undone operation.") );
  result->AppendSeparator();

  result->Append
    ( wxID_CUT, _("Cu&t"), _("Move the selection to the clipboard.") );
  result->Append
    ( wxID_COPY, _("&Copy"), _("Copy the selection to the clipboard.") );
  result->Append
    ( wxID_PASTE, _("Paste"), _("Paste data from the clipboard.") );
  result->Append
    ( wxID_DELETE, _("&Delete\tDEL"), _("Delete the selection.") );

  result->AppendSeparator();
  result->Append
    ( ID_NEW_SNAPSHOT, _("&New snapshot\tCTRL+INS"), _("New snapshot.") );
  result->Append
    ( ID_DELETE_SNAPSHOT, _("&Delete snapshot\tCTRL+DEL"),
      _("Delete snasphot.") );

  return result;
} // model_frame::create_edit_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the view menu.
 */
wxMenu* bf::model_frame::create_view_menu() const
{
  wxMenu* result = new wxMenu();

  result->AppendCheckItem
    ( ID_WIREFRAME, _("&Wireframe\tW"),
      _("Display the bounding box and the marks.") );
  result->AppendCheckItem
    ( ID_GRAPHISM, _("&Graphic\tg"), _("Display the sprites of the marks.") );
  result->AppendCheckItem
    ( ID_TEXT, _("&Text\tg"), _("Display the name of the marks.") );

  result->AppendSeparator();

  wxMenu* sub_menu = new wxMenu;

  wx_menu_append_item
    ( sub_menu, wxID_ZOOM_IN, _("Zoom more\tCtrl+Wheel up"),
      _("Increase the zoom ratio."), wxBitmap(zoom_in_xpm) );
  wx_menu_append_item
    ( sub_menu, wxID_ZOOM_OUT, _("Zoom less\tCtrl+Wheel down"),
      _("Decrease the zoom ratio."), wxBitmap(zoom_out_xpm) );
  wx_menu_append_item
    ( sub_menu,  wxID_ZOOM_100, _("No zoom"), _("Remove the zoom ratio."),
      wxBitmap(zoom_original_xpm) );
  wx_menu_append_item
    ( sub_menu,  wxID_ZOOM_FIT, _("Zoom fit"), _("Zoom fit"),
      wxBitmap(zoom_fit_xpm) );

  result->Append(wxID_ANY, _("&Zoom"), sub_menu, _("Adjust the zoom ratio.") );

  return result;
} // model_frame::create_view_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the toolbar.
 */
void bf::model_frame::create_toolbar()
{
  wxToolBar* bar = CreateToolBar();

  bar->AddTool
    ( wxID_SAVE, _("&Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE),
      _("Save") );
  bar->AddTool
    ( ID_COMPILE_MODEL, _("&Compile"), wxBitmap(compile_xpm), _("Compile") );
  bar->AddSeparator();
  bar->AddTool
    ( wxID_CUT, _("Cu&t"), wxArtProvider::GetBitmap(wxART_CUT), _("Cut") );
  bar->AddTool
    ( wxID_COPY, _("&Copy"), wxArtProvider::GetBitmap(wxART_COPY), _("Copy") );
  bar->AddTool
    ( wxID_PASTE, _("&Paste"), wxArtProvider::GetBitmap(wxART_PASTE),
      _("Paste") );
  bar->AddSeparator();
  bar->AddTool
    ( wxID_UNDO, _("&Undo"), wxArtProvider::GetBitmap(wxART_UNDO), _("Undo") );
  bar->AddTool
    ( wxID_REDO, _("&Redo"), wxArtProvider::GetBitmap(wxART_REDO), _("Redo") );
  bar->AddSeparator();

  bar->AddCheckTool
    ( ID_WIREFRAME, _("&Wireframe"), wxBitmap(wireframe_xpm),
      wxBitmap(wireframe_xpm), _("Wireframe") );
  bar->AddCheckTool
    ( ID_GRAPHISM, _("&Graphism"), wxBitmap(graphism_xpm),
      wxBitmap(graphism_xpm), _("Graphism") );
  bar->AddCheckTool
    ( ID_TEXT, _("&Text"), wxBitmap(text_xpm), wxBitmap(text_xpm),
      _("Text") );
  bar->AddSeparator();

  m_zoom_spin = new wxSpinCtrl( bar, ID_ZOOM );
  m_zoom_spin->SetRange(10, std::numeric_limits<int>::max());
  m_zoom_spin->SetValue(100);

  bar->AddTool
    ( wxID_ZOOM_IN, _("Zoom &in"), wxBitmap(zoom_in_xpm), _("Zoom in") );
  bar->AddTool
    ( wxID_ZOOM_OUT, _("Zoom &out"), wxBitmap(zoom_out_xpm), _("Zoom out") );
  bar->AddTool
    ( wxID_ZOOM_100, _("No zoom"), wxBitmap(zoom_original_xpm), _("No zoom") );
  bar->AddTool
    ( wxID_ZOOM_FIT, _("Zoom &fit"),  wxBitmap(zoom_fit_xpm), _("Zoom fit") );
  bar->AddControl(m_zoom_spin);

  bar->Realize();
} // model_frame::create_toolbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the accelerators for the actions in the menus.
 */
void bf::model_frame::create_accelerators()
{
  wxAcceleratorEntry entries[] =
    {
      wxAcceleratorEntry( wxACCEL_CTRL, (int)'C', wxID_COPY ),
      wxAcceleratorEntry( wxACCEL_CTRL, (int)'S', wxID_SAVE ),
      wxAcceleratorEntry( wxACCEL_CTRL | wxACCEL_SHIFT, (int)'S', wxID_SAVEAS ),
      wxAcceleratorEntry( wxACCEL_CTRL, (int)'V', wxID_PASTE ),
      wxAcceleratorEntry( wxACCEL_CTRL, (int)'X', wxID_CUT ),
      wxAcceleratorEntry( wxACCEL_CTRL, (int)'Y', wxID_REDO ),
      wxAcceleratorEntry( wxACCEL_NORMAL, WXK_DELETE, wxID_DELETE ),
      wxAcceleratorEntry( wxACCEL_CTRL, WXK_INSERT, ID_NEW_SNAPSHOT ),
      wxAcceleratorEntry( wxACCEL_CTRL, WXK_DELETE, ID_DELETE_SNAPSHOT ),
    };

  wxAcceleratorTable accel
    ( sizeof(entries) / sizeof(wxAcceleratorEntry), entries );
  SetAcceleratorTable(accel);
} // model_frame::create_accelerators()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the wireframe mode.
 */
void bf::model_frame::toggle_wireframe_mode()
{
  m_model_view->toggle_wireframe_mode();
} // model_frame::toggle_wireframe_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the graphic mode.
 */
void bf::model_frame::toggle_graphic_mode()
{
  m_model_view->toggle_graphic_mode();
} // model_frame::toggle_graphic_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the text mode.
 */
void bf::model_frame::toggle_text_mode()
{
  m_model_view->toggle_text_mode();
} // model_frame::toggle_text_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the validity of current model.
 * \return true if the model is ok.
 */
bool bf::model_frame::check_model()
{
  bool result = true;
  std::list< const action* > actions;

  model::const_action_iterator it;

  for ( it = get_model().action_begin();
        it != get_model().action_end(); ++it )
    if ( !it->get_auto_next().empty() )
      if ( !get_model().has_action( it->get_auto_next() )  )
        actions.push_back(&(*it));

  if ( !actions.empty() )
    {
      error_check_model_dialog dlg(*this, actions);

      dlg.ShowModal();

      unsigned int index = dlg.get_index_selected();
      unsigned int i = 0;
      std::list< const action* >::const_iterator it;

      for ( it = actions.begin();
            (i != index) && (it != actions.end()); ++i, ++it)
        ;

      set_active_action((*it)->get_name());

      result = false;
    }

  return result;
} // model_frame::check_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the validity of current model.
 * \return true if the model is ok.
 */
void bf::model_frame::check_model_verbose()
{
  if ( check_model() )
    GetStatusBar()->SetStatusText( _("Check completed."), 0 );
} // model_frame::check_model_verbose()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the model.
 */
void bf::model_frame::compile_model()
{
  Disable();
  Refresh();

  if ( check_model() )
    {
      bool do_it = true;

      if ( m_model_file.IsEmpty() )
        do_it = save_as();

      if ( do_it )
        compile_model_no_check();
    }

  Enable();
  GetStatusBar()->SetStatusText( _("Compilation done."), 0 );
} // model_frame::compile_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the model without checking anything.
 */
void bf::model_frame::compile_model_no_check()
{
  model_file_compiler c;

  if ( !c.compile( get_model(), wx_to_std_string(m_model_file), m_workspace ) )
    {
      wxMessageDialog dlg
        ( this, _("Error"), _("Can't open the model file."), wxOK );

      dlg.ShowModal();
    }
} // model_frame::compile_model_no_check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of the windows of the program.
 */
void bf::model_frame::update_layout()
{
  m_layout.set_current_model_frame(*this);
  fill();
  Refresh();
  SendSizeEvent();
} // model_frame::update_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the slider.
 */
void bf::model_frame::update_slider() const
{
  m_slider->clear_ticks();

  if ( m_history.get_model().has_active_action() )
    {
      m_slider->set_max_value
        (m_history.get_model().get_active_action().get_duration());

      action::const_snapshot_iterator it;
      for ( it = m_history.get_model().get_active_action().snapshot_begin();
            it != m_history.get_model().get_active_action().snapshot_end();
            ++it )
        m_slider->add_tick(it->get_date());
    }
} // model_frame::update_slider()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update buttons.
 */
void bf::model_frame::update_button() const
{
  m_new_button->Enable(can_create_snapshot());
  m_delete_button->Enable(can_delete_snapshot());
  m_snapshot_copy_button->Enable(can_copy_snapshot());
  m_snapshot_paste_button->Enable(can_paste_snapshot());
  m_next_change_button->Enable(true);
  m_previous_change_button->Enable(true);
} // model_frame::update_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update toolbar.
 */
void bf::model_frame::update_toolbar() const
{
  wxToolBar* bar = GetToolBar();

  bar->ToggleTool(ID_WIREFRAME, m_model_view->get_wireframe_drawing());
  bar->ToggleTool(ID_GRAPHISM, m_model_view->get_graphic_drawing());
  bar->ToggleTool(ID_TEXT, m_model_view->get_text_drawing());

  if( m_model_view->get_zoom() == 10 )
    bar->EnableTool(wxID_ZOOM_OUT, false);
  else
    bar->EnableTool(wxID_ZOOM_OUT, true);
  m_zoom_spin->SetValue(m_model_view->get_zoom());

  bar->EnableTool(wxID_UNDO, m_history.can_undo());
  bar->EnableTool(wxID_REDO, m_history.can_redo());
} // model_frame::update_toolbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the controls.
 */
void bf::model_frame::fill()
{
  update_button();
  update_slider();
  update_toolbar();
  m_model_view->reload_action();
  m_model_view->set_date(m_slider->get_value());
} // model_frame::fill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if we can delete a snapshot.
 */
bool bf::model_frame::can_delete_snapshot() const
{
  bool result = false;

  if ( m_history.get_model().has_active_action() )
    {
      double date = m_slider->get_value();

      result =
        m_history.get_model().get_active_action().has_snapshot_date(date) &&
        (m_slider->get_value() != 0);
    }

  return result;
} // model_frame::can_delete_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if we can create a snapshot.
 */
bool bf::model_frame::can_create_snapshot() const
{
  bool result = false;

  if ( m_history.get_model().has_active_action() )
    {
      double date = m_slider->get_value();

      result =
        !m_history.get_model().get_active_action().has_snapshot_date(date);
    }

  return result;
} // model_frame::can_create_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a snapshot to copy in the editor.
 */
bool bf::model_frame::can_copy_snapshot() const
{
  bool result = false;

  if ( m_history.get_model().has_active_action() )
    {
      double date = m_slider->get_value();

      result =
        m_history.get_model().get_active_action().has_snapshot_date(date);
    }

  return result;
} // model_frame::can_copy_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a snapshot can be pasted in the editor.
 */
bool bf::model_frame::can_paste_snapshot() const
{
  return m_history.get_model().has_active_action();
} // model_frame::can_paste_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an action that deletes the current snapshot.
 */
bf::model_action* bf::model_frame::create_delete_snapshot_action()
{
  model_action* result(NULL);

  if ( m_history.get_model().has_active_action() )
    {
      const double date = m_slider->get_value();
      action& a = m_history.get_model().get_active_action();

      if ( a.has_snapshot_date(date) )
        result = new action_remove_snapshot
          ( a.get_name(), a.get_snapshot_before_or_at_date(date) );

    }

  return result;
} // model_frame::create_delete_snapshot_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the current snapshot.
 */
void bf::model_frame::delete_snapshot()
{
  model_action* a( create_delete_snapshot_action() );

  if ( a != NULL )
    {
      const double date = m_slider->get_value();
      m_slider->remove_tick(date);

      do_action( a );
    }
} // model_frame::delete_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new the current snapshot.
 */
void bf::model_frame::create_snapshot()
{
  if (m_history.get_model().has_active_action())
    {
      const action& a = m_history.get_model().get_active_action();

      const snapshot* ref =
        a.get_snapshot_before_or_at_date(m_slider->get_value());

      /// \todo If there is a snapshot at date zero, it should be returned by
      ///       the previous call.
      if ( ref == NULL )
        ref = a.get_snapshot_before_or_at_date(0);

      snapshot* s = new snapshot( *ref );
      s->set_date(m_slider->get_value());

      do_action( new action_add_snapshot( a.get_name(), s) );
      fill();
    }
} // model_frame::create_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the stapshot at the current date.
 */
void bf::model_frame::copy_snapshot() const
{
  if ( !m_history.get_model().has_active_action() )
    return;

  const double date = m_slider->get_value();
  const action& a = m_history.get_model().get_active_action();

  if ( a.has_snapshot_date(date) )
    {
      const snapshot* s( a.get_snapshot_before_or_at_date(date) );
      std::ostringstream oss;
      xml::model_snapshot_node writer;

      writer.write( *s, oss );

      if ( wxTheClipboard->Open() )
        {
          wxTheClipboard->SetData
            ( new wxTextDataObject( std_to_wx_string(oss.str()) ) );
          wxTheClipboard->Close();
        }

      update_button();
    }
} // model_frame::copy_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Paste a snapshot at the current date.
 */
void bf::model_frame::paste_snapshot()
{
  if ( !m_history.get_model().has_active_action() )
    return;

  const action& a( m_history.get_model().get_active_action() );
  snapshot* new_snapshot( get_snapshot_from_clipboard() );

  if ( new_snapshot == NULL )
    return;

  const double date( m_slider->get_value() );
  new_snapshot->set_date( date );

  action_group* const result = new action_group();
  result->set_description( wxT("Paste a snapshot") );

  model_action* const delete_action = create_delete_snapshot_action();
  if ( delete_action != NULL )
    result->add_action( delete_action );

  result->add_action( new action_add_snapshot( a.get_name(), new_snapshot ) );

  do_action( result );
} // model_frame::paste_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a snapshot according to the content of the clipboard.
 */
bf::snapshot* bf::model_frame::get_snapshot_from_clipboard() const
{
  snapshot* result(NULL);

  if ( m_history.get_model().has_active_action()
       && wxTheClipboard->Open() && wxTheClipboard->IsSupported( wxDF_TEXT ) )
    {

      wxTextDataObject data;
      wxTheClipboard->GetData( data );

      wxStringInputStream iss( data.GetText() );
      wxXmlDocument xml( iss );

      if ( xml.IsOk() )
        {
          const action& a( m_history.get_model().get_active_action() );
          result = a.create_snapshot();

          try
            {
              xml::model_snapshot_node reader;
              reader.read( *result, xml.GetRoot() );
            }
          catch ( std::exception& e )
            {
              delete result;
              result = NULL;
            }
        }

      wxTheClipboard->Close();
    }

  return result;
} // model_frame::get_snapshot_from_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the action.
 */
void bf::model_frame::play()
{
  if ( !m_model_view->is_finished() )
    {
      m_play_button->SetBitmapLabel( wxBitmap(player_stop_xpm) );
      m_timer.Start( m_model_view->get_duration_until_next() * 1000 );
    }
} // model_frame::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Continue playing the action.
 */
void bf::model_frame::play_next()
{
  set_date( m_model_view->next() );
  m_layout.update_snapshot();

  if ( m_model_view->is_finished() )
    stop();
  else
    m_timer.Start( m_model_view->get_duration_until_next() * 1000 );
} // model_frame::play_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop playing the action.
 */
void bf::model_frame::stop()
{
  m_play_button->SetBitmapLabel( wxBitmap(player_play_xpm) );
  m_timer.Stop();
} // model_frame::stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of a the menu bar or the popup menu. Entries are
 *        enabled/disabled, checkmarks are set or not.
 * \param m The menu to update
 */
template<typename MenuType>
void bf::model_frame::update_menu( MenuType& m ) const
{
  m.Enable(wxID_SAVE, is_changed());

  if ( m_history.can_undo() )
    m.SetLabel
      ( wxID_UNDO,
        wxString::Format
        ( _("Undo \"%s\"\tCTRL+Z"),
          m_history.get_undo_description().c_str() ) );
  else
    m.SetLabel(wxID_UNDO, _("Nothing to undo\tCTRL+Z"));

  if ( m_history.can_redo() )
    m.SetLabel
      ( wxID_REDO,
        wxString::Format
        ( _("Redo \"%s\"\tCTRL+Y"),
          m_history.get_redo_description().c_str() ) );
  else
    m.SetLabel(wxID_REDO, _("Nothing to redo\tCTRL+Y"));

  m.Enable(wxID_UNDO, m_history.can_undo());
  m.Enable(wxID_REDO, m_history.can_redo());

  m.Enable(ID_NEW_SNAPSHOT, can_create_snapshot());
  m.Enable(ID_DELETE_SNAPSHOT, can_delete_snapshot());
} // model_frame::update_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the windows with the properties of this model.
 * \param event The paint event.
 */
void bf::model_frame::on_activate(wxActivateEvent& WXUNUSED(event))
{
  m_layout.set_current_model_frame(*this);
} // model_frame::on_activate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::model_frame::on_close(wxCloseEvent& event)
{
  bool ok = true;

  if ( m_changed )
    {
      wxMessageDialog dlg
        ( this,
          _("This model has changes that are not saved. Save now?"),
          _("Model is not saved."), wxYES_NO | wxCANCEL );

      int answer = dlg.ShowModal();

      if ( answer == wxID_YES )
        ok = save();
      else if ( answer == wxID_CANCEL )
        ok = false;
    }

  if ( ok || !event.CanVeto() )
    {
      m_layout.remove_model_frame(*this);
      Destroy();
    }
  else
    event.Veto();
} // layer_list_frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user wants the context menu.
 * \param event The event.
 */
void bf::model_frame::on_context_menu(wxContextMenuEvent& event)
{
  update_menu( m_popup_menu );
  update_menu( *GetMenuBar() );

  if ( event.GetPosition() == wxDefaultPosition )
    PopupMenu( &m_popup_menu, event.GetPosition() );
  else // event.GetPosition() is the mouse position on the screen
    PopupMenu( &m_popup_menu, ScreenToClient(event.GetPosition()) );
} // model_frame::on_context_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Save".
 * \param event This event occured.
 */
void bf::model_frame::on_save(wxCommandEvent& WXUNUSED(event))
{
  save();
} // model_frame::on_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Save as".
 * \param event This event occured.
 */
void bf::model_frame::on_save_as(wxCommandEvent& WXUNUSED(event))
{
  save_as();
} // model_frame::on_save_as()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Undo".
 * \param event This event occured.
 */
void bf::model_frame::on_undo(wxCommandEvent& WXUNUSED(event))
{
  undo();
} // model_frame::on_undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Redo".
 * \param event This event occured.
 */
void bf::model_frame::on_redo(wxCommandEvent& WXUNUSED(event))
{
  redo();
} // model_frame::on_redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Cut".
 * \param event This event occured.
 */
void bf::model_frame::on_cut(wxCommandEvent& WXUNUSED(event))
{

} // model_frame::on_cut()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Copy".
 * \param event This event occured.
 */
void bf::model_frame::on_copy(wxCommandEvent& WXUNUSED(event))
{

} // model_frame::on_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Paste".
 * \param event This event occured.
 */
void bf::model_frame::on_paste(wxCommandEvent& WXUNUSED(event))
{

} // model_frame::on_paste()


/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "next change".
 * \param event This event occured.
 */
void bf::model_frame::on_next_change(wxCommandEvent& WXUNUSED(event))
{
  if (m_history.get_model().has_active_action())
    {
      m_slider->set_value
        (m_model_view->get_duration_until_next() +  m_slider->get_value());

      fill();
    }
} // model_frame::on_next_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "previous change".
 * \param event This event occured.
 */
void bf::model_frame::on_previous_change(wxCommandEvent& WXUNUSED(event))
{
  // TODO
  // move the current tick
} // model_frame::on_previous_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Delete".
 * \param event This event occured.
 */
void bf::model_frame::on_delete(wxCommandEvent& WXUNUSED(event))
{

} // model_frame::on_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Check model".
 * \param event This event occured.
 */
void bf::model_frame::on_check_model(wxCommandEvent& WXUNUSED(event))
{
  check_model_verbose();
} // model_frame::on_check_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Compile model".
 * \param event This event occured.
 */
void bf::model_frame::on_compile_model(wxCommandEvent& WXUNUSED(event))
{
  compile_model();
} // model_frame::on_compile_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Close".
 * \param event This event occured.
 */
void bf::model_frame::on_menu_close(wxCommandEvent& WXUNUSED(event))
{
  Close();
} // model_frame::on_menu_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief A menu entry is highlighted.
 * \param event This event occured.
 */
void bf::model_frame::on_menu_highlight(wxMenuEvent& event)
{
  GetStatusBar()->SetStatusText
    ( m_popup_menu.GetHelpString(event.GetMenuId()), 0 );
} // model_frame::on_menu_highlight()

/*----------------------------------------------------------------------------*/
/**
 * \brief A menu is opened.
 * \param event The event.
 */
void bf::model_frame::on_open_menu(wxMenuEvent& WXUNUSED(event))
{
  update_menu( m_popup_menu );
  update_menu( *GetMenuBar() );
} // model_frame::on_menu_open()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Wireframe".
 * \param event This event occured.
 */
void bf::model_frame::on_wireframe(wxCommandEvent& WXUNUSED(event))
{
  toggle_wireframe_mode();
  GetToolBar()->ToggleTool(ID_WIREFRAME, m_model_view->get_wireframe_drawing());
} // model_frame::on_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Graphic".
 * \param event This event occured.
 */
void bf::model_frame::on_graphic(wxCommandEvent& WXUNUSED(event))
{
  toggle_graphic_mode();
  GetToolBar()->ToggleTool(ID_GRAPHISM, m_model_view->get_graphic_drawing());
} // model_frame::on_graphic()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Text".
 * \param event This event occured.
 */
void bf::model_frame::on_text(wxCommandEvent& WXUNUSED(event))
{
  toggle_text_mode();
  GetToolBar()->ToggleTool(ID_TEXT, m_model_view->get_text_drawing());
} // model_frame::on_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_100".
 * \param event This event occured.
 */
void bf::model_frame::on_zoom_100(wxCommandEvent& WXUNUSED(event))
{
  m_model_view->set_zoom(100);
  m_model_view->Refresh();
  SendSizeEvent();
  update_toolbar();
} // model_frame::on_zoom_100()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_in".
 * \param event This event occured.
 */
void bf::model_frame::on_zoom_in(wxCommandEvent& WXUNUSED(event))
{
  m_model_view->set_zoom( m_model_view->get_zoom() + 10);
  m_model_view->Refresh();
  SendSizeEvent();
  update_toolbar();
} // model_frame::on_zoom_in()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_out".
 * \param event This event occured.
 */
void bf::model_frame::on_zoom_out(wxCommandEvent& WXUNUSED(event))
{
  if ( m_model_view->get_zoom() > 10)
    {
      if ( m_model_view->get_zoom() < 20 )
        m_model_view->set_zoom( 10);
      else
        m_model_view->set_zoom( m_model_view->get_zoom() - 10);

      m_model_view->Refresh();
      SendSizeEvent();
      update_toolbar();
    }
} // model_frame::on_zoom_out()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_fit".
 * \param event This event occured.
 */
void bf::model_frame::on_zoom_fit(wxCommandEvent& WXUNUSED(event))
{
  m_model_view->auto_zoom();
  m_model_view->Refresh();
  SendSizeEvent();
  update_toolbar();
} // model_frame::on_zoom_fit()

/*----------------------------------------------------------------------------*/
/**
 * \brief The zoom control has been changed.
 * \param event The event that occured.
 */
void bf::model_frame::on_zoom_change( wxSpinEvent& event )
{
  m_model_view->set_zoom( event.GetPosition() );
  m_model_view->Refresh();
  SendSizeEvent();
} // model_frame::on_zoom_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The middle button of mouse turn.
 * \param event The mouse event that occured.
 */
void bf::model_frame::on_mouse_wheel_rotation(wxMouseEvent& event)
{
  int rotation = event.GetWheelRotation();

  if ( event.ControlDown() )
    {
      unsigned int zoom = m_model_view->get_zoom();

      if ( rotation >= 0 )
        zoom += 15;
      else if ( zoom > 15 )
        zoom -= 15;

      m_model_view->set_zoom(zoom);
      m_model_view->Refresh();
      SendSizeEvent();
      update_toolbar();
    }
} // model_frame::on_mouse_wheel_rotation

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on a new snapshot.
 * \param event This event occured.
 */
void bf::model_frame::on_new_snapshot(wxCommandEvent& WXUNUSED(event))
{
  if ( can_create_snapshot() )
    create_snapshot();
} // model_frame::on_new_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on delete snapshot.
 * \param event This event occured.
 */
void bf::model_frame::on_delete_snapshot(wxCommandEvent& WXUNUSED(event))
{
  if ( can_delete_snapshot() )
    delete_snapshot();
} // model_frame::on_delete_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the current snapshot in the clipboard in response to an event.
 * \param event This event occured.
 */
void bf::model_frame::on_copy_snapshot(wxCommandEvent& WXUNUSED(event))
{
  if ( can_copy_snapshot() )
    copy_snapshot();
} // model_frame::on_copy_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Paste the current snapshot in the clipboard in response to an event.
 * \param event This event occured.
 */
void bf::model_frame::on_paste_snapshot(wxCommandEvent& WXUNUSED(event))
{
  if ( can_paste_snapshot() )
    paste_snapshot();
} // model_frame::on_paste_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief The date of the slider changed.
 * \param event This event occured.
 */
void bf::model_frame::on_slider_change(slider_event& WXUNUSED(event))
{
  fill();
  get_model().set_date(m_slider->get_value());
  m_layout.update_snapshot();
} // model_frame::on_slider_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief A tick move.
 * \param event This event occured.
 */
void bf::model_frame::on_tick_move(tick_event& event)
{
  double initial_date = event.get_initial_value();
  double new_date = event.get_new_value();
  bool ok = true;

  if ( new_date != initial_date )
    {
      action& act = get_model().get_active_action();

      if ( act.has_snapshot_date( new_date ) )
        {
          wxMessageDialog dlg
            ( this, _("This date is already used."
                      " The snapshot at this date is going to be removed."),
              _("Snapshot at this date"), wxCANCEL | wxOK );
          ok = (dlg.ShowModal() == wxID_OK );
        }

      if ( ok )
        {
          action_group* group = new action_group();

          if ( event.get_copy() )
            group->set_description(_("Add snapshot"));
          else
            group->set_description(_("Move snapshot"));

          if ( act.has_snapshot_date( new_date ) )
            group->add_action
              ( new action_remove_snapshot
                ( act.get_name(),
                  act.get_snapshot_before_or_at_date(new_date) ) );

          if ( event.get_copy() )
            {
              snapshot* s =
                new snapshot(*act.get_snapshot_before_or_at_date(initial_date));
              s->set_date(new_date);
              group->add_action( new action_add_snapshot(act.get_name(), s));
            }
          else
            group->add_action
              ( new action_move_snapshot
                ( act.get_snapshot_before_or_at_date(initial_date),
                  new_date ) );

          if ( !event.get_copy() && ( initial_date == 0 ) )
            {
              snapshot* s =
                new snapshot
                ( *act.get_snapshot_before_or_at_date(initial_date) );
              s->set_date(0);
              group->add_action( new action_add_snapshot(act.get_name(), s));
            }

          do_action(group);
          set_date(new_date);
          m_layout.update_snapshot();
        }
      else
        event.Veto();
    }
} // model_frame::on_tick_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "play" or "stop" button.
 * \param event The event that occured.
 */
void bf::model_frame::on_play_stop( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_timer.IsRunning() )
    stop();
  else
    play();
} // model_frame::on_play_stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the "start" button.
 * \param event The event that occured.
 */
void bf::model_frame::on_start( wxCommandEvent& WXUNUSED(event) )
{
  m_slider->set_value(0);
} // model_frame::on_start()

/*----------------------------------------------------------------------------*/
/**
 * \brief The timer has expired.
 * \param event The event that occured.
 */
void bf::model_frame::on_timer( wxTimerEvent& WXUNUSED(event) )
{
  play_next();
} // model_frame::on_timer()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark is modified.
 * \param event This event occured.
 */
void bf::model_frame::on_mark_change(mark_event& event)
{
  set_mark_selection(event.get_value());
} // model_frame::on_mark_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark moves.
 * \param event This event occured.
 */
void bf::model_frame::on_mark_move(placement_event& event)
{
  set_mark_selection(event.get_label());

  mark* m( get_model().get_main_mark_selection() );

  action_group* r = new action_group();

  r->add_action
    ( make_action_modify_placement_x
      ( get_model().get_selected_snapshot(), m, event.get_x() ) );
  r->add_action
    ( make_action_modify_placement_y
      ( get_model().get_selected_snapshot(), m, event.get_y() ) );

  do_action( r );
} // model_frame::on_mark_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief The selected mark is resized.
 * \param event This event occured.
 */
void bf::model_frame::on_mark_size(placement_event& event)
{
  set_mark_selection(event.get_label());

  mark* m( get_model().get_main_mark_selection() );

  action_group* a = new action_group();
  a->set_description( _("Resize mark") );

  a->add_action
    ( make_action_modify_placement_x
      ( get_model().get_selected_snapshot(), m, event.get_x() ) );
  a->add_action
    ( make_action_modify_placement_y
      ( get_model().get_selected_snapshot(), m, event.get_y() ) );

  a->add_action
    ( make_action_modify_placement_width
      ( get_model().get_selected_snapshot(), m, event.get_width() ) );
  a->add_action
    ( make_action_modify_placement_height
      ( get_model().get_selected_snapshot(), m, event.get_height() ) );

  do_action(a);
} // model_frame::on_mark_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief The bounding_box change.
 * \param event This event occured.
 */
void bf::model_frame::on_bounding_box_change(bounding_box_event& event)
{
  snapshot* s = get_model().get_selected_snapshot();

  if ( s )
    {
      action_group* a = new action_group();
      a->set_description(_("Modify the bounding box"));

      snapshot::const_mark_placement_iterator it;

      if ( ( event.get_left() != 0 ) || ( event.get_bottom() != 0) )
        for ( it = s->mark_placement_begin();
              it != s->mark_placement_end(); ++it )
          {
            const std::string label(it->get_mark()->get_label());

            mark_placement& p = s->get_placement(label);
            mark* m = get_model().get_active_action().get_mark(label);

            double x = p.get_x_position();
            double y = p.get_y_position();

            x -= event.get_left();
            y -= event.get_bottom();

            a->add_action( make_action_modify_placement_x( s, m, x ) );
            a->add_action( make_action_modify_placement_y( s, m, y ) );
          }

      a->add_action( new action_modify_snapshot_size
                    (s, event.get_width(), event.get_height() ));

      do_action( a );
    }
} // model_frame::on_bounding_box_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief The bounding_box has been selected.
 * \param event This event occured.
 */
void bf::model_frame::on_bounding_box_selected
( bounding_box_event& WXUNUSED(event) )
{
  set_bounding_box_selection();
} // model_frame::on_bounding_box_selected()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::model_frame, wxFrame)
  EVT_ACTIVATE( bf::model_frame::on_activate )
  EVT_CLOSE( bf::model_frame::on_close )
  EVT_CONTEXT_MENU( bf::model_frame::on_context_menu )

  EVT_MENU( wxID_SAVE, bf::model_frame::on_save )
  EVT_MENU( wxID_SAVEAS, bf::model_frame::on_save_as )

  EVT_MENU( wxID_UNDO, bf::model_frame::on_undo )
  EVT_MENU( wxID_REDO, bf::model_frame::on_redo )
  EVT_MENU( wxID_CUT, bf::model_frame::on_cut )
  EVT_MENU( wxID_COPY, bf::model_frame::on_copy )
  EVT_MENU( wxID_PASTE, bf::model_frame::on_paste )
  EVT_MENU( wxID_DELETE, bf::model_frame::on_delete )
  EVT_MENU(  bf::model_frame::ID_CHECK_MODEL,
             bf::model_frame::on_check_model )
  EVT_MENU(  bf::model_frame::ID_COMPILE_MODEL,
             bf::model_frame::on_compile_model )
  EVT_MENU( wxID_CLOSE, bf::model_frame::on_menu_close )
  EVT_MENU_HIGHLIGHT_ALL( bf::model_frame::on_menu_highlight )
  EVT_MENU_OPEN( bf::model_frame::on_open_menu )

  EVT_MENU( bf::model_frame::ID_WIREFRAME, bf::model_frame::on_wireframe )
  EVT_MENU( bf::model_frame::ID_GRAPHISM, bf::model_frame::on_graphic )
  EVT_MENU( bf::model_frame::ID_TEXT, bf::model_frame::on_text )

  EVT_MENU( wxID_ZOOM_100,
            bf::model_frame::on_zoom_100)
  EVT_MENU( wxID_ZOOM_IN,
            bf::model_frame::on_zoom_in)
  EVT_MENU( wxID_ZOOM_OUT,
            bf::model_frame::on_zoom_out)
  EVT_MENU( wxID_ZOOM_FIT,
            bf::model_frame::on_zoom_fit)
  EVT_MENU( bf::model_frame::ID_NEW_SNAPSHOT,
            bf::model_frame::on_new_snapshot)
  EVT_MENU( bf::model_frame::ID_DELETE_SNAPSHOT,
            bf::model_frame::on_delete_snapshot)

  EVT_BUTTON( bf::model_frame::ID_NEW_SNAPSHOT,
              bf::model_frame::on_new_snapshot )
  EVT_BUTTON( bf::model_frame::ID_DELETE_SNAPSHOT,
              bf::model_frame::on_delete_snapshot )
  EVT_BUTTON( bf::model_frame::ID_COPY_SNAPSHOT,
              bf::model_frame::on_copy_snapshot )
  EVT_BUTTON( bf::model_frame::ID_PASTE_SNAPSHOT,
              bf::model_frame::on_paste_snapshot )
  EVT_BUTTON( bf::model_frame::ID_NEXT_CHANGE,
              bf::model_frame::on_next_change )
  EVT_BUTTON( bf::model_frame::ID_PREVIOUS_CHANGE,
              bf::model_frame::on_previous_change )

  EVT_SLIDER_VALUE_CHANGE( bf::model_frame::ID_SLIDER,
                           bf::model_frame::on_slider_change )
  EVT_TICK_MOVE( bf::model_frame::ID_SLIDER,
                 bf::model_frame::on_tick_move )
  EVT_SPINCTRL( ID_ZOOM,
                bf::model_frame::on_zoom_change )

  EVT_BUTTON( bf::model_frame::ID_PLAY_STOP, bf::model_frame::on_play_stop )
  EVT_BUTTON( bf::model_frame::ID_START, bf::model_frame::on_start )

  EVT_TIMER( bf::model_frame::ID_TIMER, bf::model_frame::on_timer )
  EVT_MARK_SELECTION_CHANGE( bf::model_frame::ID_MODEL_VIEW,
                             bf::model_frame::on_mark_change )
  EVT_PLACEMENT_MOVE( bf::model_frame::ID_MODEL_VIEW,
                      bf::model_frame::on_mark_move )
  EVT_PLACEMENT_SIZE( bf::model_frame::ID_MODEL_VIEW,
                      bf::model_frame::on_mark_size )
  EVT_BOUNDING_BOX_CHANGE( bf::model_frame::ID_MODEL_VIEW,
                           bf::model_frame::on_bounding_box_change )
  EVT_BOUNDING_BOX_SELECTED( bf::model_frame::ID_MODEL_VIEW,
                             bf::model_frame::on_bounding_box_selected )
  EVT_MOUSEWHEEL( bf::model_frame::on_mouse_wheel_rotation )

END_EVENT_TABLE()
