/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A dialog to create a layer from an image and generate the sprites.
 * \author Julien Jorge
 */
#ifndef __BF_NEW_LAYER_FROM_IMAGE_DIALOG_HPP__
#define __BF_NEW_LAYER_FROM_IMAGE_DIALOG_HPP__

#include "bf/item_class_pool.hpp"
#include "bf/spin_ctrl.hpp"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>

namespace bf
{
  class layer;

  /**
   * \brief A dialog to create a layer from an image and generate the sprites.
   * \author Julien Jorge
   */
  class new_layer_from_image_dialog:
    public wxDialog
  {
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        ID_BROWSE,
        ID_ITEM_CLASS
      }; // enum control_id

  public:
    new_layer_from_image_dialog
    ( wxWindow& parent, std::size_t max_index, const item_class_pool& pool );
    ~new_layer_from_image_dialog();

    bool layer_replaces_previous() const;
    std::size_t get_layer_index() const;
    layer* drop_layer();

  private:
    void create_controls( std::size_t max_index );
    void create_sizers();

    wxSizer* create_source_sizer();
    wxSizer* create_layer_sizer();
    wxSizer* create_item_sizer();
    wxSizer* create_result_sizer();

    void generate() const;

    void update_field_list();
    void fill_fields_from_class( const item_class& c );

    void on_item_class( wxCommandEvent& event );
    void on_browse_source( wxCommandEvent& event );

  private:
    /** \brief The layer created from the image. */
    layer* m_layer;

    /** \brief The pool of item classes in which the user can pick the class
        that will be used to store the sprites. */
    const item_class_pool& m_item_class_pool;

    /** \brief The path to the image from which the sprites are created. */
    wxTextCtrl* m_image_path;

    /** \brief The index of the layer. */
    wxSpinCtrl* m_layer_index;

    /** \brief Tell if the layer replaces the previous one at the given
        index. */
    wxCheckBox* m_replace_previous;

    /** \brief The directory where the images are saved. */
    wxComboBox* m_output_dir;

    /** \brief The prefix of the images created. */
    wxTextCtrl* m_prefix;

    /** \brief The button used to pick the item class. */
    wxButton* m_item_class;

    /** \brief The name of the field to which the sprite is assigned. */
    wxChoice* m_field_name;

    /** \brief The Maximum size of one side of the result images. */
    wxSpinCtrl* m_max_size;

    DECLARE_EVENT_TABLE()

  }; // class new_layer_from_image_dialog
} // namespace bf

#endif // __BF_NEW_LAYER_FROM_IMAGE_DIALOG_HPP__
