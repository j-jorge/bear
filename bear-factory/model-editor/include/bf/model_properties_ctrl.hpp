/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This window display the properties of the model currently edited.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_PROPERTIES_CTRL_HPP__
#define __BF_MODEL_PROPERTIES_CTRL_HPP__

#include <wx/notebook.h>

namespace bf
{
  class model;
  class model_frame;
  class fields_frame;
  class action_list_frame;
  class mark_list_frame;
  class snapshot_frame;
  class windows_layout;

  /**
   * \brief This window display the properties of the model currently edited.
   * \author Julien Jorge
   */
  class model_properties_ctrl:
    public wxNotebook
  {
  public:
    model_properties_ctrl( wxWindow* parent );

    void set_model_frame( model_frame* m );
    void update_action();
    void update_snapshot();
    void update_mark();

  private:
    void create_controls();

  private:
    /** \brief The frame that displays the actions of the current model. */
    action_list_frame* m_actions_frame;

    /** \brief The frame that displays the marks of the current action. */
    mark_list_frame* m_marks_frame;

    /** \brief The frame that displays the current snapshot. */
    snapshot_frame* m_snapshot_frame;

  }; // class model_properties_ctrl
} // namespace

#endif // __BF_MODEL_PROPERTIES_CTRL_HPP__
