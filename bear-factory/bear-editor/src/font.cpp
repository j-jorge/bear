/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::font class.
 * \author Julien Jorge
 */
#include "bf/font.hpp"

#include "bf/compilation_context.hpp"
#include "bf/compiled_file.hpp"
#include "bf/path_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::font::font()
  : m_size( 12 )
{

} // font::font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the name of the font resource to use.
 * \param name The new value.
 */
void bf::font::set_font_name( std::string name )
{
  m_font_name = name;
} // font::set_font_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the name of the font.
 */
std::string bf::font::get_font_name() const
{
  return m_font_name;
} // font::get_font_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the size of the text displayed with this font.
 * \param s The new value.
 */
void bf::font::set_size( double s )
{
  m_size = std::max( 0.0, s );
} // font::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the size of the text displayed with this font.
 */
double bf::font::get_size() const
{
  return m_size;
} // font::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the font.
 * \param f The stream in which we write the compiled font.
 * \param c The context in which the compilation is done.
 */
void bf::font::compile( compiled_file& f, compilation_context& c ) const
{
  std::string font_path(m_font_name);

  if ( path_configuration::get_instance().expand_file_name
       ( font_path, c.get_workspace_name() ) )
    path_configuration::get_instance().get_relative_path
      (font_path, c.get_workspace_name() );

  f << font_path << m_size;
} // font::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two fonts are the same.
 * \param that The instance to compare to.
 */
bool bf::font::operator==( const font& that ) const
{
  return (m_font_name == that.m_font_name)
    && (m_size == that.m_size);
} // font::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two fonts are different.
 * \param that The instance to compare to.
 */
bool bf::font::operator!=( const font& that ) const
{
  return !(*this == that);
} // font::operator!=()
