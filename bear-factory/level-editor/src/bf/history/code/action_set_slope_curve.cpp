/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_slope_curve class.
 * \author Sebastien Angibaud
 */
#include "bf/history/action_set_slope_curve.hpp"

#include <wx/intl.h>
#include <iostream>

/*----------------------------------------------------------------------------*/
bf::action_set_slope_curve::action_set_slope_curve
( item_instance* item, double left_x, double left_y, 
  double right_x, double right_y )
{
  std::cout << left_x << " " << left_y << " " 
            << right_x << " " << right_y << std::endl;
} // action_set_slope_curve::action_set_slope_curve()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_slope_curve::get_description() const
{
  return _("Change slope curve");
} // action_set_slope_curve::get_description()
