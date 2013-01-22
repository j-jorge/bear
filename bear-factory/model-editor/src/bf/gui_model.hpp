/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A model in the graphical user interface.
 * \author Sébastien angibaud
 */
#ifndef __BF_GUI_MODEL_HPP__
#define __BF_GUI_MODEL_HPP__

#include "bf/model.hpp"
#include "bf/sprite.hpp"
#include "bf/model_selection.hpp"

#include <map>

namespace bf
{
  class action;
  class mark_placement;

  /**
   * \brief A model in the graphical user interface.
   * \author Sébastien Angibaud
   */
  class gui_model:
    public model
  {
  public:
    gui_model();

    bool has_active_action() const;
    void set_active_action( const std::string& action_name );
    action& get_active_action();
    const action& get_active_action() const;
    void reset_active_action();

    bool has_mark_selection() const;
    mark* get_main_mark_selection();
    const mark* get_main_mark_selection() const;
    void set_mark_selection( const std::string& label );
    void add_to_mark_selection
    ( const std::string& label, bool is_main_selection=false );

    bool bounding_box_is_selected() const;
    bool bounding_box_is_main_selection() const;
    void set_bounding_box_selection( bool s );

    void clear_selection();

    void set_date(double date);
    double get_date() const;

    bool has_selected_snapshot();
    snapshot* get_selected_snapshot();

    sprite get_current_sprite_for( const mark_placement& m ) const;

    action& add_action
    ( const std::string& action_name, double duration,
      const sound_description& sound,
      const std::string& next_action);
    void add_action( action* a );
    action* remove_action( const std::string& action_name );

    void add_mark( const std::string& action_name, mark* m);
    void remove_mark( const std::string& action_name, mark* m);

    void add_snapshot( const std::string& action_name, snapshot* m);
    void remove_snapshot( const std::string& action_name, snapshot* m);

  private:
    /** \brief Action on which we are working. */
    action* m_active_action;

    /** \brief Name of the main mark selection. */
    std::map<action*, model_selection> m_selection;

    /** \brief Current date. */
    double m_date;

  }; // class gui_model
} // namespace bf

#endif // __BF_GUI_MODEL_HPP__
