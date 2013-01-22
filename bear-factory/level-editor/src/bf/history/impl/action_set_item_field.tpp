/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_field class.
 * \author Julien Jorge
 */

#include "bf/human_readable.hpp"
#include "bf/item_class.hpp"
#include "bf/item_instance.hpp"

#include "bf/wx_facilities.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
template<typename ValueType>
bf::action_set_item_field<ValueType>::action_set_item_field
( item_instance* item, const std::string& name )
  : m_item(item), m_name(name), m_has_value(false)
{
  CLAW_PRECOND( item!=NULL );
} // action_set_item_field::action_set_item_field()

/*----------------------------------------------------------------------------*/
template<typename ValueType>
bf::action_set_item_field<ValueType>::action_set_item_field
( item_instance* item, const std::string& name, const ValueType& val )
  : m_item(item), m_name(name), m_value(val), m_has_value(true)
{
  CLAW_PRECOND( item!=NULL );
} // action_set_item_field::action_set_item_field()

/*----------------------------------------------------------------------------*/
template<typename ValueType>
void bf::action_set_item_field<ValueType>::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  const type_field& f( m_item->get_class().get_field(m_name) );
  ValueType old_value;
  const bool has_old_value( m_item->has_value(f) );

  if ( has_old_value )
    m_item->get_value( m_name, old_value );

  if ( m_has_value )
    {
      if ( wx_to_std_string( human_readable<ValueType>::convert(m_value) )
           != m_item->get_class().get_default_value(m_name) )
        m_item->set_value( m_name, m_value );
      else
        m_item->delete_value(f);
    }
  else
    m_item->delete_value(f);

  m_value = old_value;
  m_has_value = has_old_value;
} // action_set_item_field::execute()

/*----------------------------------------------------------------------------*/
template<typename ValueType>
void bf::action_set_item_field<ValueType>::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  /* the first call to execute() saved the initial value. */
  execute(lvl);
} // action_set_item_field::undo()

/*----------------------------------------------------------------------------*/
template<typename ValueType>
bool
bf::action_set_item_field<ValueType>::is_identity( const gui_level& gui ) const
{
  CLAW_PRECOND( m_item!=NULL );

  const type_field& f( m_item->get_class().get_field(m_name) );

  if ( !m_item->has_value(f) )
    return !m_has_value;
  else if ( m_has_value )
    {
      ValueType val;
      m_item->get_value( m_name, val );
      return val == m_value;
    }
  else
    return false;
} // action_set_item_field::is_identity()

/*----------------------------------------------------------------------------*/
template<typename ValueType>
wxString bf::action_set_item_field<ValueType>::get_description() const
{
  return _("Set field ") + std_to_wx_string(m_name);
} // action_set_item_field::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a generic description of the action.
 */
template<typename ValueType>
wxString bf::action_set_item_field<ValueType>::get_action_description()
{
  return _("Set field value");
} // action_set_item_field::get_action_description()
