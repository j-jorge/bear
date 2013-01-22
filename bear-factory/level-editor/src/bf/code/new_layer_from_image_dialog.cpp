/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::new_layer_from_image_dialog class.
 * \author Julien Jorge
 */
#include "bf/new_layer_from_image_dialog.hpp"

#include "bf/layer.hpp"
#include "bf/item_class_selection_dialog.hpp"
#include "bf/path_configuration.hpp"

#include <wx/filename.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The parent window.
 * \param max_index The maximum index at which the layer can be inserted.
 * \param pool The pool of item classes in which the user can pick the class
 *        that will be used to store the sprites.
 */
bf::new_layer_from_image_dialog::new_layer_from_image_dialog
( wxWindow& parent, std::size_t max_index, const item_class_pool& pool )
  : wxDialog( &parent, wxID_ANY, wxString(_("Create layer from image")) ),
    m_layer( new layer("decorative_layer") ), m_item_class_pool(pool)
{
  create_controls( max_index );
  create_sizers();

  Fit();
} // new_layer_from_image_dialog::new_layer_from_image_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::new_layer_from_image_dialog::~new_layer_from_image_dialog()
{
  delete m_layer;
} // new_layer_from_image_dialog::~new_layer_from_image_dialog()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the new layer replaces the previous one at the given index.
 */
bool bf::new_layer_from_image_dialog::layer_replaces_previous() const
{
  return m_replace_previous->GetValue();
} // new_layer_from_image_dialog::layer_replaces_previous()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index at which the new layer will be inserted.
 */
std::size_t bf::new_layer_from_image_dialog::get_layer_index() const
{
  return m_layer_index->GetValue();
} // new_layer_from_image_dialog::get_layer_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the newly created layer.
 */
bf::layer* bf::new_layer_from_image_dialog::drop_layer()
{
  CLAW_PRECOND( m_layer != NULL );

  layer* result = m_layer;
  m_layer = NULL;
  return result;
} // new_layer_from_image_dialog::drop_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls.
 * \param max_index The maximum index at which the layer can be inserted.
 */
void bf::new_layer_from_image_dialog::create_controls( std::size_t max_index )
{
  m_image_path = new wxTextCtrl(this, wxID_ANY);

  m_layer_index = new wxSpinCtrl(this);
  m_layer_index->SetRange(0, max_index);
  m_layer_index->SetValue(0);

  m_replace_previous =
    new wxCheckBox
    ( this, wxID_ANY, _("Replace previous layer at this index.") );

  wxArrayString c;
  std::list<std::string>::const_iterator it;

  for ( it=path_configuration::get_instance().data_path.begin();
        it!=path_configuration::get_instance().data_path.end(); ++it )
    c.Add( std_to_wx_string(*it) );

  m_output_dir =
    new wxComboBox
    ( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, c );
  m_output_dir->SetSelection(0);

  m_prefix = new wxTextCtrl( this, wxID_ANY, wxT("bk_") );

  m_item_class =
    new wxButton( this, ID_ITEM_CLASS, _("Click to select the class") );
  m_field_name = new wxChoice( this, wxID_ANY );

  m_max_size = new wxSpinCtrl(this);
  m_max_size->SetRange(0, 1024);
  m_max_size->SetValue(512);
} // new_layer_from_image_dialog::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers for the controls.
 */
void bf::new_layer_from_image_dialog::create_sizers()
{
  wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

  sizer->Add( create_source_sizer(), 0, wxEXPAND );
  sizer->Add( create_layer_sizer(), 0, wxEXPAND );
  sizer->Add( create_item_sizer(), 0, wxEXPAND );
  sizer->Add( create_result_sizer(), 0, wxEXPAND );

  sizer->Add
    ( CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxCENTER, 5 );
  SetSizer(sizer);
} // new_layer_from_image_dialog::create_sizers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers for the controls of the source image.
 */
wxSizer* bf::new_layer_from_image_dialog::create_source_sizer()
{
  wxSizer* sizer =
    new wxStaticBoxSizer( wxHORIZONTAL, this, _("Source image") );

  sizer->Add( m_image_path, 1, wxALL | wxEXPAND, 5 );
  sizer->Add( new wxButton( this, ID_BROWSE, wxT("…") ), 0, wxALL, 5 );

  return sizer;
} // new_layer_from_image_dialog::create_source_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers for the controls of the destination layer.
 */
wxSizer* bf::new_layer_from_image_dialog::create_layer_sizer()
{
  wxSizer* sizer = new wxStaticBoxSizer( wxHORIZONTAL, this, _("Layer") );
  sizer->Add
    ( new wxStaticText( this, wxID_ANY, _("Index of the new layer:") ),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
  sizer->Add( m_layer_index, 0, wxALL | wxEXPAND, 5 );
  sizer->Add( m_replace_previous, 01, wxALL | wxALIGN_CENTER_VERTICAL, 5 );

  return sizer;
} // new_layer_from_image_dialog::create_source_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers for the controls of the item class used to display
 *        the sprite.
 */
wxSizer* bf::new_layer_from_image_dialog::create_item_sizer()
{
  wxSizer* sizer = new wxStaticBoxSizer( wxVERTICAL, this, _("Item class") );
  wxSizer* s_sizer = new wxBoxSizer( wxHORIZONTAL );

  s_sizer->Add
    ( new wxStaticText
      ( this, wxID_ANY, _("The class of the items containing the sprites:") ),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
  s_sizer->Add( m_item_class, 1, wxALL | wxEXPAND, 5 );
  sizer->Add( s_sizer, 1, wxEXPAND );

  s_sizer = new wxBoxSizer( wxHORIZONTAL );
  s_sizer->Add
    ( new wxStaticText
      ( this, wxID_ANY, _("The field in which the sprite is set:") ),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
  s_sizer->Add( m_field_name, 1, wxALL | wxEXPAND, 5 );
  sizer->Add( s_sizer, 1, wxEXPAND );

  return sizer;
} // new_layer_from_image_dialog::create_item_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the sizers for the controls of the source image.
 */
wxSizer* bf::new_layer_from_image_dialog::create_result_sizer()
{
  wxSizer* sizer = new wxStaticBoxSizer( wxVERTICAL, this, _("Result") );
  wxSizer* t_sizer = new wxBoxSizer( wxHORIZONTAL );
  t_sizer->Add
    ( new wxStaticText( this, wxID_ANY, _("Output directory:") ),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
  t_sizer->Add( m_output_dir, 1, wxALL | wxEXPAND, 5 );
  sizer->Add( t_sizer, 1, wxEXPAND );

  t_sizer = new wxBoxSizer( wxHORIZONTAL );
  t_sizer->Add
    ( new wxStaticText( this, wxID_ANY, _("File prefix:") ),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
  t_sizer->Add( m_prefix, 1, wxALL | wxEXPAND, 5 );
  t_sizer->Add
    ( new wxStaticText( this, wxID_ANY, _("Maximum side size:") ),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
  t_sizer->Add( m_max_size, 1, wxALL | wxEXPAND, 5 );
  sizer->Add( t_sizer, 1, wxEXPAND );

  return sizer;
} // new_layer_from_image_dialog::create_result_sizer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generate the images and the layer.
 */
void bf::new_layer_from_image_dialog::generate() const
{
#if 0
  wxImage source( m_image_path );
  std::vector<unsigned int> x_sizes, y_sizes;

  compute_sizes( source.GetWidth(), x_sizes );
  compute_sizes( source.GetHeight(), y_sizes );

  generate_images_and_layer( source, x_sizes, y_sizes );
#endif
} // new_layer_from_image_dialog::generate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the list of fields that can be used to set the sprites.
 */
void bf::new_layer_from_image_dialog::update_field_list()
{
  const item_class& c =
    m_item_class_pool.get_item_class
    ( wx_to_std_string(m_item_class->GetLabel()) );
  const wxString old_field = m_field_name->GetStringSelection();
  m_field_name->Clear();

  fill_fields_from_class(c);

  m_field_name->SetSelection( m_field_name->FindString(old_field) );
} // new_layer_from_image_dialog::on_item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the fields that can be used for the sprites in a given class.
 * \param c The class in which we search for the fields.
 */
void
bf::new_layer_from_image_dialog::fill_fields_from_class( const item_class& c )
{
  item_class::field_iterator it;

  for ( it=c.field_begin(); it!=c.field_end(); ++it )
    if ( (it->get_field_type() == type_field::sprite_field_type)
         || (it->get_field_type() == type_field::animation_field_type) )
      m_field_name->Append( std_to_wx_string(it->get_name()) );

  item_class::const_super_class_iterator it_c;

  for ( it_c=c.super_class_begin(); it_c!=c.super_class_end(); ++it_c )
    fill_fields_from_class(*it_c);
} // new_layer_from_image_dialog::fill_fields_from_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user wants to pick the class of the items used to display the
 *        sprites.
 * \param event The event triggered by the user.
 */
void bf::new_layer_from_image_dialog::on_item_class
( wxCommandEvent& WXUNUSED(event) )
{
  item_class_selection_dialog dlg
    (m_item_class_pool, this, wx_to_std_string(m_item_class->GetLabel()));

  if ( dlg.ShowModal() == wxID_OK )
    {
      m_item_class->SetLabel( std_to_wx_string( dlg.get_class_name() ) );
      update_field_list();
    }
} // new_layer_from_image_dialog::on_item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user asked to select the source image.
 * \param event The event triggered by the user.
 */
void bf::new_layer_from_image_dialog::on_browse_source
( wxCommandEvent& WXUNUSED(event) )
{
  const wxFileName path( m_image_path->GetValue() );
  wxFileDialog dlg
    ( this, _("Choose an image"), path.GetPath(), wxEmptyString,
      _("JPEG images|*.jpg|PNG images|*.png"),
      wxFD_OPEN | wxFD_FILE_MUST_EXIST );

  if ( dlg.ShowModal() == wxID_OK )
    m_image_path->SetValue( dlg.GetPath() );
} // new_layer_from_image_dialog::on_browse_source()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::new_layer_from_image_dialog, wxDialog)
  EVT_BUTTON( bf::new_layer_from_image_dialog::ID_ITEM_CLASS,
              bf::new_layer_from_image_dialog::on_item_class )
  EVT_BUTTON( bf::new_layer_from_image_dialog::ID_BROWSE,
              bf::new_layer_from_image_dialog::on_browse_source )
END_EVENT_TABLE()
