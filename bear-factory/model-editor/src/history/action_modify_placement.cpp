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

#include "bf/mark_placement.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the x-position of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<double>*
bf::make_action_modify_placement_x( snapshot* s, const mark* m, double value )
{
  return new action_modify_placement<double>
    ( s, m,
      boost::bind( &mark_placement::set_x_position, _1, _2 ),
      boost::bind( &mark_placement::get_x_position, _1 ),
      value );
} // make_action_modify_placement_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the y-position of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<double>*
bf::make_action_modify_placement_y( snapshot* s, const mark* m, double value )
{
  return new action_modify_placement<double>
    ( s, m,
      boost::bind( &mark_placement::set_y_position, _1, _2 ),
      boost::bind( &mark_placement::get_y_position, _1 ),
      value );
} // make_action_modify_placement_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the angle of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<double>*
bf::make_action_modify_placement_angle
( snapshot* s, const mark* m, double value )
{
  return new action_modify_placement<double>
    ( s, m,
      boost::bind( &mark_placement::set_angle, _1, _2 ),
      boost::bind( &mark_placement::get_angle, _1 ),
      value );
} // make_action_modify_placement_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the depth of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<int>*
bf::make_action_modify_placement_depth
( snapshot* s, const mark* m, int value )
{
  return new action_modify_placement<int>
    ( s, m,
      boost::bind( &mark_placement::set_depth_position, _1, _2 ),
      boost::bind( &mark_placement::get_depth_position, _1 ),
      value );
} // make_action_modify_placement_depth()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the visibility of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<bool>*
bf::make_action_modify_placement_visibility
( snapshot* s, const mark* m, bool value )
{
  return new action_modify_placement<bool>
    ( s, m,
      boost::bind( &mark_placement::set_visibility, _1, _2 ),
      boost::bind( &mark_placement::is_visible, _1 ),
      value );
} // make_action_modify_placement_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the width of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<double>*
bf::make_action_modify_placement_width
( snapshot* s, const mark* m, double value )
{
  return new action_modify_placement<double>
    ( s, m,
      boost::bind( &mark_placement::set_width, _1, _2 ),
      boost::bind( &mark_placement::get_width, _1 ),
      value );
} // make_action_modify_placement_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build a model action to change the height of a mark placement.
 * \param s The snapshot from which the mark placement is taken.
 * \param m The mark whose placement is modified.
 * \param value The new value of the property.
 */
bf::action_modify_placement<double>*
bf::make_action_modify_placement_height
( snapshot* s, const mark* m, double value )
{
  return new action_modify_placement<double>
    ( s, m,
      boost::bind( &mark_placement::set_height, _1, _2 ),
      boost::bind( &mark_placement::get_height, _1 ),
      value );
} // make_action_modify_placement_height()
