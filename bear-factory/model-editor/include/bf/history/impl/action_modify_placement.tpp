/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_placement class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_modify_placement.hpp"

#include "bf/action.hpp"
#include "bf/snapshot.hpp"
#include "bf/mark_placement.hpp"

#include "bf/history/action_group.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The snapshot in which the mark placement is modified.
 * \param m The mark whose placement is modified.
 * \param setter The function to call to set the value of the property.
 * \param getter The function to call to get the value of the property.
 * \param value The value to assign to the property.
 */
template<typename T>
bf::action_modify_placement<T>::action_modify_placement
( snapshot* s, const mark* m, const setter_type& setter,
  const getter_type& getter, T value )
  : m_snapshot(s), m_mark(m), m_setter(setter), m_getter(getter), m_value(value)
{

} // action_modify_placement::action_modify_placement()

/*----------------------------------------------------------------------------*/
template<typename T>
void bf::action_modify_placement<T>::execute( gui_model& mdl )
{
  mark_placement& p( m_snapshot->get_placement( m_mark ) );

  T old_value( m_getter(&p) );
  m_setter( &p, m_value );
  m_value = old_value;
} // action_modify_placement::execute()

/*----------------------------------------------------------------------------*/
template<typename T>
void bf::action_modify_placement<T>::undo( gui_model& mdl )
{
  /* the first call to execute saved the initial label and animation. */
  execute(mdl);
} // action_modify_placement::undo()

/*----------------------------------------------------------------------------*/
template<typename T>
bool bf::action_modify_placement<T>::is_identity
( const gui_model& mdl ) const
{
  mark_placement& p( m_snapshot->get_placement( m_mark ) );

  return m_getter(&p) == m_value;
} // action_modify_placement::is_identity()

/*----------------------------------------------------------------------------*/
template<typename T>
wxString bf::action_modify_placement<T>::get_description() const
{
  return _("Modify a mark placement");
} // action_modify_placement::get_description()





/*----------------------------------------------------------------------------*/
/**
 * \brief Create an action that modifies a property on a mark placement on all
 *        the snapshots of an action.
 * \param a The action whose snapshots are modified.
 * \param m The mark whose placement is modified.
 * \param value The value to assign to the property of the placement.
 * \param f The function to call to modify the property on a placement of one
 *        snapshot.
 */
template<typename T>
bf::model_action* bf::make_action_modify_placement_globally
( action& a, const mark* m, T value,
  typename action_modify_placement_maker<T>::maker_type f )
{
  action_group* result = new action_group();

  for ( typename action::snapshot_iterator it=a.snapshot_begin();
        it!=a.snapshot_end(); ++it )
    {
      model_action* a = f( &*it, m, value );
      result->add_action( a );
      result->set_description( a->get_description() );
    }

  return result;
} // make_action_modify_placement_globally()
