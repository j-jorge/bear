/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_rotate_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_rotate_selection.hpp"

#include "bf/item_class.hpp"
#include "bf/item_selection.hpp"

#include "bf/history/action_set_item_field.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param selection The selection of items to rotate.
 * \param clockwise Indicates if the rotation is clockwise.
 */
bf::action_rotate_selection::action_rotate_selection
( const item_selection& selection, bool clockwise )
  : action_group( _("Rotate selected items") )
{
  m_selection = selection;

  const std::string field("basic_renderable_item.angle");
  const double delta( clockwise ? 0.05 : -0.05 );

  for ( item_selection::const_iterator it( selection.begin() );
        it != selection.end(); ++it )
    {
      const item_class& my_class( (*it)->get_class() );

      if ( my_class.has_field( field, type_field::real_field_type ) )
        {
          real_type angle;

          if ( (*it)->has_value( my_class.get_field(field) ) )
            (*it)->get_value( field, angle );
          else
            angle.set_value(0);

          angle.set_value( angle.get_value() + delta );
                
          add_action
            ( new action_set_item_field<real_type>( *it, field, angle ) );
        }
    }
} // action_rotate_selection::action_rotate_selection()

/*----------------------------------------------------------------------------*/
/**
 * Gets the selection rotated by this action.
 */
bf::item_selection bf::action_rotate_selection::get_selection() const
{
  return m_selection;
} // action_rotate_selection::get_selection()
