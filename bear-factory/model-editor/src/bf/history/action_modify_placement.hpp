/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of modify a mark placement.
 * \author Sébastien Angibaud
 */
#ifndef __BF_ACTION_MODIFY_PLACEMENT_HPP__
#define __BF_ACTION_MODIFY_PLACEMENT_HPP__

#include "bf/history/model_action.hpp"

#include <boost/function.hpp>

namespace bf
{
  class action;
  class snapshot;
  class mark;
  class mark_placement;

  /**
   * \brief The action of changing the property of a mark placement.
   * \author Julien Jorge
   */
  template<typename T>
  class action_modify_placement:
    public model_action
  {
  public:
    /** \brief The type of the function to call to set the property. */
    typedef boost::function<void (mark_placement*, T)> setter_type;

    /** \brief The type of the function to call to get the value of the
        property. */
    typedef boost::function<T (const mark_placement*)> getter_type;

  public:
    action_modify_placement
    ( snapshot* s, const mark* m, const setter_type& setter,
      const getter_type& getter, T value );

    /**
     * \remark Calling execute() two times will restore the initial state.
     */
    void execute( gui_model& mdl );
    void undo( gui_model& mdl );

    bool is_identity( const gui_model& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The snapshot in which the placement is modified. */
    snapshot* m_snapshot;

    /** \brief The mark whose placement is modified. */
    const mark* m_mark;

    /** \brief The function to call to set the value of the property. */
    setter_type m_setter;

    /** \brief The function to call to get the value of the property. */
    getter_type m_getter;

    /** \brief The value to assign to the property. */
    T m_value;

  }; // class action_modify_placement

  action_modify_placement<double>*
  make_action_modify_placement_x( snapshot* s, const mark* m, double value );

  action_modify_placement<double>*
  make_action_modify_placement_y( snapshot* s, const mark* m, double value );

  action_modify_placement<int>*
  make_action_modify_placement_depth( snapshot* s, const mark* m, int value );

  action_modify_placement<bool>*
  make_action_modify_placement_visibility
  ( snapshot* s, const mark* m, bool value );

  action_modify_placement<double>*
  make_action_modify_placement_angle
  ( snapshot* s, const mark* m, double value );

  action_modify_placement<double>*
  make_action_modify_placement_width
  ( snapshot* s, const mark* m, double value );

  action_modify_placement<double>*
  make_action_modify_placement_height
  ( snapshot* s, const mark* m, double value );

  /**
   * \brief This structure defines for a given value type T the type of a
   *        function that can be called to build an action_modify_placement<T>
   * \author Julien Jorge
   */
  template<typename T>
  struct action_modify_placement_maker
  {
    /** \brief The type of a function that can be called to build an
        action_modify_placement<T>. */
    typedef boost::function
    < action_modify_placement<T>* (snapshot*, const mark*, T) > maker_type;
  }; // struct action_modify_placement_maker

  template<typename T>
  model_action* make_action_modify_placement_globally
  ( action& a, const mark* m, T value,
    typename action_modify_placement_maker<T>::maker_type f );

} // namespace bf

#include "bf/history/impl/action_modify_placement.tpp"

#endif // __BF_ACTION_MODIFY_PLACEMENT_HPP__
