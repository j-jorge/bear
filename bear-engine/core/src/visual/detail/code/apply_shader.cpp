#include "visual/detail/apply_shader.hpp"

#include "visual/gl_error.hpp"
#include "visual/gl_shader_program.hpp"
#include "visual/shader_program.hpp"

namespace bear
{
  namespace visual
  {
    namespace detail
    {
      class uniform_setter
      {
      public:
        explicit uniform_setter( GLuint program );

        void operator()( const std::string& name, int value ) const;
        void operator()( const std::string& name, float value ) const;
        void operator()( const std::string& name, bool value ) const;
        void operator()
        ( const std::string& name, const std::array< float, 16 >& value ) const;

      private:
        const GLuint m_program;
      };
    }
  }
}
  
bear::visual::detail::uniform_setter::uniform_setter( GLuint program )
  : m_program( program )
{

}

void bear::visual::detail::uniform_setter::operator()
( const std::string& name, int value ) const
{
  glUniform1i( glGetUniformLocation( m_program, name.c_str() ), value);
  VISUAL_GL_ERROR_THROW();
}

void bear::visual::detail::uniform_setter::operator()
( const std::string& name, float value ) const
{
  glUniform1f( glGetUniformLocation( m_program, name.c_str() ), value);
  VISUAL_GL_ERROR_THROW();
}

void bear::visual::detail::uniform_setter::operator()
( const std::string& name, bool value ) const
{
  glUniform1i( glGetUniformLocation( m_program, name.c_str() ), value);
  VISUAL_GL_ERROR_THROW();
}

void bear::visual::detail::uniform_setter::operator()
  ( const std::string& name, const std::array< float, 16 >& value ) const
{
  glUniformMatrix4fv
    ( glGetUniformLocation( m_program, name.c_str() ),
      1, GL_FALSE, value.data() );
  VISUAL_GL_ERROR_THROW();
}





void bear::visual::detail::apply_shader( const shader_program& shader )
{
  assert ( shader.is_valid() );

  const gl_shader_program* const s
    ( static_cast<const gl_shader_program*>( shader.get_impl() ) );

  assert( s->program_id() != 0 );
  
  glUseProgram( s->program_id() );
  VISUAL_GL_ERROR_THROW();

  shader_program::variable_visitor_type visitor;
  shader_program::input_variable_map vars( shader.get_variables() );
  visitor.run( vars, uniform_setter( s->program_id() ) );
}

