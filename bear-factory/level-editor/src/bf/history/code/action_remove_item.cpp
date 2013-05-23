/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_remove_item class.
 * \author Julien Jorge
 */
#include "bf/history/action_remove_item.hpp"

#include "bf/history/action_deprioritize.hpp"
#include "bf/history/action_set_item_field.hpp"
#include "bf/item_class.hpp"
#include "bf/gui_level.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to remove.
 * \param lay The layer from which the item is removed.
 */
bf::action_remove_item::action_remove_item
( item_instance* item, unsigned int lay )
  : m_item(item), m_layer(lay), m_done(false), m_pre_actions(NULL),
    m_post_actions(NULL)
{

} // action_remove_item::action_remove_item()

/*----------------------------------------------------------------------------*/
bf::action_remove_item::~action_remove_item()
{
  /* If the action has been done, the item must be deleted here. Otherwise, it
     will be deleted by the level. */
  if ( m_done )
    delete m_item;

  delete m_pre_actions;
  delete m_post_actions;
} // action_remove_item::~action_remove_item()

/*----------------------------------------------------------------------------*/
void bf::action_remove_item::execute( gui_level& lvl )
{
  CLAW_PRECOND( !m_done );
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  create_related_actions(lvl);

  m_selected = lvl.item_is_selected( m_item );
  m_main_selection = lvl.item_is_main_selection( m_item );

  m_pre_actions->execute(lvl);
  lvl.remove_item( m_layer, m_item );
  m_post_actions->execute(lvl);

  m_done = true;
} // action_remove_item::execute()

/*----------------------------------------------------------------------------*/
void bf::action_remove_item::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_done );
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  m_post_actions->undo(lvl);
  lvl.get_layer( m_layer ).add_item( m_item );
  m_pre_actions->undo(lvl);

  if ( m_selected )
    lvl.add_to_selection( m_item, m_main_selection );

  m_done = false;
} // action_remove_item::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_remove_item::is_identity( const gui_level& lvl ) const
{
  return false;
} // action_remove_item::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_remove_item::get_description() const
{
  return wxString::Format
    ( _("Remove %s"),
      std_to_wx_string(m_item->get_class().get_class_name()).c_str() );
} // action_remove_item::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill m_post_actions with actions consecutive to the removal of
 *        m_item.
 * \param lvl The level in which the action is done.
 */
void bf::action_remove_item::create_related_actions( gui_level& lvl )
{
  if ( (m_pre_actions != NULL) && (m_post_actions != NULL) )
    return;

  CLAW_PRECOND( m_pre_actions == NULL );
  CLAW_PRECOND( m_post_actions == NULL );

  m_pre_actions = new action_group;
  m_post_actions = new action_group;

  if ( !m_item->get_id().empty() )
    {
      layer::item_iterator it;
      const layer::item_iterator eit(lvl.get_layer(m_layer).item_end());

      for ( it=lvl.get_layer(m_layer).item_begin(); it!=eit; ++it )
        create_related_actions_with(&*it);
    }

  if ( lvl.get_layer(m_layer).is_prioritized(m_item) )
    m_pre_actions->add_action
      ( new action_deprioritize(m_item, m_layer) );
} // action_remove_item::create_related_actions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill m_post_actions with actions consecutive to the removal of
 *        m_item.
 * \param item The item in which the actions are done (not m_item).
 */
void bf::action_remove_item::create_related_actions_with( item_instance* item )
{
  create_related_actions_with(item, item->get_class());
} // action_remove_item::create_related_actions_with()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create actions to remove all references to m_item in an other item.
 * \param item The item in which the fields are removed (not m_item).
 * \param c The parent class of \a item in which we search the modified fields.
 */
void bf::action_remove_item::create_related_actions_with
  ( item_instance* item, const item_class& c )
{
  item_class::field_iterator itf;
  for (itf=c.field_begin(); itf!=c.field_end(); ++itf)
    if ( itf->get_field_type() == type_field::item_reference_field_type )
      {
        if ( itf->is_list() )
          create_action_set_item_field_list_for(item, *itf);
        else
          create_action_set_item_field_for(item, *itf);
      }

  item_class::const_super_class_iterator it;
  for ( it=c.super_class_begin(); it!=c.super_class_end(); ++it )
    create_related_actions_with(item, *it);
} // action_remove_item::create_related_actions_with()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an action to remove a reference to m_item in a given field of
 *        an other item.
 * \param item The item in which the actions are done (not m_item).
 * \param f The field in which the reference is removed.
 */
void bf::action_remove_item::create_action_set_item_field_for
( item_instance* item, const type_field& f )
{
  CLAW_PRECOND( !m_item->get_id().empty() );
  CLAW_PRECOND( !f.is_list() );

  if ( item->has_value(f) )
    {
      item_reference_type val;
      item->get_value(f, val);

      if ( val.get_value() == m_item->get_id() )
        m_post_actions->add_action
          ( new action_set_item_field<item_reference_type>
            (item, f.get_name()) );
    }
} // action_remove_item::create_action_set_item_field_for()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an action to remove a reference to m_item in a given field of
 *        an other item.
 * \param item The item in which the actions are done (not m_item).
 * \param f The field in which the reference is removed.
 */
void bf::action_remove_item::create_action_set_item_field_list_for
( item_instance* item, const type_field& f )
{
  CLAW_PRECOND( !m_item->get_id().empty() );
  CLAW_PRECOND( f.is_list() );

  if ( item->has_value(f) )
    {
      std::list<item_reference_type> val;
      item->get_value(f, val);

      if ( !val.empty() )
        {
          std::list<item_reference_type>::iterator it;

          for ( it=val.begin(); it!=val.end(); )
            if ( it->get_value() == m_item->get_id() )
              {
                std::list<item_reference_type>::iterator tmp(it);
                ++it;
                val.erase(tmp);
              }
            else
              ++it;

          if ( val.empty() )
            m_post_actions->add_action
              ( new action_set_item_field< std::list<item_reference_type> >
                (item, f.get_name()) );
          else
            m_post_actions->add_action
              ( new action_set_item_field< std::list<item_reference_type> >
                (item, f.get_name(), val) );
        }
    }
} // action_remove_item::create_action_set_item_field_list_for()
