/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Some facilities for using the wxWidget library with other libraries
 *        (namely standard library).
 * \author Julien Jorge
 */
#ifndef __BF_WX_FACILITIES_HPP__
#define __BF_WX_FACILITIES_HPP__

#include <wx/bmpbuttn.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <string>

namespace bf
{
  /**
   * \brief Create a std::string from a wxString.
   * \param str The reference string.
   */
  inline std::string wx_to_std_string( const wxString& str )
  {
    return std::string( str.To8BitData() );
  } // wx_to_std_string()

  /**
   * \brief Create a std::string from a wxString.
   * \param str The reference string.
   */
  inline wxString std_to_wx_string( const std::string& str )
  {
    return wxString::From8BitData( str.c_str() );
  } // std_to_wx_string()

  /**
   * \brief Append a menu item with an icon.
   * \param menu The menu in which the item is added.
   * \param id The identifier of the menu entry.
   * \param label The string to appear on the menu item.
   * \param help An help string associated with the item.
   * \param icon An icon displayed next to the item.
   */
  inline void wx_menu_append_item
  ( wxMenu* menu, int id, const wxString& label, const wxString& help,
    const wxBitmap& icon = wxBitmap() )
  {
    wxMenuItem* item = new wxMenuItem(menu, id, label, help);
    item->SetBitmap(icon);
    menu->Append(item);
  } // wx_menu_append_item()

  /**
   * \brief Create a wxBitmapButton.
   * \param parent The owner of the button.
   * \param id The identifier of the button.
   * \param bmp The bitmap displayed in the button.
   * \param tip The message displayed in the tool tip.
   */
  inline wxBitmapButton* wx_new_bitmap_button
  ( wxWindow* parent, int id, const wxBitmap& bmp, const wxString& tip )
  {
    wxBitmapButton* result = new wxBitmapButton(parent, id, bmp);
    result->SetToolTip(tip);
    return result;
  } // wx_new_bitmap_button()

} // namespace bf

#endif // __BF_WX_FACILITIES_HPP__
