/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::any_animation class.
 * \author Julien Jorge
 */
#include "bf/any_animation.hpp"

#include "bf/compiled_file.hpp"
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a string representation of a content type.
 * \param c The content type.
 */
std::string bf::any_animation::content_to_string( content_type c )
{
  switch( c )
    {
    case content_animation: return "content_animation";
    case content_file: return "content_file";
    default:
      { CLAW_FAIL("Invalid content type."); return "invalid content type"; }
    }
} // any_animation::content_to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a content type from its string representation.
 * \param c The string representing the content type.
 */
bf::any_animation::content_type
bf::any_animation::string_to_content( const std::string& c )
{
  if ( c == "content_animation" )
    return content_animation;
  else if ( c == "content_file" )
    return content_file;
  else
    {
      CLAW_FAIL("Not a valid content name :'" + c + "'.");
      return content_animation;
    }
} // any_animation::string_to_content()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The initial content type.
 */
bf::any_animation::any_animation( content_type c )
  : m_content_type(c)
{

} // any_animation::any_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation file and switch to the content type "content_file"
 *        if needed.
 * \param a The animation file.
 */
void bf::any_animation::set_animation_file( const animation_file_type& a )
{
  m_content_type = content_file;
  m_animation_file = a;
} // any_animation::set_animation_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation and switch to the content type "content_animation"
 *        if needed.
 * \param a The animation.
 */
void bf::any_animation::set_animation( const animation& a )
{
  m_content_type = content_animation;
  m_animation = a;
} // any_animation::set_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reload the animation.
 */
void bf::any_animation::reload()
{
  if ( m_content_type == content_file )
    m_animation_file.set_path( m_animation_file.get_path() );
} // any_animation::reload()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation file.
 * \pre The content type is "content_file".
 */
const bf::animation_file_type& bf::any_animation::get_animation_file() const
{
  CLAW_PRECOND( m_content_type == content_file );
  return m_animation_file;
} // any_animation::get_animation_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation.
 * \pre The content type is "content_animation".
 */
const bf::animation& bf::any_animation::get_animation() const
{
  CLAW_PRECOND( m_content_type == content_animation );
  return m_animation;
} // any_animation::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation associated with the current content.
 * \pre The content type is "content_animation".
 */
bf::animation bf::any_animation::get_current_animation() const
{
  switch( m_content_type )
    {
    case content_animation: return m_animation;
    case content_file: return m_animation_file.get_animation();
    default:
      { CLAW_FAIL("Invalid content type."); return animation(); }
    }
} // any_animation::get_current_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Switch the current content to an other type.
 * \param c The new content type.
 * \remark When switching from an animation file to an animation, the data from
 *         the animation file is copied in the animation.
 */
void bf::any_animation::switch_to( content_type c )
{
  if ( c==m_content_type )
    return;

  if ( m_content_type == content_file )
    m_animation = m_animation_file.get_animation();

  m_content_type = c;
} // any_animation::switch_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current content type.
 */
bf::any_animation::content_type bf::any_animation::get_content_type() const
{
  return m_content_type;
} // any_animation::get_content_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the animation.
 * \param f The stream in which we write the compiled data.
 * \param c The context in which the compilation is done.
 */
void
bf::any_animation::compile( compiled_file& f, compilation_context& c ) const
{
  f << content_to_string(m_content_type);

  switch(m_content_type)
    {
    case content_animation:
      m_animation.compile(f, c);
      break;
    case content_file:
      m_animation_file.compile(f);
      break;
    default:
      { CLAW_FAIL("Invalid content type."); }
    }
} // any_animation::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two any_animations are the same.
 * \param that The other any_animation.
 */
bool bf::any_animation::operator==( const any_animation& that ) const
{
  if ( m_content_type != that.m_content_type )
    return false;

  switch(m_content_type)
    {
    case content_animation: return m_animation == that.m_animation;
    case content_file: return m_animation_file == that.m_animation_file;
    default:
      { CLAW_FAIL("Invalid content type."); return false; }
    }
} // any_animation::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two any_animations are different.
 * \param that The other any_animation.
 */
bool bf::any_animation::operator!=( const any_animation& that ) const
{
  return !(*this == that);
} // any_animation::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two any_animations in lexicographic order.
 * \param that The other any_animation.
 */
bool bf::any_animation::operator<( const any_animation& that ) const
{
  if ( m_content_type != that.m_content_type )
    return m_content_type < that.m_content_type;

  switch(m_content_type)
    {
    case content_animation: return m_animation < that.m_animation;
    case content_file: return m_animation_file < that.m_animation_file;
    default:
      { CLAW_FAIL("Invalid content type."); return false; }
    }
} // any_animation::operator<()
