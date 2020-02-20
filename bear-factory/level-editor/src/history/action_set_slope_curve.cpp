/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_slope_curve class.
 * \author Sebastien Angibaud
 */
#include "bf/history/action_set_slope_curve.hpp"

#include "bf/item_instance.hpp"
#include "bf/item_class_pool.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>
#include <iostream>

/*----------------------------------------------------------------------------*/
bf::action_set_slope_curve::action_set_slope_curve
( item_instance* item, double left_x, double left_y, 
  double right_x, double right_y )
  : m_item(item), m_left_x(left_x), m_left_y(left_y),
    m_right_x(right_x), m_right_y(right_y)
{
  CLAW_PRECOND( item != NULL );
} // action_set_slope_curve::action_set_slope_curve()

/*----------------------------------------------------------------------------*/
void bf::action_set_slope_curve::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  double left_x = 0;
  double left_y = 0;
  double right_x = 0;
  double right_y = 0;
  real_type left_x_field;
  real_type left_y_field;
  real_type right_x_field;
  real_type right_y_field;
      
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.left.x") ) )
    {
      m_item->get_value( "slope.control_point.left.x", left_x_field );
      left_x = left_x_field.get_value();
    }
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.left.y") ) )
    {
      m_item->get_value( "slope.control_point.left.y", left_y_field );
      left_y = left_y_field.get_value();
    }
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.right.x") ) )
    {
      m_item->get_value( "slope.control_point.right.x", right_x_field );
      right_x = right_x_field.get_value();
    }
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.right.y") ) )
    {
      m_item->get_value( "slope.control_point.right.y", right_y_field );
      right_y = right_y_field.get_value();
    }

  left_x_field.set_value(m_left_x);
  left_y_field.set_value(m_left_y);
  right_x_field.set_value(m_right_x);
  right_y_field.set_value(m_right_y);

  m_item->set_value("slope.control_point.left.x", left_x_field);
  m_item->set_value("slope.control_point.left.y", left_y_field);
  m_item->set_value("slope.control_point.right.x", right_x_field);
  m_item->set_value("slope.control_point.right.y", right_y_field);

  m_left_x = left_x;
  m_left_y = left_y;
  m_right_x = right_x;
  m_right_y = right_y;
} // action_set_slope_curve::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_slope_curve::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  /* the first call to execute saved the initial position. */
  execute(lvl);
} // action_set_slope_curve::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_slope_curve::is_identity( const gui_level& gui ) const
{
  CLAW_PRECOND( m_item!=NULL );

  double left_x = 0;
  double left_y = 0;
  double right_x = 0;
  double right_y = 0;

  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.left.x") ) )
    {
      real_type left_x_field;
      m_item->get_value( "slope.control_point.left.x", left_x_field );
      left_x = left_x_field.get_value();
    }
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.left.y") ) )
    {
      real_type left_y_field;
      m_item->get_value( "slope.control_point.left.y", left_y_field );
      left_y = left_y_field.get_value();
    }
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.right.x") ) )
    {
      real_type right_x_field;
      m_item->get_value( "slope.control_point.right.x", right_x_field );
      right_x = right_x_field.get_value();
    }
  if ( m_item->has_value
       ( m_item->get_class().get_field("slope.control_point.right.y") ) )
    {
      real_type right_y_field;
      m_item->get_value( "slope.control_point.right.y", right_y_field );
      right_y = right_y_field.get_value();
    }

  return m_left_x == left_x && 
    m_left_y == left_y && 
    m_right_x == right_x && 
    m_right_y == right_y;
} // action_set_slope_curve::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_slope_curve::get_description() const
{
  return _("Change slope curve");
} // action_set_slope_curve::get_description()
