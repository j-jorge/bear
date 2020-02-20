/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::gui_model class.
 * \author Sébastien Angibaud
 */
#include "bf/gui_model.hpp"

#include "bf/action.hpp"
#include "bf/mark.hpp"
#include "bf/mark_placement.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::gui_model::gui_model()
  : m_active_action(NULL), m_date(0)
{

} // gui_model::gui_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an action active for edition.
 */
bool bf::gui_model::has_active_action() const
{
  return ( m_active_action != NULL );
} // gui_model::has_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the action to work on.
 * \param action_name The name of the action.
 */
void bf::gui_model::set_active_action( const std::string& action_name )
{
  CLAW_PRECOND( has_action(action_name) );

  std::set< std::string > mark_labels;
  bool bounding_box_selection = false;

  if ( has_active_action() )
    {
      model_selection::const_iterator it;
      for ( it = m_selection[m_active_action].begin(); 
            it != m_selection[m_active_action].end(); ++it )
        mark_labels.insert( (*it)->get_label() );

      bounding_box_selection = 
        m_selection[m_active_action].bounding_box_is_selected();
    }

  m_active_action = &get_action(action_name);
  clear_selection();

  if ( ! mark_labels.empty() )
    {
      std::set< std::string >::const_iterator it_label;

      for ( it_label = mark_labels.begin(); 
            it_label != mark_labels.end(); ++it_label )
        if ( m_active_action->has_mark_label( *it_label ) )
          add_to_mark_selection
            ( *it_label, m_selection[m_active_action].mark_empty() );
    }

  m_selection[m_active_action].set_bounding_box_selection
    ( bounding_box_selection );
} // gui_model::set_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the action on which we are working.
 */
bf::action& bf::gui_model::get_active_action()
{
  CLAW_PRECOND( m_active_action != NULL );

  return *m_active_action;
} // gui_model::get_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the action on which we are working.
 */
const bf::action& bf::gui_model::get_active_action() const
{
  CLAW_PRECOND(  m_active_action != NULL );

  return *m_active_action;
} // gui_model::get_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the active action.
 */
void bf::gui_model::reset_active_action()
{
  m_active_action = NULL;
} // gui_model::reset_active_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a mark selected.
 */
bool bf::gui_model::has_mark_selection() const
{
  if ( has_active_action() )
    return !m_selection.find(m_active_action)->second.mark_empty();
  else
    return false;
} // gui_model::has_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main selected mark.
 */
const bf::mark* bf::gui_model::get_main_mark_selection() const
{
  CLAW_PRECOND( has_active_action() );

  return m_selection.find(m_active_action)->second.get_main_mark_selection();
} // gui_model::get_main_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selected mark.
 */
bf::mark* bf::gui_model::get_main_mark_selection()
{
  CLAW_PRECOND( has_active_action() );

  return m_selection[m_active_action].get_main_mark_selection();
} // gui_model::get_main_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selected mark.
 * \param label The label of the new selected mark.
 */
void bf::gui_model::set_mark_selection( const std::string& label )
{
  CLAW_PRECOND( has_active_action() );
  CLAW_PRECOND( get_active_action().has_mark_label(label) );

  m_selection[m_active_action].clear();
  m_selection[m_active_action].insert( get_active_action().get_mark(label) );
} // gui_model::set_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a mark in the selection.
 * \param label The label of the new selected mark.
 * \param is_main_selection Tell if \a label becomes the new main selection.
 */
void bf::gui_model::add_to_mark_selection
( const std::string& label, bool is_main_selection )
{
  CLAW_PRECOND( has_active_action() );
  CLAW_PRECOND( get_active_action().has_mark_label(label) );

  m_selection[m_active_action].insert
    ( get_active_action().get_mark(label), is_main_selection );
} // gui_model::add_to_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bounding box is selected.
 */
bool bf::gui_model::bounding_box_is_selected() const
{
  if ( has_active_action() )
    return m_selection.find(m_active_action)->second.bounding_box_is_selected();
  else
    return false;
} // gui_model::bounding_box_is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bounding box is the main selection.
 */
bool bf::gui_model::bounding_box_is_main_selection() const
{
  if ( has_active_action() )
    return
      m_selection.find
      (m_active_action)->second.bounding_box_is_main_selection();
  else
    return false;
} // gui_model::bounding_box_is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select or deselect the bounding box.
 * \param s Tell if the bounding box has to be selected.
 */
void bf::gui_model::set_bounding_box_selection( bool s )
{
  if ( has_active_action() )
    m_selection[m_active_action].set_bounding_box_selection(s);
} // gui_model::set_bounding_box_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all selections on the active action.
 */
void bf::gui_model::clear_selection()
{
  if ( has_active_action() )
    m_selection[m_active_action].clear();
} // gui_model::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the date.
 * \param date Te new date.
 */
void bf::gui_model::set_date( double date )
{
  m_date = date;
} // gui_model::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the date.
 */
double bf::gui_model::get_date() const
{
  return m_date;
} // gui_model::get_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if there exist a selected snapshot.
 */
bool bf::gui_model::has_selected_snapshot()
{
  bool result = false;

  if ( m_active_action != NULL )
    result = ( m_active_action->has_snapshot_date(m_date) );

  return result;
} // gui_model::has_selected_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selected snapshot.
 */
bf::snapshot* bf::gui_model::get_selected_snapshot()
{
  snapshot* result = NULL;

  if ( m_active_action != NULL )
    if ( m_active_action->has_snapshot_date(m_date) )
      result = m_active_action->get_snapshot_before_or_at_date(m_date);

  return result;
} // gui_model::get_selected_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite displayed for a given mark at the current date.
 * \param m The mark placement.
 */
bf::sprite
bf::gui_model::get_current_sprite_for( const mark_placement& m ) const
{
  sprite result;
  mark const* const mk = m.get_mark();

  if ( mk != NULL )
    if ( mk->has_animation() )
      if ( !mk->get_animation_data().empty() )
        {
          result = mk->get_animation_data().get_frame(0).get_sprite();

          if ( mk->apply_angle_to_animation() )
            result.set_angle( result.get_angle() + m.get_angle() );
        }

  return result;
} // gui_model::get_current_sprite_for()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an action.
 * \param action_name The name of the new action.
 * \param duration The duration of the new action
 * \param sound The sound of the new action.
 * \param next_action The new next action.
 */
bf::action& bf::gui_model::add_action
( const std::string& action_name, double duration,
  const sound_description& sound,
  const std::string& next_action)
{
  return model::add_action
    ( action_name, duration, sound, next_action);
} // gui_model::add_action()

/*----------------------------------------------------------------------------*/
void bf::gui_model::add_action( action* a )
{
  CLAW_PRECOND( !has_action( a->get_name() ) );

  model::add_action( a );

  m_active_action = a;
  m_selection[m_active_action].clear();
} // gui_model::add_action()

/*----------------------------------------------------------------------------*/
bf::action* bf::gui_model::remove_action( const std::string& action_name )
{
  CLAW_PRECOND( has_action( action_name ) );

  action* result = model::remove_action( action_name );

  if ( m_active_action == result )
    reset_active_action();

  m_selection.erase(result);

  return result;
} // gui_model::remove_action()

/*----------------------------------------------------------------------------*/
void bf::gui_model::add_mark(const std::string& action_name, mark* m)
{
  CLAW_PRECOND( has_action( action_name ) );
  CLAW_PRECOND( !get_action( action_name ).has_mark(m) );

  get_action(action_name).add_mark(m);
} // gui_model::add_mark()

/*----------------------------------------------------------------------------*/
void bf::gui_model::remove_mark(const std::string& action_name, mark* m)
{
  CLAW_PRECOND( has_action( action_name ) );
  CLAW_PRECOND( get_action( action_name ).has_mark(m) );

  action& a = get_action(action_name);

  a.remove_mark(m);
  m_selection[&a].remove(m);
} // gui_model::add_mark()

/*----------------------------------------------------------------------------*/
void bf::gui_model::add_snapshot(const std::string& action_name, snapshot* m)
{
  CLAW_PRECOND( has_action( action_name ) );
  CLAW_PRECOND( !get_action( action_name ).has_snapshot(m) );

  get_action(action_name).add_snapshot(m);
} // gui_model::add_snapshot()

/*----------------------------------------------------------------------------*/
void bf::gui_model::remove_snapshot(const std::string& action_name, snapshot* m)
{
  CLAW_PRECOND( has_action( action_name ) );
  CLAW_PRECOND( get_action( action_name ).has_snapshot(m) );

  get_action(action_name).remove_snapshot(m);
} // gui_model::add_snapshot()
