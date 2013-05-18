/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Informations on a drag local to the view.
 * \author Julien Jorge
 */
#ifndef __BF_DRAG_INFO_HPP__
#define __BF_DRAG_INFO_HPP__

#include <wx/gdicmn.h>

namespace bf
{
  class item_instance;

  /**
   * \brief Informations on a drag local to the view.
   * \author Julien Jorge
   */
  class drag_info
  {
  public:
    enum drag_mode_type
      {
        drag_mode_none,
        drag_mode_selection,
        drag_mode_move,
        drag_mode_size,
        drag_mode_pick,
        drag_mode_slope          
      }; // enum drag_mode

  public:
    drag_info();

    wxSize delta() const;

  public:
    /** \brief The current mode of the drag. */
    drag_mode_type drag_mode;

    /** \brief The item picked at the begining of the drag. */
    item_instance* picked_item;

    /** \brief Origin of the drag. */
    wxPoint mouse_origin;

    /** \brief Position of the mouse during the drag. */
    wxPoint mouse_position;

    /** \brief Tell if the drag concerns the X-axis. */
    bool x_active;
    
    /** \brief Tell if the drag concerns the Y-axis. */
    bool y_active;

    /** \brief Tell if the drag concerns the left side. */
    bool left_side;

  }; // struct drag_info
} // namespace bf

#endif // __BF_DRAG_INFO_HPP__
