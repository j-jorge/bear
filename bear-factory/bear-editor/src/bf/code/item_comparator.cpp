/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_comparator class.
 * \author Julien Jorge
 */
#include "bf/item_comparator.hpp"

#include "bf/item_instance.hpp"

/*----------------------------------------------------------------------------*/
bool bf::item_comparator::by_place::operator()
( const item_instance& a, const item_instance& b ) const
{
  bool result = false;

  if ( a.get_rendering_parameters().get_left()
       < b.get_rendering_parameters().get_left() )
    result = true;
  else if ( a.get_rendering_parameters().get_left()
            == b.get_rendering_parameters().get_left() )
    {
      if ( a.get_rendering_parameters().get_bottom()
           < b.get_rendering_parameters().get_bottom() )
        result = true;
      else if ( a.get_rendering_parameters().get_bottom()
                == b.get_rendering_parameters().get_bottom() )
        {
          if ( a.get_rendering_parameters().get_width()
               < b.get_rendering_parameters().get_width() )
            result = true;
          else if ( a.get_rendering_parameters().get_width()
                    == b.get_rendering_parameters().get_width() )
            result = ( a.get_rendering_parameters().get_height()
                       < b.get_rendering_parameters().get_height() );
        }
    }

  return result;
} // item_comparator::by_place::operator()
