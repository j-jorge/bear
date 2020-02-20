/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::shader_program class.
 * \author Julien Jorge
 */
#include "bear/visual/shader_program.hpp"

#include "bear/visual/screen.hpp"
#include "bear/visual/gl_shader_program.hpp"
#include "bear/visual/detail/get_default_fragment_shader_code.hpp"
#include "bear/visual/detail/get_default_vertex_shader_code.hpp"

#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::shader_program::shader_program()
  : m_impl(NULL)
{

} // shader_program::shader_program()

bear::visual::shader_program::shader_program( std::istream& fragment )
  : m_impl(new base_shader_program_ptr(NULL))
{
  restore( fragment );
}
/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a shader_program with a given code.
 * \param program code The code of the program.
 */
bear::visual::shader_program::shader_program
( const std::string& fragment, const std::string& vertex )
  : m_impl(new base_shader_program_ptr(NULL))
{
  restore( fragment, vertex );
} // shader_program::shader_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the data of the shader program.
 */
void bear::visual::shader_program::clear()
{
  if ( m_impl != NULL )
    *m_impl = NULL;
} // shader_program::clear()

void bear::visual::shader_program::restore( std::istream& fragment )
{
  std::ostringstream oss;
  oss << fragment.rdbuf();

  restore( oss.str(), detail::get_default_vertex_shader_code() );
}

void bear::visual::shader_program::restore
( const std::string& fragment, const std::string& vertex )
{
  if ( m_impl == NULL )
    m_impl = new base_shader_program_ptr(NULL);

  switch ( screen::get_sub_system() )
    {
    case screen::screen_gl:
      *m_impl = new gl_shader_program( fragment, vertex );
      break;
    case screen::screen_undef:
      claw::exception("screen sub system has not been set.");
    }
} // shader_program::restore()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the shader program is initialized.
 */
bool bear::visual::shader_program::is_valid() const
{
  bool result = false;

  if ( m_impl != NULL )
    result = ( *m_impl != NULL );

  return result;
} // shader_program::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigment operator.
 */
const bear::visual::base_shader_program*
bear::visual::shader_program::get_impl() const
{
  CLAW_PRECOND( is_valid() );

  return &(**m_impl);
} // shader_program::get_impl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the values of th variables of the program.
 */
const bear::visual::shader_program::input_variable_map&
bear::visual::shader_program::get_variables() const
{
  return m_input_variable;
} // shader_program::get_variables()
