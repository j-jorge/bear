/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Utility functions to cast from and to wx widgets.
 * \author Julien Jorge
 */
#ifndef __BF_WX_TYPE_CAST_HPP__
#define __BF_WX_TYPE_CAST_HPP__

#include "bf/types.hpp"
#include <wx/gdicmn.h>

namespace bf
{
  position_type wx_to_position( wxPoint p );
  wxPoint position_to_wx( position_type p );

  rectangle_type wx_to_rectangle( wxRect r );
  wxRect rectangle_to_wx( rectangle_type r );

} // namespace bf

#endif // __BF_WX_TYPE_CAST_HPP__
