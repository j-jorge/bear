/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::input::system class.
 * \author Julien Jorge
 */
#include "input/system.hpp"

#include "input/joystick.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

#include <SDL/SDL.h>
#include <claw/exception.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Global initializations. Must be called at the begining of your
 *        program.
 */
void bear::input::system::initialize()
{
  if ( !SDL_WasInit(SDL_INIT_VIDEO) )
    if ( SDL_InitSubSystem(SDL_INIT_VIDEO) != 0 )
      throw claw::exception( SDL_GetError() );

  if ( SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0 )
    throw claw::exception( SDL_GetError() );

  SDL_EnableKeyRepeat(0, 0);
  SDL_EnableUNICODE(1);

  SDL_EventState( SDL_KEYDOWN, SDL_ENABLE );
  SDL_EventState( SDL_BUTTON_WHEELUP, SDL_ENABLE );
  SDL_EventState( SDL_BUTTON_WHEELDOWN, SDL_ENABLE );

  // force the creation of the instance
  get_instance().refresh();
} // system::initialize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Global uninitializations. Must be called at the end of your program.
 */
void bear::input::system::release()
{
  system::get_instance().clear();

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
} // system::release()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the instance.
 */
bear::input::system& bear::input::system::get_instance()
{
  return super::get_instance();
} // system::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh the controllers.
 * \remark Actually this method refresh all bear::input::system instances.
 */
void bear::input::system::refresh()
{
  SDL_PumpEvents();

  refresh_alone();
} // system::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an instance of the bear::input::keyboard class.
 */
bear::input::keyboard& bear::input::system::get_keyboard()
{
  return *m_keyboard;
} // system::get_keyboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an instance of the bear::input::mouse class.
 */
bear::input::mouse& bear::input::system::get_mouse()
{
  return *m_mouse;
} // system::get_mouse()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an instance of the bear::input::joystick class.
 * \param joy_id Joystick identifier.
 * \pre joy_id < bear::input::joystick::number_of_joysticks()
 */
bear::input::joystick& bear::input::system::get_joystick( unsigned int joy_id )
{
  CLAW_PRECOND( joy_id < joystick::number_of_joysticks() );

  return *m_joystick[joy_id];
} // system::get_joystick()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::input::system::system()
{
  m_keyboard = new keyboard;
  m_mouse = new mouse;

  for (unsigned int i=0; i!=joystick::number_of_joysticks(); ++i)
    m_joystick.push_back( new joystick(i) );
} // system::system()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::input::system::~system()
{
  clear();
} // system::~system()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh all controllers of this instance.
 */
void bear::input::system::refresh_alone()
{
  m_keyboard->refresh();
  m_mouse->refresh();

  for (unsigned int i=0; i!=m_joystick.size(); ++i)
    m_joystick[i]->refresh();
} // system::refresh_alone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close and delete the controllers.
 */
void bear::input::system::clear()
{
  if (m_keyboard != NULL)
    {
      delete m_keyboard;
      m_keyboard = NULL;
    }

  if (m_mouse != NULL)
    {
      delete m_mouse;
      m_mouse = NULL;
    }

  for (unsigned int i=0; i!=m_joystick.size(); ++i)
    delete m_joystick[i];

  m_joystick.clear();
} // system::clear()
