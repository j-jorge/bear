/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::accelerator_table class.
 * \author Julien Jorge
 */
#include "bf/accelerator_table.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param key_code The code of the key to press to trigger the accelerator.
 * \param ctrl The state of the control key to trigger the accelerator.
 * \param alt The state of the alt key to trigger the accelerator.
 * \param shift The state of the shift key to trigger the accelerator.
 */
bf::accelerator_table::entry::entry
( int key_code, control_state ctrl, alt_state alt, shift_state shift )
  : m_key_code(key_code), m_control_state(ctrl), m_alt_state(alt),
    m_shift_state(shift)
{

} // accelerator_table::entry::entry()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compares the entry with an other one, in lexicographic order of their
 *        fields.
 * \param that The instance to compare to.
 */
bool bf::accelerator_table::entry::operator<( const entry& that ) const
{
  if ( m_key_code != that.m_key_code )
    return m_key_code < that.m_key_code;

  if ( m_control_state != that.m_control_state )
    return m_control_state < that.m_control_state;

  if ( m_alt_state != that.m_alt_state )
    return m_alt_state < that.m_alt_state;

  return m_shift_state < that.m_shift_state;
} // accelerator_table::entry::operator<()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. 
 * \param event_handler The instance that dispatches the key events on on which
 *        we dispatch the command events.
 */
bf::accelerator_table::accelerator_table( wxWindow& event_handler )
  : m_event_handler( event_handler )
{
  m_event_handler.Connect
    ( wxEVT_KEY_UP, wxKeyEventHandler(accelerator_table::on_key_pressed),
      NULL, this );
} // accelerator_table::accelerator_table()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an accelerator in the table.
 * \param event_type The type of the event dispatched when the accelerator is
 *        validated.
 */
void bf::accelerator_table::add_accelerator
( wxEventType event_type, int key_code, control_state ctrl,
  alt_state alt, shift_state shift )
{
  m_accelerators[ entry(key_code, ctrl, alt, shift) ] = event_type;
} // accelerator_table::add_accelerator()

/*----------------------------------------------------------------------------*/
/**
 * \brief A key has been pressed, we dispatch an event for the corresponding
 *        combination.
 * \param event The event describing the pressed keys.
 */
void bf::accelerator_table::on_key_pressed( wxKeyEvent& event )
{
  const control_state ctrl
    ( event.ControlDown() ? control_pressed : control_released );
  const alt_state alt( event.AltDown() ? alt_pressed : alt_released );
  const shift_state shift( event.ShiftDown() ? shift_pressed : shift_released );

  const entry e( event.GetKeyCode(), ctrl, alt, shift );

  const entry_map::const_iterator it( m_accelerators.find( e ) );

  if ( it != m_accelerators.end() )
    {
      wxCommandEvent command( wxEVT_COMMAND_MENU_SELECTED, it->second );
      m_event_handler.ProcessEvent( command );
    }
} // accelerator_table::on_key_pressed()
