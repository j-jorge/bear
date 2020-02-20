/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::sound_description class.
 * \author Julien Jorge
 */
#include "bf/sound_description.hpp"

#include "bf/compiled_file.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::sound_description::sound_description()
  : m_played_globally(false)
{

} // sound_description::sound_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the path of a file to play.
 * \param file The path to add.
 */
void bf::sound_description::insert_file( std::string file )
{
  m_files.insert( file );
} // sound_description::insert_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes the path of a file to play.
 * \param file The path to add.
 */
void bf::sound_description::erase_file( std::string file )
{
  m_files.erase( file );
} // sound_description::erase_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the paths of the files to play.
 */
const bf::sound_description::sound_set& bf::sound_description::get_files() const
{
  return m_files;
} // sound_description::get_files()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if the sound is played globally.
 * \param b Global or not.
 */
void bf::sound_description::set_played_globally( bool b )
{
  m_played_globally = b;
} // sound_description::set_played_globally()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sound is played globally.
 */
bool bf::sound_description::is_played_globally() const
{
  return m_played_globally;
} // sound_description::is_played_globally()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the sound.
 * \param f The file in which we compile.
 */
void bf::sound_description::compile( compiled_file& f ) const
{
  f << m_played_globally << m_files.size();

  for ( sound_set::const_iterator it=m_files.begin(); it!=m_files.end(); ++it )
    f << *it;
} // sound_description::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this description is the same than an other one.
 * \param that The instance to compare to.
 */
bool bf::sound_description::operator==( const sound_description& that ) const
{
  return (get_files() == that.get_files())
    && (is_played_globally() == that.is_played_globally() );
} // sound_description::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this description is not the same than an other one.
 * \param that The instance to compare to.
 */
bool bf::sound_description::operator!=( const sound_description& that ) const
{
  return !(*this == that);
} // sound_description::operator!=()
