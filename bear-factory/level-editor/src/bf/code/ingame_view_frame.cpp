/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::ingame_view_frame class.
 * \author Julien Jorge
 */
#include "bf/ingame_view_frame.hpp"

#include "bf/clone_selection_dialog.hpp"
#include "bf/error_check_level_dialog.hpp"
#include "bf/grid_properties_frame.hpp"
#include "bf/gui_level.hpp"
#include "bf/image_pool.hpp"
#include "bf/ingame_view.hpp"
#include "bf/layer_list_frame.hpp"
#include "bf/level_file_xml_reader.hpp"
#include "bf/level_overview_frame.hpp"
#include "bf/level_properties_frame.hpp"
#include "bf/level_runner.hpp"
#include "bf/main_frame.hpp"
#include "bf/new_layer_from_image_dialog.hpp"
#include "bf/offset_selection_dialog.hpp"
#include "bf/properties_frame.hpp"
#include "bf/windows_layout.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/wx_type_cast.hpp"

#include "bf/history/action_add_layer.hpp"
#include "bf/history/action_align_selection_bottom.hpp"
#include "bf/history/action_align_selection_center_x.hpp"
#include "bf/history/action_align_selection_center_y.hpp"
#include "bf/history/action_align_selection_left.hpp"
#include "bf/history/action_align_selection_right.hpp"
#include "bf/history/action_align_selection_top.hpp"
#include "bf/history/action_arrange_selection_horizontally.hpp"
#include "bf/history/action_arrange_selection_vertically.hpp"
#include "bf/history/action_clone_selection.hpp"
#include "bf/history/action_delete_selection.hpp"
#include "bf/history/action_group.hpp"
#include "bf/history/action_move_down.hpp"
#include "bf/history/action_move_selection.hpp"
#include "bf/history/action_move_up.hpp"
#include "bf/history/action_move_in_other_layer.hpp"
#include "bf/history/action_remove_layer.hpp"
#include "bf/history/action_resize_level.hpp"
#include "bf/history/action_set_item_field.hpp"

#include "bf/icon/compile.xpm"
#include "bf/icon/quick_compile.xpm"
#include "bf/icon/run.xpm"

#include "bf/icon/adjust_horizontally.xpm"
#include "bf/icon/adjust_vertically.xpm"
#include "bf/icon/align_bottom.xpm"
#include "bf/icon/align_horizontal_middle.xpm"
#include "bf/icon/align_left.xpm"
#include "bf/icon/align_right.xpm"
#include "bf/icon/align_top.xpm"
#include "bf/icon/align_vertical_middle.xpm"
#include "bf/icon/bright.xpm"
#include "bf/icon/clone.xpm"
#include "bf/icon/edit_active_layer.xpm"
#include "bf/icon/edit_all_layers.xpm"
#include "bf/icon/edit_same_tag.xpm"
#include "bf/icon/graphism.xpm"
#include "bf/icon/grid.xpm"
#include "bf/icon/magnetism.xpm"
#include "bf/icon/move_backward.xpm"
#include "bf/icon/move_forward.xpm"
#include "bf/icon/offset.xpm"
#include "bf/icon/overview.xpm"
#include "bf/icon/relationship.xpm"
#include "bf/icon/text.xpm"
#include "bf/icon/wireframe.xpm"
#include "bf/icon/zoom_in.xpm"
#include "bf/icon/zoom_out.xpm"
#include "bf/icon/zoom_original.xpm"

#include "bf/item_filter/item_filter_by_class_name.hpp"

#include <fstream>
#include <wx/filename.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/utils.h> 

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param layout The windows of the program.
 * \param lvl The level.
 * \param level_file The path of the file from which \a lvl was read.
 * The level will be deleted in the destructor.
 */
bf::ingame_view_frame::ingame_view_frame
( windows_layout& layout, gui_level* lvl, const wxString& level_file )
  : wxFrame( &layout.get_main_frame(), wxID_ANY, _("New level") ),
    m_accelerator(*this), m_layout(layout), m_level_file(level_file),
    m_layer_info(_("no layer")), m_changed(false), m_compile_changed(false),
    m_edit_mode_menu(NULL), m_align_menu(NULL), m_arrange_menu(NULL)
{
  create_controls(layout, lvl);
  m_layout.add_level_view(*this);

  m_overview = new level_overview_frame(*this);
  m_errors = new error_check_level_dialog(this, ID_ERROR_FRAME);

  set_changed(false);
  set_compile_changed(false);
  set_layer_info();
  update_toolbar();
} // ingame_view_frame::ingame_view_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path of the file in which the level is saved.
 */
const wxString& bf::ingame_view_frame::get_level_file() const
{
  return m_level_file;
} // ingame_view_frame::get_level_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the value/range of the scrollbars.
 */
void bf::ingame_view_frame::adjust_scrollbars()
{
  if ( !m_ingame_view->empty() )
    {
#if 0
      m_h_scrollbar->SetScrollbar
        ( m_ingame_view->get_view_position().x,
          m_ingame_view->get_view_size().x * 100 / m_ingame_view->get_zoom(),
          m_ingame_view->get_layer_view_size().x,
          m_ingame_view->get_view_size().x * 100 /
          ( 2 * m_ingame_view->get_zoom()), true );
      m_v_scrollbar->SetScrollbar
        ( m_ingame_view->get_layer_view_size().y -
          m_ingame_view->get_view_position().y - 100 *
          m_ingame_view->get_view_size().y / m_ingame_view->get_zoom(),
          m_ingame_view->get_view_size().y*100/m_ingame_view->get_zoom(),
          m_ingame_view->get_layer_view_size().y,
          (100 * m_ingame_view->get_view_size().y) /
          ( 2 * m_ingame_view->get_zoom()), true );
#else
     m_h_scrollbar->SetScrollbar
        ( m_ingame_view->get_view_position().x,
          m_ingame_view->get_view_size().x * 100 / m_ingame_view->get_zoom(),
          m_ingame_view->get_level().get_width(),
          m_ingame_view->get_view_size().x * 100 /
          ( 2 * m_ingame_view->get_zoom()), true );
      m_v_scrollbar->SetScrollbar
        ( m_ingame_view->get_level().get_height() -
          m_ingame_view->get_view_position().y - 100 *
          m_ingame_view->get_view_size().y / m_ingame_view->get_zoom(),
          m_ingame_view->get_view_size().y*100/m_ingame_view->get_zoom(),
          m_ingame_view->get_level().get_height(),
          (100 * m_ingame_view->get_view_size().y) /
          ( 2 * m_ingame_view->get_zoom()), true );
#endif
    }
} // ingame_view_frame::adjust_scrollbars()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level is changed or not.
 * \param b Tell if the level is changed or not.
 */
void bf::ingame_view_frame::set_changed( bool b )
{
  m_changed = b;
  make_title();
  update_toolbar();
} // ingame_view_frame::set_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level is compiled or not.
 * \param b Tell if the level is compile or not.
 */
void bf::ingame_view_frame::set_compile_changed( bool b )
{
  m_compile_changed = b;
  make_title();
  update_toolbar();
} // ingame_view_frame::set_compile_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level has changes that are not saved.
 */
bool bf::ingame_view_frame::is_changed() const
{
  return m_changed;
} // ingame_view_frame::is_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level has changes that are not compiled.
 */
bool bf::ingame_view_frame::is_compile_changed() const
{
  return m_compile_changed;
} // ingame_view_frame::is_compile_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform of the name and the index of the active layer.
 */
void bf::ingame_view_frame::set_layer_info()
{
  if (m_ingame_view->empty() )
    m_layer_info = _("no layer");
  else
    {
      std::ostringstream oss;

      oss << m_ingame_view->get_active_index() << "-"
          << m_ingame_view->get_active_layer().get_displayed_name();

      m_layer_info = std_to_wx_string(oss.str());
    }

  make_title();
  update_layer_menu(m_layer_popup_menu, m_layer_popup_index);
  update_layer_menu(m_layer_menu, m_layer_index);
} // ingame_view_frame::set_layer_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the level.
 */
bool bf::ingame_view_frame::save()
{
  bool result = false;

  if ( m_level_file.empty() )
    result = save_as();
  else
    result = effective_save();

  return result;
} // ingame_view_frame::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the level and ask the file to the user.
 */
bool bf::ingame_view_frame::save_as()
{
  bool result = false;
  wxFileDialog dlg( this, _("Choose a file"), wxEmptyString, m_level_file,
                    _("Level files (*.lvl)|*.lvl"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_level_file = dlg.GetPath();
      result = effective_save();
    }

  return result;
} // ingame_view_frame::save_as()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item to the current layer.
 * \param class_name The name of the class to instanciate.
 * \return true if the item was added.
 */
bool bf::ingame_view_frame::add_item( const std::string& class_name )
{
  return m_ingame_view->add_item( class_name );
} // ingame_view_frame::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ingame view.
 */
bf::ingame_view* bf::ingame_view_frame::get_ingame_view()
{
  return m_ingame_view;
} // ingame_view_frame::get_ingame_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the ingame view.
 */
const bf::ingame_view* bf::ingame_view_frame::get_ingame_view() const
{
  return m_ingame_view;
} // ingame_view_frame::get_ingame_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the view in the level.
 */
wxPoint bf::ingame_view_frame::get_view_position() const
{
  return m_ingame_view->get_view_position();
} // ingame_view_frame::get_view_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the horizontal scrollbar.
 */
const wxScrollBar& bf::ingame_view_frame::get_h_scrollbar()
{
  return *m_h_scrollbar;
} // ingame_view_frame::get_h_scrollbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the vertical scrollbar.
 */
const wxScrollBar& bf::ingame_view_frame::get_v_scrollbar()
{
  return *m_v_scrollbar;
} // ingame_view_frame::get_v_scrollbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the zoom.
 */
void bf::ingame_view_frame::update_zoom() const
{
  m_zoom_spin->SetValue(m_ingame_view->get_zoom());
} // ingame_view_frame::update_zoom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make the title of the window.
 */
void bf::ingame_view_frame::make_title()
{
  if ( m_level_file.empty() )
    SetTitle( _("New level") );
  else
    SetTitle( wxFileName(m_level_file).GetName() );

  if ( m_changed )
    SetTitle( GetTitle() + wxT("*") );

  if ( m_compile_changed )
    SetTitle( GetTitle() + wxT(" .cl*") );

  SetTitle( GetTitle() + wxT(" (") + m_layer_info + wxT(")") );
} // ingame_view_frame::make_title()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the level without asking anything.
 */
bool bf::ingame_view_frame::effective_save()
{
  bool result = false;

  std::string std_path( wx_to_std_string(m_level_file) );
  std::ofstream f( std_path.c_str() );

  if (f)
    {
      m_ingame_view->save(f);
      result = true;
      set_changed(false);
    }
  else
    {
      wxMessageDialog dlg
        ( this, _("Error"), _("Can't open the level file."), wxOK );

      dlg.ShowModal();
    }

  return result;
} // ingame_view_frame::effective_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of the window.
 * \param layout The windows of the program.
 * \param lvl The level.
 */
void
bf::ingame_view_frame::create_controls( windows_layout& layout, gui_level* lvl )
{
  create_member_controls(layout, lvl);
  create_sizer_controls();
  create_accelerators();
  create_menu();
  create_toolbar();

  const int bar_sizes[5] = { -2, 80, 80, -1, 160 };

  wxStatusBar* bar = new wxStatusBar(this, wxID_ANY);
  bar->SetFieldsCount(5, bar_sizes);
  SetStatusBar(bar);
} // ingame_view_frame::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls for which we keep a reference.
 * \param layout The windows of the program.
 * \param lvl The level.
 */
void bf::ingame_view_frame::create_member_controls
( windows_layout& layout, gui_level* lvl )
{
  m_ingame_view = new ingame_view(*this, lvl, layout);
  m_h_scrollbar = new wxScrollBar(this, wxID_ANY);
  m_v_scrollbar = new wxScrollBar
    ( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
} // ingame_view_frame::create_member_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls and add them in sizers.
 */
void bf::ingame_view_frame::create_sizer_controls()
{
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(m_v_scrollbar, 1, wxEXPAND);

  wxBoxSizer* s_sizer = new wxBoxSizer(wxHORIZONTAL);
  s_sizer->Add(m_ingame_view, 1, wxEXPAND);
  s_sizer->Add(sizer, 0, wxEXPAND);

  sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(s_sizer, 1, wxEXPAND);

  s_sizer = new wxBoxSizer(wxHORIZONTAL);
  s_sizer->Add(m_h_scrollbar, 1, wxALIGN_BOTTOM);
  s_sizer->Add
    ( new wxWindow
      ( this, wxID_ANY, wxDefaultPosition,
        wxSize( m_v_scrollbar->GetSize().x, m_h_scrollbar->GetSize().y ) ),
      0
      );

  sizer->Add(s_sizer, 0, wxEXPAND);

  SetSizer(sizer);
  SetAutoLayout(true);
} // ingame_view_frame::create_sizer_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the popup menu.
 */
void bf::ingame_view_frame::create_menu()
{
  wxMenuBar* bar = new wxMenuBar();
  m_edit_mode_menu = create_edit_mode_menu();
  m_align_menu = create_align_menu();
  m_moving_layer_menu = new wxMenu();
  m_moving_layer_popup_menu = new wxMenu(); 
  m_reference_item_field_popup_menu = new wxMenu();
  m_layer_menu = new wxMenu();
  m_layer_popup_menu = new wxMenu();

  m_popup_menu.Append
    ( wxID_ANY, _("&Level"), create_level_menu(), _("Options of the level.") );
  bar->Append( create_level_menu(), _("&Level") );

  m_popup_menu.Append
    ( wxID_ANY, _("&Edit"), create_edit_menu(m_moving_layer_popup_menu), 
      _("Edit options.") );
  bar->Append( create_edit_menu(m_moving_layer_menu), _("&Edit") );
  
  m_popup_menu.Append
    ( wxID_ANY, _("&View"), create_view_menu(), _("Options of the view.") );
  bar->Append( create_view_menu(), _("&View") );
  
  m_popup_menu.Append
    ( wxID_ANY, _("&Layer"), create_layer_menu(m_layer_popup_menu), 
      _("Layer selection.") );
  bar->Append( create_layer_menu(m_layer_menu), _("&Layer") );

  m_popup_menu.Append
    ( wxID_ANY, _("&Reference item fields"), 
      m_reference_item_field_popup_menu, _("Reference item fields.") );

  SetMenuBar(bar);
} // ingame_view_frame::create_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the level menu.
 */
wxMenu* bf::ingame_view_frame::create_level_menu() const
{
  wxMenu* result = new wxMenu();

  result->Append( wxID_SAVE, _("&Save"), _("Save the current level.") );
  result->Append
    ( wxID_SAVEAS, _("Save &as...\tCTRL+SHIFT+S"),
      _("Save the current level under a new name.") );
  result->AppendSeparator();
  result->Append
    ( ID_CHECK_LEVEL, _("Check the &validty of the level\tF8"),
      _("Check validity of the level."));
  wx_menu_append_item
    ( result, ID_COMPILE_LEVEL, _("C&ompile the level\tF9"),
      _("Compile the level."), wxBitmap(compile_xpm) );
  wx_menu_append_item
    ( result, ID_QUICK_COMPILE_LEVEL, _("Compile the level (&quick)"),
      _("Compile the level with no optimization."),
      wxBitmap(quick_compile_xpm) );
  wx_menu_append_item
    ( result, ID_RUN_LEVEL, _("&Run this level\tF11"), _("Run this level."),
      wxBitmap(run_xpm) );
  result->AppendSeparator();
  result->Append
    ( wxID_PROPERTIES, _("&Properties"), _("Properties of the level.") );
  result->Append
    ( ID_STATISTICS, _("S&tatistics"), _("Statistics about the level.") );
  result->AppendSeparator();
  result->Append( wxID_CLOSE, _("&Close"), _("Close the level.") );

  return result;
} // ingame_view_frame::create_level_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the edit menu.
 * \param moving_layer_menu The layer menu for selection moving.
 */
wxMenu* bf::ingame_view_frame::create_edit_menu(wxMenu* moving_layer_menu)
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
    ( wxID_SELECTALL, _("&Select all\tCTRL+A"),
      _("Select all items in the current layer.") );
  result->Append
    ( ID_SELECT_NONE, _("Select &none\tCTRL+SHIFT+A"),
      _("Clear the selection.") );

  result->AppendSeparator();

  result->Append
    ( ID_ALIGN, _("&Align"), create_align_menu(),
      _("Align the selected items.") );

  m_arrange_menu = new wxMenu();
  wx_menu_append_item
    ( m_arrange_menu, ID_ARRANGE_X, _("&Horizontally"),
      _("Make the horizontal space uniform between the items."),
      wxBitmap(adjust_horizontally_xpm) );
  wx_menu_append_item
    ( m_arrange_menu, ID_ARRANGE_Y, _("&Vertically"),
      _("Make the vertical space uniform between the items."),
      wxBitmap(adjust_vertically_xpm) );

  result->Append
    ( ID_ARRANGE, _("&Arrange"), m_arrange_menu,
      _("Arrange the selected items.") );

  wx_menu_append_item
    ( result, ID_CLONE, _("C&lone..."), _("Clone the selection."),
      wxBitmap(clone_xpm) );
  wx_menu_append_item
    ( result, ID_OFFSET, _("&Offset..."),
      _("Apply an offset to the selection."), wxBitmap(offset_xpm) );

  result->Append
    ( ID_MOVE_IN_LAYER, _("C&hange layer..."), moving_layer_menu,
      _("Move the selection in an other layer.") );

  result->Append
    ( ID_EDIT_MODE, _("Edit &mode..."), create_edit_mode_menu(),
      _("Tells to which objects are applied the operations.") );

#if 0
  result->AppendSeparator();
  result->Append
    ( ID_NEW_LAYER_FROM_IMAGE, _("New layer from image..."),
      _("Create a new layer from image.") );
#endif

  return result;
} // ingame_view_frame::create_edit_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the menu with the operations to align the selection.
 */
wxMenu* bf::ingame_view_frame::create_align_menu() const
{
  wxMenu* const result( new wxMenu() );

  wx_menu_append_item
    ( result, ID_ALIGN_LEFT, _("&Left"),
      _("Align the left edge of the items."), wxBitmap(align_left_xpm) );
  wx_menu_append_item
    ( result, ID_ALIGN_RIGHT, _("&Right"),
      _("Align the right edge of the items."), wxBitmap(align_right_xpm) );
  wx_menu_append_item
    ( result, ID_ALIGN_TOP, _("&Top"),
      _("Align the top edge of the items."), wxBitmap(align_top_xpm) );
  wx_menu_append_item
    ( result,  ID_ALIGN_BOTTOM, _("&Bottom"),
      _("Align the bottom edge of the items."), wxBitmap(align_bottom_xpm) );
  wx_menu_append_item
    ( result, ID_ALIGN_CENTER_X, _("&Horizontal center"),
      _("Align the center of the items on the X-axis."),
      wxBitmap(align_horizontal_middle_xpm) );
  wx_menu_append_item
    ( result, ID_ALIGN_CENTER_Y, _("&Vertical center"),
      _("Align the center of the items on the Y-axis."),
      wxBitmap(align_vertical_middle_xpm) );

  return result;
} // ingame_view_frame::create_align_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the menu with various edit modes.
 */
wxMenu* bf::ingame_view_frame::create_edit_mode_menu() const
{
  wxMenu* const result( new wxMenu() );

  wx_menu_append_item
    ( result, ID_EDIT_ACTIVE_LAYER, _("&Active layer\tF3"),
      _("The operations are done in the selected layer."),
      wxBitmap(edit_active_layer_xpm) );
  wx_menu_append_item
    ( result, ID_EDIT_SAME_TAG,
      _("Layers &tagged as the active layer\tCtrl+F3"),
      _("The operations are done on all layers having the same tag than the "
        "active layer."), wxBitmap(edit_same_tag_xpm) );
  wx_menu_append_item
    ( result, ID_EDIT_ALL, _("All &layers\tShift+F3"),
      _("The operations are done on all layers."),
      wxBitmap(edit_all_layers_xpm) );

  return result;
} // ingame_view_frame::create_edit_mode_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the view menu.
 */
wxMenu* bf::ingame_view_frame::create_view_menu() const
{
  wxMenu* result = new wxMenu();

  wxMenu* sub_menu = new wxMenu();

  sub_menu->AppendCheckItem
    ( ID_GRID_VISIBLE, _("&Visible\tCtrl+G"),
      _("Toggle the visibility of the grid.") );
  sub_menu->AppendCheckItem
    ( ID_GRID_MAGNETISM, _("&Magnetism\tCtrl+M"),
      _("Toggle the magnetism of the grid.") );

  sub_menu->AppendSeparator();
  sub_menu->Append
    ( ID_SET_GRID_ON_SELECTION,
      _("&Configure on the selection\tAlt+G"),
      _("Configuration of the grid on the selection.") );
  sub_menu->Append
    ( ID_SHOW_GRID_PROPERTIES, _("&Configure...\tCtrl+Shift+G"),
      _("Configuration of the grid.") );

  result->Append
    ( wxID_ANY, _("Gri&d"), sub_menu, _("Options of the grid.") );

  result->AppendCheckItem
    ( ID_WIREFRAME, _("&Wireframe\tW"), _("Display items in wireframe mode.") );
  result->AppendCheckItem
    ( ID_GRAPHISM, _("&Graphic\tG"), _("Display the sprites of the items.") );
  result->AppendCheckItem
    ( ID_ID_VISIBLE, _("&Item identifiers\tI"),
      _("Display the id of the items.") );
  result->AppendCheckItem
    ( ID_RELATIONSHIP, _("&Relationship\tR"),
      _("Display the relationship among the items.") );
  result->AppendCheckItem
    ( ID_BRIGHT_BACKGROUND, _("&Bright background\tB"),
      _("Use a bright background.") );

  result->AppendSeparator();
  result->AppendCheckItem
    ( ID_LEVEL_OVERVIEW, _("&Global view\tV"),
      _("Display a global view of the level.") );

  result->AppendSeparator();
  sub_menu = new wxMenu;

  wx_menu_append_item
    ( sub_menu, wxID_ZOOM_IN, _("Zoom more\tCtrl+Wheel up"),
      _("Increase the zoom ratio."), wxBitmap(zoom_in_xpm) );
  wx_menu_append_item
    ( sub_menu, wxID_ZOOM_OUT, _("Zoom less\tCtrl+Wheel down"),
      _("Decrease the zoom ratio."), wxBitmap(zoom_out_xpm) );
  wx_menu_append_item
    ( sub_menu,  wxID_ZOOM_100, _("No zoom"), _("Remove the zoom ratio."),
      wxBitmap(zoom_original_xpm) );

  result->Append(wxID_ANY, _("&Zoom"), sub_menu, _("Adjust the zoom ratio.") );

  return result;
} // ingame_view_frame::create_view_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the reference_item_field menu.
 */
wxMenu* bf::ingame_view_frame::create_reference_item_field_menu()
{
  return m_reference_item_field_popup_menu;
} // ingame_view_frame::create_reference_item_field_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the layer menu.
 * \param layer_menu The layer menu.
 */
wxMenu* bf::ingame_view_frame::create_layer_menu(wxMenu* layer_menu)
{
  return layer_menu;
} // ingame_view_frame::create_layer_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the toolbar.
 */
void bf::ingame_view_frame::create_toolbar()
{
  wxToolBar* bar = CreateToolBar();

  bar->AddTool
    ( wxID_SAVE, _("&Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE),
      _("Save") );
  bar->AddTool
    ( ID_COMPILE_LEVEL, _("&Compile"), wxBitmap(compile_xpm), _("Compile") );
  bar->AddTool
    ( ID_QUICK_COMPILE_LEVEL, _("&Quick compile"), wxBitmap(quick_compile_xpm),
      _("Compile with no optimization") );
  bar->AddTool
    ( ID_RUN_LEVEL, _("&Run level"), wxBitmap(run_xpm), _("Run the level") );
  bar->AddSeparator();

  bar->AddTool
    ( wxID_CUT, _("Cu&t"), wxArtProvider::GetBitmap(wxART_CUT), _("Cut") );
  bar->AddTool
    ( wxID_COPY, _("&Copy"), wxArtProvider::GetBitmap(wxART_COPY), _("Copy") );
  bar->AddTool
    ( wxID_PASTE, _("&Paste"), wxArtProvider::GetBitmap(wxART_PASTE),
      _("Paste") );
  bar->AddTool
    ( wxID_DELETE, _("&Delete"), wxArtProvider::GetBitmap(wxART_DELETE),
      _("Delete the selection") );
  bar->AddSeparator();

  bar->AddTool
    ( wxID_UNDO, _("&Undo"), wxArtProvider::GetBitmap(wxART_UNDO), _("Undo") );
  bar->AddTool
    ( wxID_REDO, _("&Redo"), wxArtProvider::GetBitmap(wxART_REDO), _("Redo") );
  bar->AddSeparator();

  bar->AddTool
    ( ID_CLONE, _("C&lone"), wxBitmap(clone_xpm), _("Clone") );
  bar->AddTool
    ( ID_OFFSET, _("&Offset"), wxBitmap(offset_xpm), _("Offset") );
  bar->AddTool
    ( ID_ALIGN, _("&Align"), wxBitmap(align_left_xpm), _("Align") );
  bar->AddTool
    ( ID_ARRANGE, _("&Arrange"), wxBitmap(adjust_horizontally_xpm),
      _("Arrange") );
  bar->AddSeparator();

  bar->AddCheckTool
    ( ID_GRID_VISIBLE, _("Gri&d"), wxBitmap(grid_xpm), wxBitmap(grid_xpm),
      _("Grid") );
  bar->AddCheckTool
    ( ID_GRID_MAGNETISM, _("&Magnetism"), wxBitmap(magnetism_xpm),
      wxBitmap(magnetism_xpm), _("Magnetism") );
  bar->AddCheckTool
    ( ID_ID_VISIBLE, _("&Identifiers"), wxBitmap(text_xpm), wxBitmap(text_xpm),
      _("Identifiers") );
  bar->AddCheckTool
    ( ID_WIREFRAME, _("&Wireframe"), wxBitmap(wireframe_xpm),
      wxBitmap(wireframe_xpm), _("Wireframe") );
  bar->AddCheckTool
    ( ID_GRAPHISM, _("&Graphism"), wxBitmap(graphism_xpm),
      wxBitmap(graphism_xpm), _("Graphism") );
  bar->AddCheckTool
    ( ID_RELATIONSHIP, _("&Relationship"), wxBitmap(relationship_xpm),
      wxBitmap(relationship_xpm), _("Relationship") );
  bar->AddCheckTool
    ( ID_LEVEL_OVERVIEW, _("O&verview"), wxBitmap(overview_xpm),
      wxBitmap(overview_xpm), _("Overview") );
  bar->AddCheckTool
    ( ID_BRIGHT_BACKGROUND, _("&Bright background"), wxBitmap(bright_xpm),
      wxBitmap(bright_xpm), _("Bright background") );
  bar->AddSeparator();

  m_zoom_spin = new wxSpinCtrl( bar, ID_ZOOM );
  m_zoom_spin->SetRange(1, std::numeric_limits<int>::max());
  m_zoom_spin->SetValue(100);

  bar->AddTool
    ( wxID_ZOOM_IN, _("Zoom &in"), wxBitmap(zoom_in_xpm), _("Zoom in") );
  bar->AddTool
    ( wxID_ZOOM_OUT, _("Zoom &out"), wxBitmap(zoom_out_xpm), _("Zoom out") );
  bar->AddTool
    ( wxID_ZOOM_100, _("No zoom"), wxBitmap(zoom_original_xpm), _("No zoom") );
  bar->AddControl(m_zoom_spin);

  bar->AddTool
    ( ID_EDIT_MODE, _("&Edit mode"), wxBitmap(edit_active_layer_xpm),
      _("Tells on which objects are applied the operations.") );

  bar->Realize();
} // ingame_view_frame::create_toolbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of the toolbar.
 */
void bf::ingame_view_frame::update_toolbar() const
{
  wxToolBar* bar = GetToolBar();

  bar->ToggleTool(ID_GRID_VISIBLE, m_ingame_view->get_grid_visibility());
  bar->ToggleTool(ID_GRID_MAGNETISM,
                  m_ingame_view->get_grid().get_magnetism_active());
  bar->ToggleTool(ID_ID_VISIBLE, m_ingame_view->get_id_visibility());
  bar->ToggleTool
    (ID_BRIGHT_BACKGROUND, m_ingame_view->get_bright_background());
  bar->ToggleTool(ID_GRAPHISM, m_ingame_view->get_graphic_drawing());
  bar->ToggleTool(ID_WIREFRAME, m_ingame_view->get_wireframe_drawing());
  bar->ToggleTool(ID_RELATIONSHIP, m_ingame_view->get_relationship_drawing());
  bar->ToggleTool(ID_LEVEL_OVERVIEW, m_overview->IsVisible());

  m_zoom_spin->SetValue(m_ingame_view->get_zoom());
  if(m_ingame_view->get_zoom()==1)
    bar->EnableTool(wxID_ZOOM_OUT, false);
  else
    bar->EnableTool(wxID_ZOOM_OUT, true);

  const bool has_selection = m_ingame_view->has_selection();
  bar->EnableTool(wxID_CUT, has_selection);
  bar->EnableTool(wxID_COPY, has_selection);
  bar->EnableTool(wxID_DELETE, has_selection);
  bar->EnableTool(ID_CLONE, has_selection);
  bar->EnableTool(ID_OFFSET, has_selection);
  bar->EnableTool(ID_ALIGN, has_selection);
  bar->EnableTool(ID_ARRANGE, has_selection);

  bar->EnableTool( wxID_PASTE, m_ingame_view->get_clipboard().has_items() );

  bar->EnableTool(wxID_UNDO, m_ingame_view->get_history().can_undo());
  bar->EnableTool(wxID_REDO, m_ingame_view->get_history().can_redo());

  update_edit_mode();
} // ingame_view_frame::update_toolbar()

/*----------------------------------------------------------------------------*/
/**
 * \brief Updates the icon of the edit mode button in the toolbar.
 */
void bf::ingame_view_frame::update_edit_mode() const
{
  wxBitmap icon;

  switch( m_ingame_view->get_edit_mode().get_value() )
    {
    case edit_mode::active_layer:
      icon = wxBitmap(edit_active_layer_xpm);
      break;
    case edit_mode::layers_by_tag:
      icon = wxBitmap(edit_same_tag_xpm);
      break;
    case edit_mode::all_layers:
      icon = wxBitmap(edit_all_layers_xpm);
      break;
    }

  GetToolBar()->SetToolNormalBitmap( ID_EDIT_MODE, icon );
} // ingame_view_frame::update_edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the accelerators for the actions in the menus.
 */
void bf::ingame_view_frame::create_accelerators()
{
  m_accelerator.add_accelerator
    ( wxID_SELECTALL, (int)'A', accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( ID_SELECT_NONE, (int)'A', accelerator_table::control_pressed,
      accelerator_table::alt_released, accelerator_table::shift_pressed );
  m_accelerator.add_accelerator( ID_BRIGHT_BACKGROUND, (int)'B' );

  m_accelerator.add_accelerator
    ( wxID_COPY, (int)'C', accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( ID_GRID_VISIBLE, (int)'G', accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( ID_GRID_MAGNETISM, (int)'M', accelerator_table::control_pressed );

  m_accelerator.add_accelerator
    ( ID_SHOW_GRID_PROPERTIES, (int)'G', accelerator_table::control_pressed,
      accelerator_table::alt_released, accelerator_table::shift_pressed );
  m_accelerator.add_accelerator
    ( ID_SET_GRID_ON_SELECTION, (int)'G', accelerator_table::control_released,
      accelerator_table::alt_pressed );

  m_accelerator.add_accelerator( ID_ID_VISIBLE, (int)'I' );

  m_accelerator.add_accelerator
    ( wxID_SAVE, (int)'S', accelerator_table::control_pressed );

  m_accelerator.add_accelerator
    ( wxID_SAVEAS, (int)'S', accelerator_table::control_pressed,
      accelerator_table::alt_released, accelerator_table::shift_pressed );

  m_accelerator.add_accelerator
    ( wxID_PASTE, (int)'V', accelerator_table::control_pressed );

  m_accelerator.add_accelerator( ID_LEVEL_OVERVIEW, (int)'V' );

  m_accelerator.add_accelerator
    ( wxID_CUT, (int)'X', accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( wxID_REDO, (int)'Y', accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( wxID_UNDO, (int)'Z', accelerator_table::control_pressed );

  m_accelerator.add_accelerator( wxID_DELETE, WXK_DELETE );

  m_accelerator.add_accelerator
    ( ID_MOVE_SELECTION_UP, WXK_UP, accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( ID_MOVE_SELECTION_DOWN, WXK_DOWN, accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( ID_MOVE_SELECTION_LEFT, WXK_LEFT, accelerator_table::control_pressed );
  m_accelerator.add_accelerator
    ( ID_MOVE_SELECTION_RIGHT, WXK_RIGHT, accelerator_table::control_pressed );

  m_accelerator.add_accelerator( ID_PREVIOUS_LAYER, WXK_PAGEUP );
  m_accelerator.add_accelerator( ID_NEXT_LAYER, WXK_PAGEDOWN );
} // ingame_view_frame::create_accelerators()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the visible status of the grid.
 */
void bf::ingame_view_frame::toggle_grid_visibility()
{
  m_ingame_view->show_grid( !m_ingame_view->get_grid_visibility() );
  m_ingame_view->Refresh();
} // ingame_view_frame::toggle_grid_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the magnetism status of the grid.
 */
void bf::ingame_view_frame::toggle_grid_magnetism()
{
  grid g( m_ingame_view->get_grid() );
  g.set_magnetism_active( !g.get_magnetism_active() );
  m_ingame_view->set_grid( g );
} // ingame_view_frame::toggle_grid_magnetism()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the visible status of the identifiers.
 */
void bf::ingame_view_frame::toggle_id_visibility()
{
  m_ingame_view->set_id_visibility( !m_ingame_view->get_id_visibility() );
  m_ingame_view->Refresh();
} // ingame_view_frame::toggle_id_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the visible status of the relationship among items.
 */
void bf::ingame_view_frame::toggle_relationship_visibility()
{
  m_ingame_view->toggle_relationship_drawing();
  m_ingame_view->Refresh();
} // ingame_view_frame::toggle_relationship_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the use of a bright background.
 */
void bf::ingame_view_frame::toggle_bright_background()
{
  m_ingame_view->set_bright_background
    ( !m_ingame_view->get_bright_background() );
  m_ingame_view->Refresh();
} // ingame_view_frame::toggle_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the wireframe mode.
 */
void bf::ingame_view_frame::toggle_wireframe_mode()
{
  m_ingame_view->toggle_wireframe_drawing();
} // ingame_view_frame::toggle_wireframe_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the graphic mode.
 */
void bf::ingame_view_frame::toggle_graphic_mode()
{
  m_ingame_view->toggle_graphic_drawing();
} // ingame_view_frame::toggle_graphic_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show the grid configuration windows.
 */
void bf::ingame_view_frame::show_grid_configuration()
{
  grid_properties_frame dlg( this, m_ingame_view->get_grid() );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_ingame_view->set_grid( dlg.get_grid() );
      m_ingame_view->Refresh();
    }
} // ingame_view_frame::show_grid_configuration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the validity of current level.
 * \return true if the level is ok.
 */
bool bf::ingame_view_frame::check_level()
{
  bool result = m_ingame_view->check_level();

  level_check_result errors = m_ingame_view->get_check_result();
  m_errors->set_errors(errors);
  m_errors->Show(!result);

  return result;
} // ingame_view_frame::check_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the validity of current level.
 * \return true if the level is ok.
 */
void bf::ingame_view_frame::check_level_verbose()
{
  if ( check_level() )
    GetStatusBar()->SetStatusText( _("Check completed."), 0 );
} // ingame_view_frame::check_level_verbose()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the level.
 * \param o The level of optimization during the compilation.
 * \return True if level has been compiled.
 */
bool bf::ingame_view_frame::compile_level( unsigned int o )
{
  bool ok = false;
  GetStatusBar()->SetStatusText( _("Wait..."), 0 );
  Disable();
  Refresh();

  wxStopWatch time;

  if ( check_level() )
    {
      bool do_it = true;

      if ( m_level_file.IsEmpty() )
        do_it = save_as();

      if ( do_it )
        ok = compile_level_no_check( o );
    }

  Enable();
  GetStatusBar()->SetStatusText
    ( wxString::Format
      (_("Compilation done in %.2f s."), time.Time()/1000.0), 0 );

  return ok;
} // ingame_view_frame::compile_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the level without checking anything.
 * \param o The level of optimization during the compilation.
 */
bool bf::ingame_view_frame::compile_level_no_check( unsigned int o )
{
  bool ok = true;

  const std::string std_path( get_compiled_level_file_path() );
  std::ofstream f( std_path.c_str() );

  if (f)
    {
      compiled_file cf(f);
      m_ingame_view->compile( cf, o );
      set_compile_changed(false);
    }
  else
    {
      ok = false;
      wxMessageDialog dlg
        ( this, _("Error"), _("Can't open the level file."), wxOK );

      dlg.ShowModal();
    }

  return ok;
} // ingame_view_frame::compile_level_no_check()

/*----------------------------------------------------------------------------*/
/**
 * Get the full path of the compiled level file corresponding to the edited
 * level.
 */
std::string bf::ingame_view_frame::get_compiled_level_file_path() const
{
  wxFileName file(m_level_file);

  file.MakeAbsolute();
  file.SetExt( wxT("cl") );

  return wx_to_std_string(file.GetFullPath());
} // ingame_view_frame::get_compiled_level_file_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the content of a the menu bar or the popup menu. Entries are
 *        enabled/disabled, checkmarks are set or not.
 * \param m The menu to update
 */
template<typename MenuType>
void bf::ingame_view_frame::update_menu( MenuType& m ) const
{
  m.Enable(wxID_SAVE, is_changed());
  m.Enable(ID_COMPILE_LEVEL, is_compile_changed());
  m.Enable(ID_QUICK_COMPILE_LEVEL, is_compile_changed());
  m.Enable(wxID_CUT, m_ingame_view->has_selection());
  m.Enable(wxID_COPY, m_ingame_view->has_selection());
  m.Enable(wxID_PASTE, m_ingame_view->get_clipboard().has_items());
  m.Enable(wxID_DELETE, m_ingame_view->has_selection());
  m.Enable(ID_SELECT_NONE, m_ingame_view->has_selection());

  if ( m_ingame_view->get_history().can_undo() )
    m.SetLabel
      ( wxID_UNDO,
        wxString::Format
        ( _("Undo \"%s\"\tCTRL+Z"),
          m_ingame_view->get_history().get_undo_description().c_str() ) );
  else
    m.SetLabel(wxID_UNDO, _("Nothing to undo\tCTRL+Z"));

  if ( m_ingame_view->get_history().can_redo() )
    m.SetLabel
      ( wxID_REDO,
        wxString::Format
        ( _("Redo \"%s\"\tCTRL+Y"),
          m_ingame_view->get_history().get_redo_description().c_str() ) );
  else
    m.SetLabel(wxID_REDO, _("Nothing to redo\tCTRL+Y"));

  m.Enable(wxID_UNDO, m_ingame_view->get_history().can_undo());
  m.Enable(wxID_REDO, m_ingame_view->get_history().can_redo());

  if ( !m_ingame_view->empty() )
    m.Enable( wxID_SELECTALL, !m_ingame_view->get_active_layer().empty() );
  else
    m.Enable( wxID_SELECTALL, false );

  m.Enable(ID_ALIGN, m_ingame_view->has_selection());
  m.Enable(ID_ARRANGE, m_ingame_view->has_selection());
  m.Enable(ID_CLONE, m_ingame_view->has_selection());
  m.Enable(ID_OFFSET, m_ingame_view->has_selection());

  if ( !m_ingame_view->empty() && m_ingame_view->has_selection() &&
       !m_ingame_view->get_active_layer().empty() )
    {
      m.Enable(ID_MOVE_IN_LAYER, true);

      if ( m_ingame_view->get_active_index() > 0 )
        m.Enable(ID_MOVE_IN_LAYER_UP, true);
      else
        m.Enable(ID_MOVE_IN_LAYER_UP, false);

      if ( m_ingame_view->get_active_index() <
           m_ingame_view->get_level().layers_count()-1 )
        m.Enable(ID_MOVE_IN_LAYER_DOWN, true);
      else
        m.Enable(ID_MOVE_IN_LAYER_DOWN, false);
    }
  else
    {
      m.Enable(ID_MOVE_IN_LAYER, false);
      m.Enable(ID_MOVE_IN_LAYER_UP, false);
      m.Enable(ID_MOVE_IN_LAYER_DOWN, false);
    }

  m.Check(ID_GRID_VISIBLE, m_ingame_view->get_grid_visibility());
  m.Check(ID_GRID_MAGNETISM, m_ingame_view->get_grid().get_magnetism_active());
  m.Check(ID_ID_VISIBLE, m_ingame_view->get_id_visibility());
  m.Check(ID_BRIGHT_BACKGROUND, m_ingame_view->get_bright_background());
  m.Check(ID_GRAPHISM, m_ingame_view->get_graphic_drawing());
  m.Check(ID_WIREFRAME, m_ingame_view->get_wireframe_drawing());
  m.Check(ID_RELATIONSHIP, m_ingame_view->get_relationship_drawing());
  m.Check(ID_LEVEL_OVERVIEW, m_overview->IsVisible());
} // ingame_view_frame::update_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief A layer menu for selection movement is opened.
 * \param layer_menu The layer menu.
 * \param moving_layer_index The map associated wxID with layer index.
 */
void bf::ingame_view_frame::update_moving_layer_menu
(wxMenu* moving_layer_menu, std::map<int, unsigned int>& moving_layer_index )
{
  while ( moving_layer_menu->GetMenuItemCount() > 0 )
    {
      int id = moving_layer_menu->GetMenuItems().front()->GetId();
      Disconnect(id);
      moving_layer_menu->Delete(id);
    }
  moving_layer_index.clear();

  wx_menu_append_item
    ( moving_layer_menu, ID_MOVE_IN_LAYER_UP, _("&Backward"),
      _("Move the selection one layer backward."),
      wxBitmap(move_backward_xpm) );
  wx_menu_append_item
    ( moving_layer_menu, ID_MOVE_IN_LAYER_DOWN, _("&Forward"),
      _("Move the selection one layer forward."), wxBitmap(move_forward_xpm) );


  moving_layer_menu->AppendSeparator();
  const level& lvl( m_ingame_view->get_level() );
  for (unsigned int i=0; i!=lvl.layers_count(); ++i)
    {
      const layer& lay = lvl.get_layer(i); 
      
      int id = 
	moving_layer_menu->Append
	( wxID_ANY, 
	  std_to_wx_string(lay.get_displayed_name()))->GetId();

      bool active (false);
      if ( !m_ingame_view->empty() && m_ingame_view->has_selection() )
	if ( !m_ingame_view->get_active_layer().empty() )
	  active = ( m_ingame_view->get_active_index() != i);
      moving_layer_menu->Enable(id, active);

      Connect
	( id, wxEVT_COMMAND_MENU_SELECTED,
	  wxCommandEventHandler(ingame_view_frame::on_move_in_other_layer) );

      moving_layer_index[ id ] = i;
    }
} // update_moving_layer_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief A layer menu is opened.
 * \param layer_menu The layer menu.
 * \param layer_index The map associated wxID with layer index.
 */
void bf::ingame_view_frame::update_layer_menu
(wxMenu* layer_menu, std::map<int, unsigned int>& layer_index )
{
  while ( layer_menu->GetMenuItemCount() > 0 )
    {
      int id = layer_menu->GetMenuItems().front()->GetId();
      Disconnect(id);
      layer_menu->Delete(id);
    }
  layer_index.clear();

  const level& lvl( m_ingame_view->get_level() );
  for (unsigned int i=0; i!=lvl.layers_count(); ++i)
    {
      const layer& lay = lvl.get_layer(i); 
      
      int id = 
        layer_menu->AppendCheckItem
	( wxID_ANY, 
	  std_to_wx_string(lay.get_displayed_name()))->GetId();

      bool active (false);
      if ( !m_ingame_view->empty() )
        active = ( m_ingame_view->get_active_index() != i);
      layer_menu->Check(id, !active);

      Connect
	( id, wxEVT_COMMAND_MENU_SELECTED,
	  wxCommandEventHandler(ingame_view_frame::on_select_layer) );

      layer_index[ id ] = i;
    }
} // update_layer_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an entry in reference_item_field menu.
 * \param field The field to set.
 * \param id_item The identifiant of the item to set.
 * \param item_to_selection Indicates if we set item in a selection's field. 
 */
void bf::ingame_view_frame::add_entry_reference_item_field_menu
( const std::string& field, const std::string& id_item, bool item_to_selection )
{
  int id;

  if ( item_to_selection )
    id = m_reference_item_field_popup_menu->Append
      ( wxID_ANY, std_to_wx_string
        ("Set " + id_item + " in selection." + field) )->GetId();
  else
    id = m_reference_item_field_popup_menu->Append
      ( wxID_ANY, std_to_wx_string
        ("Set selection in " + id_item + "." + field) )->GetId();

  if ( item_to_selection )
    Connect
      ( id, wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(ingame_view_frame::on_set_item_to_selection) );
  else
    Connect
      ( id, wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler(ingame_view_frame::on_set_selection_to_item) );

  m_reference_item_field_popup_fields[ id ] = field;
} // ingame_view_frame::add_entry_reference_item_field_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an entry of list in reference_item_field menu.
 * \param field The field to set.
 * \param id_item The identifiant of the item to set.
 * \param item_to_selection Indicates if we set item in a selection's field.  
 * \param add Indicates that we must add selection in list.
 */
void bf::ingame_view_frame::add_entry_reference_item_list_field_menu
( const std::string& field, const std::string& id_item, 
  bool item_to_selection, bool add )
{
  int id;
  std::string operation("Set ");
  if ( add )
    operation = "Add ";

  if ( item_to_selection )
    id = m_reference_item_field_popup_menu->Append
      ( wxID_ANY, 
        std_to_wx_string
        (operation + id_item + " in list selection." + field) )->GetId();
  else
    id = m_reference_item_field_popup_menu->Append
      ( wxID_ANY, 
        std_to_wx_string
        ( operation + "selection in list " + id_item + "." + field) )->GetId();
  
  if ( add )
    {
      if ( item_to_selection )
        Connect
          ( id, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler
            (ingame_view_frame::on_add_list_item_to_selection) );
      else
        Connect
          ( id, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler
            (ingame_view_frame::on_add_list_selection_to_item) );
    }
  else if ( item_to_selection )
    Connect
      ( id, wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler
        (ingame_view_frame::on_set_list_item_to_selection) );
  else
    Connect
      ( id, wxEVT_COMMAND_MENU_SELECTED,
        wxCommandEventHandler
        (ingame_view_frame::on_set_list_selection_to_item) );

  m_reference_item_field_popup_fields[ id ] = field;
} // ingame_view_frame::add_entry_reference_item_field_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear reference_item_field menu.
 */
void bf::ingame_view_frame::clear_reference_item_field_menu()
{
  while ( m_reference_item_field_popup_menu->GetMenuItemCount() > 0 )
    {
      int id = 
        m_reference_item_field_popup_menu->GetMenuItems().front()->GetId();
      Disconnect(id);
      m_reference_item_field_popup_menu->Delete(id);
    }
  
  m_reference_item_field_popup_fields.clear();
} // ingame_view_frame::clear_reference_item_field_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add entry in reference_item_field menu to set item in selection's 
 field.
 */
void bf::ingame_view_frame::add_entry_item_to_selection()
{
  if ( m_reference_item_field_instance != NULL )
    if ( ! m_reference_item_field_instance->get_id().empty() )
      {
        std::set<std::string> item_reference_fields;
        std::set<std::string> item_reference_list_fields;
        
        const item_selection& selection = 
          m_ingame_view->get_level().get_selection();
        item_selection::const_iterator it;
        
        for ( it=selection.begin(); it!=selection.end(); ++it )
          (*it)->get_item_reference_field_names
            ( item_reference_fields, item_reference_list_fields );
        
        std::set<std::string>::const_iterator it_field;
        for ( it_field = item_reference_fields.begin();
              it_field != item_reference_fields.end(); ++it_field )
          add_entry_reference_item_field_menu
            (*it_field, m_reference_item_field_instance->get_id(), true );
        
        for ( it_field = item_reference_list_fields.begin();
              it_field != item_reference_list_fields.end(); ++it_field )
          add_entry_reference_item_list_field_menu
            (*it_field, m_reference_item_field_instance->get_id(), 
             true, false);
                
        for ( it_field = item_reference_list_fields.begin();
              it_field != item_reference_list_fields.end(); ++it_field )
          add_entry_reference_item_list_field_menu
            (*it_field, m_reference_item_field_instance->get_id(), 
             true, true);
      }
} // ingame_view_frame::add_entry_item_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add entry in reference_item_field menu to set selection in item's 
 field.
 */
void bf::ingame_view_frame::add_entry_selection_to_item()
{
  if ( m_reference_item_field_instance != NULL )
    {
      const item_selection& selection = 
        m_ingame_view->get_level().get_selection();
      item_selection selection_with_id;
      item_selection::const_iterator it;
          
      for ( it=selection.begin(); it!=selection.end(); ++it )
        if ( ! (*it)->get_id().empty() )
          selection_with_id.insert(*it);

      if ( ! selection_with_id.empty() )
        {
          std::set<std::string> item_reference_fields;
          std::set<std::string> item_reference_list_fields;
          
          m_reference_item_field_instance->get_item_reference_field_names
              ( item_reference_fields, item_reference_list_fields );
          
          std::set<std::string>::const_iterator it_field;
          for ( it_field = item_reference_fields.begin();
                it_field != item_reference_fields.end(); ++it_field )
            add_entry_reference_item_field_menu
              (*it_field, m_reference_item_field_instance->get_id(), false );

          for ( it_field = item_reference_list_fields.begin();
                it_field != item_reference_list_fields.end(); ++it_field )
            add_entry_reference_item_list_field_menu
              (*it_field, m_reference_item_field_instance->get_id(), 
               false, false);

          for ( it_field = item_reference_list_fields.begin();
                it_field != item_reference_list_fields.end(); ++it_field )
            add_entry_reference_item_list_field_menu
              (*it_field, m_reference_item_field_instance->get_id(), 
               false, true);
        }
    }
} // ingame_view_frame::add_entry_selection_to_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief A reference_item_field menu is opened.
 */
void bf::ingame_view_frame::update_reference_item_field_menu()
{
  clear_reference_item_field_menu();

  add_entry_item_to_selection();
  m_reference_item_field_popup_menu->AppendSeparator();
  add_entry_selection_to_item();
} // update_reference_item_field_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Event sent to a resized window.
 * \param event The event.
 */
void bf::ingame_view_frame::on_size(wxSizeEvent& event)
{
  adjust_scrollbars();

  if ( !m_ingame_view->empty() )
    m_ingame_view->set_view_position
      ( m_h_scrollbar->GetThumbPosition(),
        m_ingame_view->get_level().get_height()
        - m_v_scrollbar->GetThumbPosition()
        - 100 * m_ingame_view->get_view_size().y / m_ingame_view->get_zoom() );

  event.Skip();
} // ingame_view_frame::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::ingame_view_frame::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  m_ingame_view->render();
} // ingame_view_frame::on_paint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the windows with the properties of this level.
 * \param event The paint event.
 */
void bf::ingame_view_frame::on_activate( wxActivateEvent& WXUNUSED(event) )
{
  if ( m_layout.set_current_level_view(*this) )
    {
      m_ingame_view->SetFocus();
      m_ingame_view->update_layout();
    }

  adjust_scrollbars();
} // ingame_view_frame::on_activate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the position of the view.
 * \param event The scroll event.
 */
void bf::ingame_view_frame::on_scroll( wxScrollEvent& WXUNUSED(event) )
{
#if 0
  m_ingame_view->set_view_position
    ( m_h_scrollbar->GetThumbPosition(),
      m_ingame_view->get_layer_view_size().y -
      m_v_scrollbar->GetThumbPosition() -
      100 * m_ingame_view->get_view_size().y / m_ingame_view->get_zoom() );
#else
  m_ingame_view->set_view_position
    ( m_h_scrollbar->GetThumbPosition(),
      m_ingame_view->get_level().get_height() -
      m_v_scrollbar->GetThumbPosition() -
      100 * m_ingame_view->get_view_size().y / m_ingame_view->get_zoom() );
#endif
} // ingame_view_frame::on_scroll()

/*----------------------------------------------------------------------------*/
/**
 * \brief Procedure called when closing the window.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_close(wxCloseEvent& event)
{
  bool ok = true;

  if ( m_changed )
    {
      wxMessageDialog dlg
        ( this,
          _("This level has changes that are not saved. Save now?"),
          _("Level is not saved."), wxYES_NO | wxCANCEL );

      int answer = dlg.ShowModal();

      if ( answer == wxID_YES )
        ok = save();
      else if ( answer == wxID_CANCEL )
        ok = false;
    }

    if ( ok && m_compile_changed )
    {
      wxMessageDialog dlg
        ( this,
          _("This level has changes that are not compiled. Compile now?"),
          _("Level is not compiled."), wxYES_NO | wxCANCEL );

      int answer = dlg.ShowModal();

      if ( answer == wxID_YES )
        ok = compile_level();
      else if ( answer == wxID_CANCEL )
        ok = false;
    }

  if ( ok || !event.CanVeto() )
    {
      m_layout.remove_level_view(*this);
      m_layout.get_properties_frame().clear();
      m_layout.get_layer_list_frame().set_level_view(NULL);

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
void bf::ingame_view_frame::on_context_menu(wxContextMenuEvent& event)
{
  update_menu( m_popup_menu );
  update_menu( *GetMenuBar() );

  if ( m_ingame_view->has_selection() )
    {
      wxPoint point; 

      if (  event.GetPosition() == wxDefaultPosition ) 
        point = m_ingame_view->compute_mouse_position( event.GetPosition() );
      else
        point = 
          m_ingame_view->compute_mouse_position
          ( ScreenToClient( event.GetPosition() ) );
      m_reference_item_field_instance = 
        m_ingame_view->get_level().first_item( wx_to_position( point ) );

      if ( m_reference_item_field_instance != NULL )
        update_reference_item_field_menu();
    }

  if ( event.GetPosition() == wxDefaultPosition )
    PopupMenu( &m_popup_menu, event.GetPosition() );
  else // event.GetPosition() is the mouse position on the screen
    PopupMenu( &m_popup_menu, ScreenToClient(event.GetPosition()) );
} // ingame_view_frame::on_context_menu()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu entry "Level/Properties".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_level_properties
( wxCommandEvent& WXUNUSED(event) )
{
  level_properties_frame dlg(this);
  dlg.init_from( m_ingame_view->get_level() );

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_ingame_view->do_action
        ( new action_resize_level
          ( dlg.get_name(), dlg.get_width(), dlg.get_height(),
            dlg.get_music() ) );

      if ( ! m_ingame_view->get_level().check_item_position() )
        m_ingame_view->show_item_position_error(this);
    }
} // ingame_view_frame::on_level_properties()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu entry "Level/Statistics".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_level_statistics
( wxCommandEvent& WXUNUSED(event) )
{
  wxString text;
  std::set<std::string> image_set;
  const level& lvl( m_ingame_view->get_level() );

  text += wxString::Format(_("%d layers:\n"), lvl.layers_count());

  for (unsigned int i=0; i!=lvl.layers_count(); ++i)
    {
      const layer& lay = lvl.get_layer(i);

      text += _("Layer ") + std_to_wx_string(lay.get_displayed_name()) +
	wxString::Format( _(" has %d items.\n"), lay.get_items_count() );
      
      layer::item_iterator it;

      for (it=lay.item_begin(); it!=lay.item_end(); ++it)
        if ( it->get_rendering_parameters().has_sprite() )
          image_set.insert
            ( it->get_rendering_parameters().get_sprite().get_image_name() );
    }

  double mem(0);
  std::set<std::string>::const_iterator it;

  for (it=image_set.begin(); it!=image_set.end(); ++it)
    {
      const wxBitmap img =
        image_pool::get_instance().get_image(std_to_wx_string(*it));
      mem += img.GetWidth() * img.GetHeight() * 4;
    }

  text += wxString::Format
    ( _("\n%.2f Mb in %d images."), mem / 1024 / 1024, image_set.size() );

  wxMessageDialog dlg(this, text, _("Level statistics"), wxOK);
  dlg.ShowModal();
} // ingame_view_frame::on_level_statistics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Undo".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_undo( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->undo();
  update_toolbar();
} // ingame_view_frame::on_undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Redo".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_redo( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->redo();
  update_toolbar();
} // ingame_view_frame::on_redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Cut".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_cut( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->cut_to_clipboard();
  update_toolbar();
} // ingame_view_frame::on_cut()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Copy".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_copy( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->copy_to_clipboard();
  update_toolbar();
} // ingame_view_frame::on_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Paste".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_paste( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->paste_from_clipboard();
  update_toolbar();

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_paste()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Delete".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_delete( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_delete_selection
      ( m_ingame_view->get_level(), m_ingame_view->get_edit_layers() ) );
  update_toolbar();
} // ingame_view_frame::on_delete()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Select all".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_select_all( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->select_all();
  update_toolbar();
} // ingame_view_frame::on_select_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Select none".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_select_none( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->clear_selection();
  update_toolbar();
} // ingame_view_frame::on_select_none()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Check level".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_check_level( wxCommandEvent& WXUNUSED(event) )
{
  check_level_verbose();
} // ingame_view_frame::on_check_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Save level".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_save( wxCommandEvent& WXUNUSED(event) )
{
  save();
} // ingame_view_frame::on_save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Compile level".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_compile_level( wxCommandEvent& WXUNUSED(event) )
{
  compile_level();
} // ingame_view_frame::on_compile_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the level with no optimization.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_quick_compile_level
( wxCommandEvent& WXUNUSED(event) )
{
  compile_level(0);
} // ingame_view_frame::on_quick_compile_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Run game".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_run_level( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_ingame_view->get_level().layers_count() == 0 )
    return;

  try
    {
      level_runner r
        ( m_layout.get_main_frame().get_run_configuration(),
          m_ingame_view->get_level(), m_ingame_view->get_active_index(),
          m_ingame_view->get_center_in_level().x,
          m_ingame_view->get_center_in_level().y );
      r.run();
    }
  catch( const std::exception& e )
    {
      wxMessageDialog dlg
        ( this, std_to_wx_string(e.what()), _("Cannot run the game"), wxOK );
      dlg.ShowModal();
    }
} // ingame_view_frame::on_run_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Close".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_menu_close( wxCommandEvent& WXUNUSED(event) )
{
  Close();
} // ingame_view_frame::on_menu_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Configure".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_show_grid_properties
(wxCommandEvent& WXUNUSED(event))
{
  show_grid_configuration();
} // ingame_view_frame::on_show_grid_properties()


/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Configure on selection".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_set_grid_on_selection
(wxCommandEvent& WXUNUSED(event))
{
  m_ingame_view->set_grid_on_selection();
} // ingame_view_frame::on_set_grid_on_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Magnetism".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_grid_magnetism( wxCommandEvent& WXUNUSED(event) )
{
  toggle_grid_magnetism();
  GetToolBar()->ToggleTool
    (ID_GRID_MAGNETISM, m_ingame_view->get_grid().get_magnetism_active());
} // ingame_view_frame::on_grid_magnetism()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "grid Visible".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_grid_visible( wxCommandEvent& WXUNUSED(event) )
{
  toggle_grid_visibility();
  GetToolBar()->ToggleTool
    (ID_GRID_VISIBLE, m_ingame_view->get_grid_visibility());
} // ingame_view_frame::on_grid_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "id Visible".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_id_visible( wxCommandEvent& WXUNUSED(event) )
{
  toggle_id_visibility();
  GetToolBar()->ToggleTool(ID_ID_VISIBLE, m_ingame_view->get_id_visibility());
} // ingame_view_frame::on_id_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Relationship".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_relationship( wxCommandEvent& WXUNUSED(event) )
{
  toggle_relationship_visibility();
  GetToolBar()->ToggleTool
    (ID_RELATIONSHIP, m_ingame_view->get_relationship_drawing());
} // ingame_view_frame::on_relationship()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Wireframe".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_wireframe( wxCommandEvent& WXUNUSED(event) )
{
  toggle_wireframe_mode();
  GetToolBar()->ToggleTool
    (ID_WIREFRAME, m_ingame_view->get_wireframe_drawing());
} // ingame_view_frame::on_wireframe()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Graphic".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_graphic( wxCommandEvent& WXUNUSED(event) )
{
  toggle_graphic_mode();
  GetToolBar()->ToggleTool(ID_GRAPHISM, m_ingame_view->get_graphic_drawing());
} // ingame_view_frame::on_graphic()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Global view".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_level_overview( wxCommandEvent& event )
{
  m_overview->Show( event.IsChecked() );
  GetToolBar()->ToggleTool(ID_LEVEL_OVERVIEW, m_overview->IsVisible());
} // ingame_view_frame::on_level_overview()

/*----------------------------------------------------------------------------*/
/**
 * \brief A menu entry is highlighted.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_menu_highlight( wxMenuEvent& event )
{
  GetStatusBar()->SetStatusText
    ( m_popup_menu.GetHelpString(event.GetMenuId()), 0 );

  if ( event.GetId() == ID_MOVE_IN_LAYER )
    {
      update_moving_layer_menu
        (m_moving_layer_popup_menu, m_moving_layer_popup_index);
      update_moving_layer_menu(m_moving_layer_menu, m_moving_layer_index);
    }
} // ingame_view_frame::on_menu_highlight()

/*----------------------------------------------------------------------------*/
/**
 * \brief A menu is opened.
 * \param event The event.
 */
void bf::ingame_view_frame::on_open_menu( wxMenuEvent& event )
{
  update_menu( m_popup_menu );
  update_menu( *GetMenuBar() );

  if ( event.GetId() == ID_MOVE_IN_LAYER )
    {
      update_moving_layer_menu
        (m_moving_layer_popup_menu, m_moving_layer_popup_index);
      update_moving_layer_menu(m_moving_layer_menu, m_moving_layer_index);
    }
} // ingame_view_frame::on_menu_open()

/*----------------------------------------------------------------------------*/
/**
 * \brief Arranges the selected items on the x-axis.
 * \param event The event that triggers the arrangeing.
 */
void bf::ingame_view_frame::on_arrange_horizontally
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_arrange_selection_horizontally
      ( m_ingame_view->get_edit_selection() ) );

  if ( !m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_arrange_horizontally()

/*----------------------------------------------------------------------------*/
/**
 * \brief Arranges the selected items on the y-axis.
 * \param event The event that triggers the arrangeing.
 */
void bf::ingame_view_frame::on_arrange_vertically
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_arrange_selection_vertically
      ( m_ingame_view->get_edit_selection() ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_arrange_vertically()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the items on their left edge.
 * \param event The event.
 */
void bf::ingame_view_frame::on_align_left( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_align_selection_left
      ( m_ingame_view->get_edit_selection() ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_align_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the items on their right egde.
 * \param event The event.
 */
void bf::ingame_view_frame::on_align_right( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_align_selection_right
      ( m_ingame_view->get_edit_selection() ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_align_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the items on their top edge.
 * \param event The event.
 */
void bf::ingame_view_frame::on_align_top( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_align_selection_top
      ( m_ingame_view->get_edit_selection() ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the items on their bottom edge.
 * \param event The event.
 */
void bf::ingame_view_frame::on_align_bottom( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_align_selection_bottom
      ( m_ingame_view->get_edit_selection() ) );

  if ( !m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_align_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the center of the items on the X-axis.
 * \param event The event.
 */
void bf::ingame_view_frame::on_align_center_x( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_align_selection_center_x
      ( m_ingame_view->get_edit_selection() ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_align_center_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the center of the items on the Y-axis.
 * \param event The event.
 */
void bf::ingame_view_frame::on_align_center_y( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_align_selection_center_y
      ( m_ingame_view->get_edit_selection() ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_align_center_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clone the selection.
 * \param event The event.
 */
void bf::ingame_view_frame::on_clone( wxCommandEvent& WXUNUSED(event) )
{
  clone_selection_dialog dlg(*this);

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_ingame_view->do_action
        ( action_clone_selection::create_for_layers
          ( m_ingame_view->get_level(), m_ingame_view->get_edit_layers(),
            dlg.get_x_count(), dlg.get_y_count(),
            dlg.get_x_offset(), dlg.get_y_offset(),
            dlg.get_add_to_selection() ) );
      
      if ( ! m_ingame_view->get_active_layer().check_item_position() )
        m_ingame_view->show_item_position_error(this);
    }
} // ingame_view_frame::on_clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an offset to the selection.
 * \param event The event.
 */
void bf::ingame_view_frame::on_offset( wxCommandEvent& WXUNUSED(event) )
{
  offset_selection_dialog dlg(*this);

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_ingame_view->do_action
        ( new action_move_selection
          ( m_ingame_view->get_edit_selection(), dlg.get_x(), dlg.get_y() ) );

      if ( ! m_ingame_view->get_active_layer().check_item_position() )
        m_ingame_view->show_item_position_error(this);
    }
} // ingame_view_frame::on_offset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to moving selection in the top layer.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_in_layer_up
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_move_up( m_ingame_view->get_level() ) );

  m_ingame_view->set_active_index(m_ingame_view->get_active_index()-1);

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_move_in_layer_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to moving selection in the bottom
 *        layer.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_in_layer_down
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_move_down( m_ingame_view->get_level() ) );

  m_ingame_view->set_active_index(m_ingame_view->get_active_index()+1);

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_move_in_layer_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to moving selection in the bottom
 *        layer.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_in_other_layer
( wxCommandEvent& event )
{
  bool found = false;

  std::map<int, unsigned int>::iterator it = 
    m_moving_layer_index.find(event.GetId());
  
  if ( it == m_moving_layer_index.end() )
    {
      it = m_moving_layer_popup_index.find(event.GetId());
      found = ( it != m_moving_layer_popup_index.end() );
    }
  else
    found = true;

  if ( found )
    {
      unsigned int layer = it->second;
      
      m_ingame_view->do_action
	( new action_move_in_other_layer
	  ( m_ingame_view->get_level(),layer ) );
      
      m_ingame_view->set_active_index(layer);

      if ( ! m_ingame_view->get_active_layer().check_item_position() )
        m_ingame_view->show_item_position_error(this);
    }
} // ingame_view_frame::on_move_in_other_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to selecting a layer.
 * \param event The event.
 */
void bf::ingame_view_frame::on_select_layer( wxCommandEvent& event )
{
  bool found = false;

  std::map<int, unsigned int>::iterator it = 
    m_layer_index.find(event.GetId());
  
  if ( it == m_layer_index.end() )
    {
      it = m_layer_popup_index.find(event.GetId());
      found = ( it != m_layer_popup_index.end() );
    }
  else
    found = true;

  if ( found )
    m_ingame_view->set_active_index(it->second);
} // ingame_view_frame::on_select_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to set a reference item.
 * \param event The event.
 */
void bf::ingame_view_frame::on_set_item_to_selection( wxCommandEvent& event )
{
  std::string field_name( m_reference_item_field_popup_fields[event.GetId()] );

  const item_selection& selection = 
    m_ingame_view->get_level().get_selection();
  
  item_selection::const_iterator it;
  type_field field( field_name, type_field::item_reference_field_type );
  
  bf::item_reference_type value;
  value.set_value(m_reference_item_field_instance->get_id());
  action_group* g = 
    new action_group( std_to_wx_string("Set " + field_name + " field"));
  
  for ( it = selection.begin(); it != selection.end(); ++it )
    if ( (*it)->get_class().has_field
         ( field_name, type_field::item_reference_field_type ) )
      g->add_action
        ( new action_set_item_field<bf::item_reference_type>
          ( *it, field_name, value ) );
  
  m_ingame_view->do_action(g);
} // ingame_view_frame::on_set_item_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to set a reference item list.
 * \param event The event.
 */
void bf::ingame_view_frame::on_set_list_item_to_selection
  ( wxCommandEvent& event )
{
  std::string field_name( m_reference_item_field_popup_fields[event.GetId()] );

  const item_selection& selection = m_ingame_view->get_level().get_selection();
  
  item_selection::const_iterator it;
  type_field field( field_name, type_field::item_reference_field_type );
  field.set_is_list(true);
            
  bf::item_reference_type value;
  value.set_value(m_reference_item_field_instance->get_id());
  std::list<bf::item_reference_type> values;
  values.push_back(value);

  action_group* g = 
    new action_group( std_to_wx_string("Set " + field_name + " field"));
  
  for ( it = selection.begin(); it != selection.end(); ++it )
    if ( (*it)->get_class().has_field
         ( field_name, type_field::item_reference_field_type ) )
      g->add_action
        ( new action_set_item_field
          < std::list<bf::item_reference_type> >( *it, field_name, values ) );
  
  m_ingame_view->do_action(g);
} // ingame_view_frame::on_set_list_item_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to add a reference item list.
 * \param event The event.
 */
void bf::ingame_view_frame::on_add_list_item_to_selection
( wxCommandEvent& event )
{
  std::string field_name( m_reference_item_field_popup_fields[event.GetId()] );

  const item_selection& selection = m_ingame_view->get_level().get_selection();
  item_selection::const_iterator it;
  
  action_group* g = 
    new action_group( std_to_wx_string("Add " + field_name + " field"));

  for ( it = selection.begin(); it != selection.end(); ++it )
    if ( (*it)->get_class().has_field
         ( field_name, type_field::item_reference_field_type ) )
      {
        bf::item_reference_type value;
        value.set_value(m_reference_item_field_instance->get_id());
        std::list<bf::item_reference_type> values;
        (*it)->get_value(field_name, values);
        values.push_back(value);
                                  
        g->add_action
          ( new action_set_item_field
            < std::list<bf::item_reference_type> >(*it, field_name, values ) );
      }
  
  m_ingame_view->do_action(g);
} // ingame_view_frame::on_add_list_item_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to set a reference item.
 * \param event The event.
 */
void bf::ingame_view_frame::on_set_selection_to_item( wxCommandEvent& event )
{  
  std::string field_name( m_reference_item_field_popup_fields[event.GetId()] );

  const item_selection& selection = 
    m_ingame_view->get_level().get_selection();
  item_selection selection_with_id;
  item_selection::const_iterator it;
  
  for ( it = selection.begin(); it != selection.end(); ++it )
    if ( ! (*it)->get_id().empty() )
      selection_with_id.insert(*it);
  
  bf::item_reference_type value;
  value.set_value( (*(selection_with_id.begin()))->get_id() );
  
  m_ingame_view->do_action
    ( new action_set_item_field<bf::item_reference_type>
      ( m_reference_item_field_instance, field_name, value ) );
} // ingame_view_frame::on_set_selection_to_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to set a reference item list.
 * \param event The event.
 */
void bf::ingame_view_frame::on_set_list_selection_to_item
( wxCommandEvent& event )
{
  std::string field_name( m_reference_item_field_popup_fields[event.GetId()] );

  const item_selection& selection = 
    m_ingame_view->get_level().get_selection();
  item_selection::const_iterator it;
  
  std::list< bf::item_reference_type> values;
  for ( it = selection.begin(); it != selection.end(); ++it )
    if ( ! (*it)->get_id().empty() )
      {
        bf::item_reference_type value;
        value.set_value( (*it)->get_id() );
        values.push_back( value );
      }

  m_ingame_view->do_action
    ( new action_set_item_field< std::list< bf::item_reference_type > >
      ( m_reference_item_field_instance, field_name, values ) );
} // ingame_view_frame::on_set_list_selection_to_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the action that consists to add a reference item list.
 * \param event The event.
 */
void bf::ingame_view_frame::on_add_list_selection_to_item
( wxCommandEvent& event )
{
  std::string field_name( m_reference_item_field_popup_fields[event.GetId()] );

  const item_selection& selection = 
    m_ingame_view->get_level().get_selection();
  item_selection::const_iterator it;

  std::list< bf::item_reference_type> values;
  m_reference_item_field_instance->get_value(field_name, values);
  
  for ( it = selection.begin(); it != selection.end(); ++it )
    if ( ! (*it)->get_id().empty() )
      {
        bf::item_reference_type value;
        value.set_value( (*it)->get_id() );
        values.push_back( value );
      }

  m_ingame_view->do_action
    ( new action_set_item_field< std::list< bf::item_reference_type > >
      ( m_reference_item_field_instance, field_name, values ) );
} // ingame_view_frame::on_add_list_selection_to_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an offset of one unit up to the selection.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_selection_up
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_move_selection( m_ingame_view->get_edit_selection(), 0, 1 ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_move_selection_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an offset of one unit down to the selection.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_selection_down
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_move_selection( m_ingame_view->get_edit_selection(), 0, -1 ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_move_selection_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an offset of one unit on the left to the selection.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_selection_left
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_move_selection( m_ingame_view->get_edit_selection(), -1, 0 ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_move_selection_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply an offset of one unit on the right to the selection.
 * \param event The event.
 */
void bf::ingame_view_frame::on_move_selection_right
( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->do_action
    ( new action_move_selection( m_ingame_view->get_edit_selection(), 1, 0 ) );

  if ( ! m_ingame_view->get_active_layer().check_item_position() )
    m_ingame_view->show_item_position_error(this);
} // ingame_view_frame::on_move_selection_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the tool button "Align".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_align( wxCommandEvent& WXUNUSED(event) )
{
  PopupMenu( m_align_menu );
} // ingame_view_frame::on_align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the tool button "Arrange".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_arrange( wxCommandEvent& WXUNUSED(event) )
{
  PopupMenu( m_arrange_menu );
} // ingame_view_frame::on_arrange()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "Bright background".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_bright_background
( wxCommandEvent& WXUNUSED(event) )
{
  toggle_bright_background();
  GetToolBar()->ToggleTool
    (ID_BRIGHT_BACKGROUND, m_ingame_view->get_bright_background());
} // ingame_view_frame::on_bright_background()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_100".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_zoom_100( wxCommandEvent& WXUNUSED(event) )
{
  m_ingame_view->set_zoom(100);
  m_ingame_view->Refresh();
  update_toolbar();
} // ingame_view_frame::on_zoom_100()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_in".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_zoom_in( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_ingame_view->get_zoom() > 9 )
    m_ingame_view->set_zoom( m_ingame_view->get_zoom() + 10 );
  else
    m_ingame_view->set_zoom( m_ingame_view->get_zoom() + 1 );
  m_ingame_view->Refresh();
  update_toolbar();
} // ingame_view_frame::on_zoom_in()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on the menu "zoom_out".
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_zoom_out( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_ingame_view->get_zoom() > 1 )
    {
      if ( m_ingame_view->get_zoom() < 11 )
        m_ingame_view->set_zoom( m_ingame_view->get_zoom() - 1 );
      else
        m_ingame_view->set_zoom( m_ingame_view->get_zoom() - 10 );

      update_toolbar();
      m_ingame_view->Refresh();
    }
} // ingame_view_frame::on_zoom_out()

/*----------------------------------------------------------------------------*/
/**
 * \brief The zoom control has been changed.
 * \param event The event that occured.
 */
void bf::ingame_view_frame::on_zoom_change( wxSpinEvent& event )
{
  m_ingame_view->set_zoom( event.GetPosition() );
  update_toolbar();
  m_ingame_view->Refresh();
} // ingame_view_frame::on_zoom_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Choose the previous layer as the current layer.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_previous_layer
( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_ingame_view->get_level().layers_count() > 1 )
    {
      if ( m_ingame_view->get_active_index() == 0 )
        m_ingame_view->set_active_index
          ( m_ingame_view->get_level().layers_count() - 1 );
      else
        m_ingame_view->set_active_index
          ( m_ingame_view->get_active_index() - 1 );
    }
} // ingame_view_frame::on_previous_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Choose the next layer as the current layer.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_next_layer( wxCommandEvent& WXUNUSED(event) )
{
  if ( m_ingame_view->get_level().layers_count() > 1 )
    m_ingame_view->set_active_index
      ( (m_ingame_view->get_active_index() + 1)
        % m_ingame_view->get_level().layers_count() );
} // ingame_view_frame::on_next_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a decorative layer from a big image and generate the sprites.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_new_layer_from_image
( wxCommandEvent& WXUNUSED(event) )
{
  new_layer_from_image_dialog dlg
    ( *this, m_ingame_view->get_level().layers_count(),
      m_layout.get_item_class_pool() );

  if ( dlg.ShowModal() == wxID_OK )
    {
      level_action* result =
        new action_add_layer( dlg.drop_layer(), dlg.get_layer_index() );

      if ( dlg.layer_replaces_previous()
           && ( dlg.get_layer_index()
                < m_ingame_view->get_level().layers_count() ) )
        {
          action_group* g = new action_group( result->get_description() );
          g->add_action( new action_remove_layer( dlg.get_layer_index() ) );
          g->add_action( result );
          result = g;
        }

      m_ingame_view->do_action( result );
    }
} // ingame_view_frame::on_new_layer_from_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Opens the menu to select the edit mode.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_edit_mode( wxCommandEvent& event )
{
  PopupMenu( m_edit_mode_menu );
} // ingame_view_frame::on_edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a decorative layer from a big image and generate the sprites.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_edit_mode_active_layer( wxCommandEvent& event )
{
  edit_mode m( m_ingame_view->get_edit_mode() );
  m.set_value( edit_mode::active_layer );
  m_ingame_view->set_edit_mode( m );

  update_edit_mode();
} // ingame_view_frame::on_edit_mode_active_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a decorative layer from a big image and generate the sprites.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_edit_mode_same_tag( wxCommandEvent& event )
{
  edit_mode m( m_ingame_view->get_edit_mode() );
  m.set_value( edit_mode::layers_by_tag );
  m_ingame_view->set_edit_mode( m );

  update_edit_mode();
} // ingame_view_frame::on_edit_mode_same_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a decorative layer from a big image and generate the sprites.
 * \param event This event occured.
 */
void bf::ingame_view_frame::on_edit_mode_all( wxCommandEvent& event )
{
  edit_mode m( m_ingame_view->get_edit_mode() );
  m.set_value( edit_mode::all_layers );
  m_ingame_view->set_edit_mode( m );

  update_edit_mode();
} // ingame_view_frame::on_edit_mode_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief An item has been selected in the error frame.
 * \param event The event carrying the item.
 */
void bf::ingame_view_frame::on_error_selected( item_event& event )
{
  m_ingame_view->select_item_and_layer( event.get_item() );
} // ingame_view_frame::on_error_selected()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::ingame_view_frame, wxFrame)
  EVT_SIZE( bf::ingame_view_frame::on_size )
  EVT_PAINT( bf::ingame_view_frame::on_paint )
  EVT_ACTIVATE( bf::ingame_view_frame::on_activate )
  EVT_SCROLL( bf::ingame_view_frame::on_scroll )
  EVT_CLOSE( bf::ingame_view_frame::on_close )
  EVT_CONTEXT_MENU( bf::ingame_view_frame::on_context_menu )
  EVT_MENU( wxID_PROPERTIES, bf::ingame_view_frame::on_level_properties )
  EVT_MENU( ID_STATISTICS, bf::ingame_view_frame::on_level_statistics )
  EVT_MENU( wxID_UNDO, bf::ingame_view_frame::on_undo )
  EVT_MENU( wxID_REDO, bf::ingame_view_frame::on_redo )
  EVT_MENU( wxID_CUT, bf::ingame_view_frame::on_cut )
  EVT_MENU( wxID_COPY, bf::ingame_view_frame::on_copy )
  EVT_MENU( wxID_PASTE, bf::ingame_view_frame::on_paste )
  EVT_MENU( wxID_DELETE, bf::ingame_view_frame::on_delete )
  EVT_MENU( wxID_SELECTALL, bf::ingame_view_frame::on_select_all )
  EVT_MENU( ID_SELECT_NONE, bf::ingame_view_frame::on_select_none )
  EVT_MENU(  bf::ingame_view_frame::ID_CHECK_LEVEL,
       bf::ingame_view_frame::on_check_level )
  EVT_MENU(  wxID_SAVE, bf::ingame_view_frame::on_save )
  EVT_MENU(  bf::ingame_view_frame::ID_COMPILE_LEVEL,
             bf::ingame_view_frame::on_compile_level )
  EVT_MENU(  bf::ingame_view_frame::ID_QUICK_COMPILE_LEVEL,
             bf::ingame_view_frame::on_quick_compile_level )
  EVT_MENU(  bf::ingame_view_frame::ID_RUN_LEVEL,
             bf::ingame_view_frame::on_run_level )
  EVT_MENU( wxID_CLOSE, bf::ingame_view_frame::on_menu_close )
  EVT_MENU( bf::ingame_view_frame::ID_SHOW_GRID_PROPERTIES,
            bf::ingame_view_frame::on_show_grid_properties )
  EVT_MENU( bf::ingame_view_frame::ID_SET_GRID_ON_SELECTION,
            bf::ingame_view_frame::on_set_grid_on_selection )
  EVT_MENU( bf::ingame_view_frame::ID_GRID_MAGNETISM,
            bf::ingame_view_frame::on_grid_magnetism )
  EVT_MENU( bf::ingame_view_frame::ID_GRID_VISIBLE,
            bf::ingame_view_frame::on_grid_visible )
  EVT_MENU( bf::ingame_view_frame::ID_ID_VISIBLE,
            bf::ingame_view_frame::on_id_visible )
  EVT_MENU( bf::ingame_view_frame::ID_RELATIONSHIP,
            bf::ingame_view_frame::on_relationship )
  EVT_MENU( bf::ingame_view_frame::ID_WIREFRAME,
            bf::ingame_view_frame::on_wireframe )
  EVT_MENU( bf::ingame_view_frame::ID_GRAPHISM,
            bf::ingame_view_frame::on_graphic )
  EVT_MENU( bf::ingame_view_frame::ID_LEVEL_OVERVIEW,
            bf::ingame_view_frame::on_level_overview )
  EVT_MENU_HIGHLIGHT_ALL( bf::ingame_view_frame::on_menu_highlight )
  EVT_MENU_OPEN( bf::ingame_view_frame::on_open_menu )

  EVT_MENU( bf::ingame_view_frame::ID_ALIGN_LEFT,
            bf::ingame_view_frame::on_align_left )
  EVT_MENU( bf::ingame_view_frame::ID_ALIGN_RIGHT,
            bf::ingame_view_frame::on_align_right )
  EVT_MENU( bf::ingame_view_frame::ID_ALIGN_TOP,
            bf::ingame_view_frame::on_align_top )
  EVT_MENU( bf::ingame_view_frame::ID_ALIGN_BOTTOM,
            bf::ingame_view_frame::on_align_bottom )
  EVT_MENU( bf::ingame_view_frame::ID_ALIGN_CENTER_X,
            bf::ingame_view_frame::on_align_center_x )
  EVT_MENU( bf::ingame_view_frame::ID_ALIGN_CENTER_Y,
            bf::ingame_view_frame::on_align_center_y )

  EVT_MENU( bf::ingame_view_frame::ID_ARRANGE_X,
            bf::ingame_view_frame::on_arrange_horizontally )
  EVT_MENU( bf::ingame_view_frame::ID_ARRANGE_Y,
            bf::ingame_view_frame::on_arrange_vertically )

  EVT_MENU( bf::ingame_view_frame::ID_CLONE,
            bf::ingame_view_frame::on_clone )
  EVT_MENU( bf::ingame_view_frame::ID_OFFSET,
            bf::ingame_view_frame::on_offset )
  EVT_MENU( bf::ingame_view_frame::ID_MOVE_IN_LAYER_UP,
            bf::ingame_view_frame::on_move_in_layer_up )
  EVT_MENU( bf::ingame_view_frame::ID_MOVE_IN_LAYER_DOWN,
            bf::ingame_view_frame::on_move_in_layer_down )

  EVT_TOOL( bf::ingame_view_frame::ID_MOVE_SELECTION_UP,
            bf::ingame_view_frame::on_move_selection_up )
  EVT_TOOL( bf::ingame_view_frame::ID_MOVE_SELECTION_DOWN,
            bf::ingame_view_frame::on_move_selection_down )
  EVT_TOOL( bf::ingame_view_frame::ID_MOVE_SELECTION_LEFT,
            bf::ingame_view_frame::on_move_selection_left )
  EVT_TOOL( bf::ingame_view_frame::ID_MOVE_SELECTION_RIGHT,
            bf::ingame_view_frame::on_move_selection_right )

  EVT_TOOL( bf::ingame_view_frame::ID_ALIGN,
            bf::ingame_view_frame::on_align )

  EVT_TOOL( bf::ingame_view_frame::ID_ARRANGE,
            bf::ingame_view_frame::on_arrange )

  EVT_MENU( bf::ingame_view_frame::ID_BRIGHT_BACKGROUND,
            bf::ingame_view_frame::on_bright_background )
  EVT_MENU( wxID_ZOOM_100,
            bf::ingame_view_frame::on_zoom_100)
  EVT_MENU( wxID_ZOOM_IN,
            bf::ingame_view_frame::on_zoom_in)
  EVT_MENU( wxID_ZOOM_OUT,
            bf::ingame_view_frame::on_zoom_out)
  EVT_SPINCTRL( ID_ZOOM,
                bf::ingame_view_frame::on_zoom_change )

  EVT_TOOL( bf::ingame_view_frame::ID_PREVIOUS_LAYER,
            bf::ingame_view_frame::on_previous_layer )
  EVT_TOOL( bf::ingame_view_frame::ID_NEXT_LAYER,
            bf::ingame_view_frame::on_next_layer )

  EVT_MENU( bf::ingame_view_frame::ID_NEW_LAYER_FROM_IMAGE,
            bf::ingame_view_frame::on_new_layer_from_image )

  EVT_MENU( bf::ingame_view_frame::ID_EDIT_MODE,
            bf::ingame_view_frame::on_edit_mode )
  EVT_MENU( bf::ingame_view_frame::ID_EDIT_ACTIVE_LAYER,
            bf::ingame_view_frame::on_edit_mode_active_layer )
  EVT_MENU( bf::ingame_view_frame::ID_EDIT_SAME_TAG,
            bf::ingame_view_frame::on_edit_mode_same_tag )
  EVT_MENU( bf::ingame_view_frame::ID_EDIT_ALL,
            bf::ingame_view_frame::on_edit_mode_all )

  EVT_ITEM_SELECTION( bf::ingame_view_frame::ID_ERROR_FRAME,
                      bf::ingame_view_frame::on_error_selected)
END_EVENT_TABLE()
