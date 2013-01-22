/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief \brief This window displays the errors of a level.
 * \author Julien Jorge
 */
#ifndef __BF_ERROR_CHECK_LEVEL_DIALOG_HPP__
#define __BF_ERROR_CHECK_LEVEL_DIALOG_HPP__

#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>

#include <vector>

namespace bf
{
  class item_instance;
  class item_check_result;
  class layer;
  class layer_check_result;
  class level_check_result;

  /**
   * \brief This window displays the errors of a level.
   * \author Sebastien Angibaud
   */
  class error_check_level_dialog:
    public wxFrame
  {
  private:
      /** \brief The identifiers of the controls. */
      enum control_id
        {
          ID_ERROR_LIST
        }; // enum control_id

  public:
    error_check_level_dialog( wxWindow* parent, wxWindowID id = wxID_ANY );

    void set_errors( const level_check_result& errors );

  private:
    void fill_layer_check_result
    ( layer* lay, const layer_check_result& result );
    void fill_item_check_result
    ( item_instance* item, const item_check_result& result,
        const wxString& lay_name );

    void create_controls();
    void create_sizers();
    void adjust_last_column_size() const;

    void on_close(wxCloseEvent& event);
    void on_size(wxSizeEvent& event);
    void on_error_selected(wxListEvent& event);

  private:
    /** \brief The control that displays the list of bad items. */
    wxListView* m_items_list;

    /** \brief The area where the complete message is displayed. */
    wxTextCtrl* m_details;

    /** \brief The items associated with each line of the list. */
    std::vector<item_instance*> m_items;

    DECLARE_EVENT_TABLE()

  }; // class error_check_level_dialog
} // namespace bf

#endif // __BF_ERROR_CHECK_LEVEL_DIALOG_HPP__
