/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class represents a grid.
 * \author Sebastien Angibaud
 */
#ifndef __BF_GRID_HPP__
#define __BF_GRID_HPP__

#include <wx/wx.h>
#include <wx/dnd.h>

namespace bf
{
  /** \brief A grid. */
  class grid
  {
  public:
    grid();

    void set_offset( const wxSize& offset);
    void set_step( const wxSize& step);
    void set_magnetism_active(bool value);
    void set_magnetism_force(unsigned int force);

    const wxSize& get_offset() const;
    const wxSize& get_step() const;
    bool get_magnetism_active() const;
    unsigned int get_magnetism_force() const;

  private:
    /** \brief The offset. */
    wxSize m_offset;

    /** \brief The step of the grid. */
    wxSize m_step;

    /** \brief Indicates if the magnetism is activate. **/
    bool m_magnetism_active;

    /** \brief Indicates if the magnetism force. **/
    unsigned int m_magnetism_force;

    /** \brief The default x-coordinate of the step. */
    static const unsigned int s_step_x_default;

    /** \brief The The default x-coordinate of the step. */
    static const unsigned int s_step_y_default;

    /** \brief The default magnetism force. */
    static const unsigned int s_magnetism_force_default;

  }; // class grid

} // namespace bf

#endif // __BF_GRID_HPP__

