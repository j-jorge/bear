/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The window showing the properties of a layer.
 * \author Julien Jorge
 */
#ifndef __BF_LAYER_PROPERTIES_FRAME_HPP__
#define __BF_LAYER_PROPERTIES_FRAME_HPP__

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <string>

namespace bf
{
  class layer;

  /**
   * \brief The layer_properties window of our program.
   * \author Julien Jorge
   */
  class layer_properties_frame:
    public wxDialog
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_FIT_LEVEL_SIZE
      }; // enum control_id

  public:
    layer_properties_frame( wxWindow* parent );

    bool get_layer_fits_level() const;
    unsigned int get_layer_width() const;
    unsigned int get_layer_height() const;
    std::string get_layer_class_name() const;
    std::string get_layer_name() const;
    std::string get_tag() const;

    void fill_from( const layer& lay );

  private:
    void create_controls();
    void create_member_controls();
    void create_sizer_controls();

    void on_ok( wxCommandEvent& event );
    void on_fit_level_size( wxCommandEvent& event );

  private:
    /** \brief Tell if the layer size is the same than the level size. */
    wxCheckBox* m_fit_level;

    /** \brief The text control in which we configure the width of the layer. */
    wxSpinCtrl* m_width;

    /** \brief The text control in which we configure the height of the
        layer. */
    wxSpinCtrl* m_height;

    /** \brief The text control in which we configure the name of the class of
        the layer. */
    wxChoice* m_class_name;

    /** \brief The text control in which we configure the name of the layer. */
    wxTextCtrl* m_name;

    /** \brief The text control in which we configure the tag of the layer. */
    wxTextCtrl* m_tag;

    /** \brief Minimum width of the layer. */
    static const unsigned int s_min_width;

    /** \brief Minimum height of the layer. */
    static const unsigned int s_min_height;

    DECLARE_EVENT_TABLE()

  }; // class layer_properties_frame
} // namespace bf

#endif // __BF_LAYER_PROPERTIES_FRAME_HPP__
