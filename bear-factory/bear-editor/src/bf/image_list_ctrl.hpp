/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A dialog to select one of the images in bf::image_pool.
 * \author Julien Jorge
 */
#ifndef __BF_IMAGE_LIST_CTRL_HPP__
#define __BF_IMAGE_LIST_CTRL_HPP__

#include "bf/libeditor_export.hpp"

#include <wx/panel.h>
#include <wx/scrolbar.h>

#include <list>

namespace bf
{
  /**
   * \brief A dialog to select one of the images in bf::image_pool.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT image_list_ctrl:
    public wxPanel
  {
  private:
    /** \brief The list view is just here to handle the mouse events. */
    class list_view:
      public wxWindow
    {
    public:
      list_view( image_list_ctrl& parent );

      void on_left_up( wxMouseEvent& event );

    private:
      /** \brief The parent class. */
      image_list_ctrl& m_parent;

      DECLARE_EVENT_TABLE()

    }; // class list_view

  public:
    image_list_ctrl( wxWindow& parent );

    void set_list( const std::list<wxString>& img );
    void set_selection( int i );
    void set_selection( const wxString& s );

    wxString get_selection() const;

  private:
    void create_controls();

    void set_scrollbar_values();

    void render();
    void render_list( wxDC& dc );
    void
    render_name( wxDC& dc, const wxString& name, wxPoint& pos, int i ) const;
    void render_thumb( wxDC& dc, wxBitmap bmp, wxPoint& pos, int i ) const;
    void
    render_border( wxDC& dc, const wxPoint& pos, const wxSize& s, int i ) const;

    void select_item( const wxPoint& pos );

    void on_paint( wxPaintEvent& event );
    void on_size( wxSizeEvent& event );
    void on_wheel( wxMouseEvent& event );

  private:
    /** \brief The list of images. */
    list_view* m_image_list;

    /** \brief The scrollbar. */
    wxScrollBar* m_bar;

    /** \brief The images displayed in the list. */
    std::list<wxString> m_image;

    /** \brief The index, in m_image, of the selected image. */
    int m_selection;

    /** \brief The margin between the thumnails. */
    static const wxSize s_margin;

    DECLARE_EVENT_TABLE()

  }; // class image_list_ctrl
} // namespace bf

#endif // __BF_IMAGE_LIST_CTRL_HPP__
