/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_resize_layer class.
 * \author Julien Jorge
 */
#include "bf/history/action_resize_layer.hpp"

#include "bf/gui_level.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_resize_layer::action_resize_layer
( bool fit_level, unsigned int width, unsigned int height,
  const std::string& class_name, const std::string& name,
  const std::string& tag, unsigned int lay )
  : m_fit_level(fit_level), m_width(width), m_height(height),
    m_class(class_name), m_name(name), m_tag(tag), m_layer(lay)
{

} // action_resize_layer::action_resize_layer()

/*----------------------------------------------------------------------------*/
void bf::action_resize_layer::execute( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );
  CLAW_PRECOND( !m_fit_level
                || ( (m_width == lvl.get_width())
                     && (m_height == lvl.get_height()) ) );

  const unsigned int f( lvl.get_layer( m_layer ).fits_level() );
  const unsigned int w( lvl.get_layer( m_layer ).get_width() );
  const unsigned int h( lvl.get_layer( m_layer ).get_height() );
  const std::string c( lvl.get_layer( m_layer ).get_class_name() );
  const std::string n( lvl.get_layer( m_layer ).get_name() );
  const std::string t( lvl.get_layer( m_layer ).get_tag() );

  lvl.get_layer( m_layer ).set_fit_level( m_fit_level );
  lvl.get_layer( m_layer ).resize( m_width, m_height );
  lvl.get_layer( m_layer ).set_class_name( m_class );
  lvl.get_layer( m_layer ).set_name( m_name );
  lvl.get_layer( m_layer ).set_tag( m_tag );

  m_fit_level = f;
  m_width = w;
  m_height = h;
  m_class = c;
  m_name = n;
  m_tag = t;
} // action_resize_layer::execute()

/*----------------------------------------------------------------------------*/
void bf::action_resize_layer::undo( gui_level& lvl )
{
  CLAW_PRECOND( lvl.layers_count() > m_layer );

  /* the first call to execute saved the initial size. */
  execute(lvl);
} // action_resize_layer::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_resize_layer::is_identity( const gui_level& lvl ) const
{
  return (m_fit_level == lvl.get_layer( m_layer ).fits_level())
    && (m_width == lvl.get_layer( m_layer ).get_width())
    && (m_height == lvl.get_layer( m_layer ).get_height())
    && (m_class == lvl.get_layer( m_layer ).get_class_name())
    && (m_name == lvl.get_layer( m_layer ).get_name())
    && (m_tag == lvl.get_layer( m_layer ).get_tag());
} // action_resize_layer::is_identity( const gui_level& gui )

/*----------------------------------------------------------------------------*/
wxString bf::action_resize_layer::get_description() const
{
  return _("Set layer size, name, class and tag");
} // action_resize_layer::get_description()
