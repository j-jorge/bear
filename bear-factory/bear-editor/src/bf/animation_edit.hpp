/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The window showing the properties of a animation.
 * \author Sebastien Angibaud
 */
#ifndef __BF_ANIMATION_EDIT_HPP__
#define __BF_ANIMATION_EDIT_HPP__

#include "bf/animation.hpp"
#include "bf/base_edit.hpp"
#include "bf/default_value.hpp"
#include "bf/spin_ctrl.hpp"

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/listctrl.h>

namespace bf
{
  class animation_view_ctrl;
  class bitmap_rendering_attributes_edit;
  class image_pool;

  /**
   * \brief The window showing the properties of a animation.
   * \author Sebastien Angibaud
   */
  class animation_edit:
    public wxPanel,
    public base_edit<animation>
  {
  public:
    /** \brief The identifiers of the controls. */
    enum control_id
      {
        IDC_BOUND_INDEX_CHANGE,
        IDC_FRAME,
        IDC_REVERSE
      }; // enum control_id

  public:
    animation_edit
    ( wxWindow& parent, const image_pool& pool,
      const animation& anim = default_value<animation>::get() );

    bool validate();

  private:
    animation make_animation() const;

    void value_updated();
    void update_frame_list(const animation& anim) const;
    void update_buttons() const;
    void update_spin_ctrl() const;
    void update_duration() const;
    void adjust_last_column_size();
    void fill_controls() const;

    void create_controls();
    void create_sizer_controls();
    wxSizer* create_loops_sizer();

    void edit_frame( long index );

    void on_up( wxCommandEvent& event );
    void on_down( wxCommandEvent& event );
    void on_new( wxCommandEvent& event );
    void on_copy( wxCommandEvent& event );
    void on_frame_edit(wxListEvent& event);
    void on_frame_selected(wxListEvent& event);
    void on_frame_deselected(wxListEvent& event);
    void on_frame_button_edit( wxCommandEvent& event );
    void on_delete( wxCommandEvent& event );
    void on_reverse( wxCommandEvent& event );
    void on_size(wxSizeEvent& event);
    void on_column_begin_drag(wxListEvent& event);
    void on_column_end_drag(wxListEvent& event);
    void on_bound_index_change( wxSpinEvent& event );
    void on_refresh(wxCommandEvent& event);

  private:
    /** \brief The text control in which we configure the number of loops. */
    bf::spin_ctrl<unsigned int>* m_loops_spin;

    /** \brief The text control in which we configure the index of first frame
     * for loops. */
    wxSpinCtrl* m_first_index_spin;

    /** \brief The text control in which we configure the index of last frame
     * for loops. */
    wxSpinCtrl* m_last_index_spin;

    /** \brief The text control in which we configure the loop_back option. */
    wxCheckBox* m_loop_back_box;

    /** \brief The control in which we configure the rendering attributes. */
    bitmap_rendering_attributes_edit* m_rendering_attributes;

    /** \brief The control that displays the list of frame. */
    wxListView* m_frame_list;

    /** \brief The button to copy a frame. */
    wxButton* m_copy_button;

    /** \brief The button to edit a frame. */
    wxButton* m_edit_button;

    /** \brief The button to move up a frame. */
    wxButton* m_up_button;

    /** \brief The button to move_down a frame. */
    wxButton* m_down_button;

    /** \brief The button to delete a frame. */
    wxButton* m_delete_button;

    /** \brief The button to reverse frames. */
    wxButton* m_reverse_button;

    /** \brief The label in which the duration of the animation is displayed. */
    wxStaticText* m_duration_label;

    /** \brief The control in which we display the animation. */
    animation_view_ctrl* m_animation_view;

    /** \brief The image pool to use. */
    const image_pool& m_image_pool;

    DECLARE_EVENT_TABLE()

  }; // class animation_edit
} // namespace bf

#endif // __BF_ANIMATION_EDIT_HPP__
