/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_file_type class.
 * \author Julien Jorge
 */
#include "bf/animation_file_type.hpp"

#include "bf/animation_file_xml_reader.hpp"
#include "bf/compiled_file.hpp"
#include "bf/path_configuration.hpp"
#include "bf/wx_facilities.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the path of the animation file.
 * \param p The new path.
 * \param env The worksapce environment to use.
 */
void bf::animation_file_type::set_path
( const std::string& p, workspace_environment* env )
{
  m_path = p;
  m_relative_path = p;

  if ( path_configuration::get_instance().expand_file_name
       (m_relative_path, env->get_name() ) )
    path_configuration::get_instance().get_relative_path
      (m_relative_path, env->get_name() );
  
  std::string::size_type pos = m_path.rfind(".canim");
  m_animation.clear();

  if ( pos != std::string::npos )
    {
      std::string p( m_path.substr(0, pos) + ".anim" );

      if ( path_configuration::get_instance().expand_file_name
           (p, 1, env->get_name() ) )
        {
          animation_file_xml_reader reader;
          m_animation = reader.load( std_to_wx_string(p), env );
        }
    }
} // animation_file_type::set_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path of the animation file.
 */
const std::string& bf::animation_file_type::get_path() const
{
  return m_path;
} // animation_file_type::get_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation of the file, combined with the rendering attributes
 *        of this instance.
 */
bf::animation bf::animation_file_type::get_animation() const
{
  animation anim( m_animation );
  anim.bitmap_rendering_attributes::combine( *this );
  return anim;
} // animation_file_type::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation of the file, without the rendering attributes of
 *        this instance.
 */
bf::animation bf::animation_file_type::original_animation() const
{
  return m_animation;
} // animation_file_type::load_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the animation_file_type.
 * \param f The stream in which we write the compiled data.
 */
void bf::animation_file_type::compile( compiled_file& f ) const
{
  f << m_relative_path;

  bitmap_rendering_attributes::compile(f);
} // animation_file_type::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two animation_file_types are the same.
 * \param that The other animation_file_type.
 */
bool bf::animation_file_type::operator==
( const animation_file_type& that ) const
{
  return that.bitmap_rendering_attributes::operator==(*this)
    && (m_path == that.m_path);
} // animation_file_type::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two animation_file_types are different.
 * \param that The other animation_file_type.
 */
bool bf::animation_file_type::operator!=
( const animation_file_type& that ) const
{
  return !(*this == that);
} // animation_file_type::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two animation_file_types in lexicographic order.
 * \param that The other animation_file_type.
 */
bool bf::animation_file_type::operator<
( const animation_file_type& that ) const
{
  if ( m_path == that.m_path )
    return that.bitmap_rendering_attributes::operator<(*this);
  else
    return m_path < that.m_path;
} // animation_file_type::operator<()
