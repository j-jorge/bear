/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_rotate_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_rotate_selection.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_set_item_field.hpp"

#include "bf/item_class.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_rotate_selection::action_rotate_selection
( const gui_level& lvl, bool clockwise )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        item_selection::const_iterator it;
        const item_selection& selection( lvl.get_selection() );

        for (it=selection.begin(); it!=selection.end(); ++it)
          {
            const std::string field("basic_renderable_item.angle");
            const item_class& my_class( (*it)->get_class() );

            if ( my_class.has_field( field, type_field::real_field_type ) )
              {
                real_type angle;

                if ( (*it)->has_value( my_class.get_field(field) ) )
                  (*it)->get_value( field, angle );
                else
                  angle.set_value(0);

                if ( clockwise )
                  angle.set_value( angle.get_value() + 0.05 );
                else
                  angle.set_value( angle.get_value() - 0.05 );
                
                add_action
                  ( new action_set_item_field<real_type>( *it, field, angle ) );
              }
          }
      }
} // action_rotate_selection::action_rotate_selection()

/*----------------------------------------------------------------------------*/
wxString bf::action_rotate_selection::get_description() const
{
  return _("Rotate selected items");
} // action_rotate_selection::get_description()
