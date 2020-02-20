/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_resize_level class.
 * \author Julien Jorge
 */
#include "bf/history/action_resize_level.hpp"

#include "bf/gui_level.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_resize_level::action_resize_level
( const std::string& name, unsigned int width, unsigned int height,
  const std::string& music )
  : m_name(name), m_width(width), m_height(height), m_music(music)
{

} // action_resize_level::action_resize_level()

/*----------------------------------------------------------------------------*/
void bf::action_resize_level::execute( gui_level& lvl )
{
  const unsigned int w( lvl.get_width() );
  const unsigned int h( lvl.get_height() );
  const std::string m( lvl.get_music() );
  const std::string n( lvl.get_name() );

  lvl.set_size( m_width, m_height );
  lvl.set_music( m_music );
  lvl.set_name( m_name );

  m_width = w;
  m_height = h;
  m_music = m;
  m_name = n;
} // action_resize_level::execute()

/*----------------------------------------------------------------------------*/
void bf::action_resize_level::undo( gui_level& lvl )
{
  /* the first call to execute saved the initial size and music. */
  execute(lvl);
} // action_resize_level::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_resize_level::is_identity( const gui_level& lvl ) const
{
  return (lvl.get_width() == m_width) && (lvl.get_height() == m_height)
    && (lvl.get_music() == m_music) && (lvl.get_name() == m_name);
} // action_resize_level::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_resize_level::get_description() const
{
  return _("Set level's size, music and name");
} // action_resize_level::get_description()
