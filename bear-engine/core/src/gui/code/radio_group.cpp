/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::radio_group class.
 * \author Julien Jorge
 */
#include "gui/radio_group.hpp"

#include "gui/callback_function.hpp"
#include "gui/radio_button.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a button in the group.
 * \param b The button to add.
 * \param margin The margin with the previous button in the group.
 */
void bear::gui::radio_group::add_button
( radio_button* b, visual::size_type margin )
{
  if ( m_buttons.empty() )
    b->set_bottom(margin);
  else
    b->set_bottom( m_buttons.back()->top() + margin );

  b->add_checked_callback
    ( callback_function_maker
      ( boost::bind( &radio_group::on_check, this, m_buttons.size() ) ) );
  m_buttons.push_back(b);

  insert( b );
} // radio_group::add_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the selected radio button.
 */
const bear::gui::radio_button* bear::gui::radio_group::get_selection() const
{
  for ( std::size_t i=0; i!=m_buttons.size(); ++i )
    if ( m_buttons[i]->checked() )
      return m_buttons[i];

  return NULL;
} // radio_group::get_selection( )

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a click on a button of the group;
 * \param b The index of the selected button.
 */
void bear::gui::radio_group::on_check( std::size_t b ) const
{
  for ( std::size_t i=0; i!=m_buttons.size(); ++i )
    if ( b != i )
      m_buttons[i]->set_value(false);
} // radio_group::on_check()
