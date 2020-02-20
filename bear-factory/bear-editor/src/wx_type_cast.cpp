/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the utility functions to cast from and to wxWidgets
 *        types.
 * \author Julien Jorge
 */
#include "bf/wx_type_cast.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds a position_type frow a wxPoint.
 * \param p The point to convert.
 */
bf::position_type bf::wx_to_position( wxPoint p )
{
  return position_type( p.x, p.y );
} // wx_to_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds a wxPoint from a position_type.
 * \param p The point to convert.
 */
wxPoint bf::position_to_wx( position_type p )
{
  return wxPoint( p.x, p.y );
} // position_to_wx()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds a rectangle from a wxRect.
 * \param r The rectangle to convert.
 */
bf::rectangle_type bf::wx_to_rectangle( wxRect r )
{
  return rectangle_type( r.GetLeft(), r.GetBottom(), r.GetRight(), r.GetTop() );
} // wx_to_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds a wxRect from a rectangle.
 * \param r The rectangle to convert.
 */
wxRect bf::rectangle_to_wx( rectangle_type r )
{
  return wxRect( r.left(), r.bottom(), r.width(), r.height() );
} // rectangle_to_wx()
