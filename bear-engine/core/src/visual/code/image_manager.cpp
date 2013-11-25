/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the image_manager class.
 * \author Julien Jorge
 */
#include <algorithm>

#include "visual/image_manager.hpp"

#include <claw/assert.hpp>
#include <claw/functional.hpp>
#include <claw/png.hpp>

/*---------------------------------------------------------------------------*/
/**
 * \brief Deletes all images.
 * \post m_images.empty == true.
 */
void bear::visual::image_manager::clear()
{
  m_images.clear();
  m_shader_program.clear();
} // image_manager::clear()

/*---------------------------------------------------------------------------*/
/**
 * \brief Adds an image to the cache.
 * \param name The name of the loaded image.
 * \param file A stream containing the file to load.
 * \pre name is not used by another image.
 * \post get_image(name) is the image in file_name.
 */
void bear::visual::image_manager::load_image
( std::string name, std::istream& file )
{
  CLAW_PRECOND( !exists(name) );

  claw::graphic::png img(file);
  add_image( name, image(img) );
} // image_manager::load_image()

/*---------------------------------------------------------------------------*/
/**
 * \brief Adds an existing image to the cache.
 * \param name The name of the loaded image.
 * \param img The image to add.
 * \pre name is not used by another image.
 * \post get_image(name) is the image in file_name.
 */
void bear::visual::image_manager::add_image
( std::string name, const image& img )
{
  CLAW_PRECOND( !exists(name) );

  m_images[name] = img;
} // image_manager::add_image()

/*---------------------------------------------------------------------------*/
/**
 * \brief Clear all images.
 *
 * The images are not deleted.
 */
void bear::visual::image_manager::clear_images()
{
  image_map_type::iterator it;

  for (it=m_images.begin(); it!=m_images.end(); ++it)
    it->second.clear();
} // image_manager::clear_images()

/*---------------------------------------------------------------------------*/
/**
 * \brief Restore an image.
 * \param name The name of the loaded image.
 * \param file A stream containing the file to load.
 * \pre There is an image called \a name.
 *
 * This method is useful when the screen goes dirty, to re-initialize the
 * images whithout losing references to them.
 */
void bear::visual::image_manager::restore_image
( std::string name, std::istream& file )
{
  CLAW_PRECOND( exists(name) );

  claw::graphic::png img(file);
  m_images[name].restore(img);
} // image_manager::restore_image()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get an image from the cache.
 * \param name The name of the image to get.
 * \pre There is an image called "name".
 */
bear::visual::image
bear::visual::image_manager::get_image( std::string name ) const
{
  CLAW_PRECOND( exists(name) );

  return m_images.find(name)->second;
} // image_manager::get_image()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get the names of the images.
 * \param names (out) The names of the images.
 */
void bear::visual::image_manager::get_image_names
( std::vector<std::string>& names ) const
{
  names.resize( m_images.size() );
  std::transform( m_images.begin(), m_images.end(), names.begin(),
                  claw::const_first<std::string, image>() );
} // image_manager::get_image_names()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tell if an image exists.
 * \param name The name of the image to find.
 */
bool bear::visual::image_manager::exists( std::string name ) const
{
  return m_images.find(name) != m_images.end();
} // image_manager::exists()

/*---------------------------------------------------------------------------*/
/**
 * \brief Creates a shader program.
 * \param name The name of the program.
 * \param file A stream containing the fragment shader code.
 * \pre name is not used by another shader.
 * \post get_fragment_shader(name) is the fragment_shader load from \a file
 */
void bear::visual::image_manager::load_shader_program
( std::string name, std::istream& file )
{
  CLAW_PRECOND( !has_shader_program(name) );

  m_shader_program[ name ] = shader_program( file );
} // image_manager::load_shader_program()

/*---------------------------------------------------------------------------*/
/**
 * \brief Clear all shader programs.
 *
 * The programs are not deleted.
 */
void bear::visual::image_manager::clear_shader_programs()
{
  std::map<std::string, shader_program>::iterator it;

  for (it=m_shader_program.begin(); it!=m_shader_program.end(); ++it)
    it->second.clear();
} // image_manager::clear_shader_programs()

/*---------------------------------------------------------------------------*/
/**
 * \brief Restores a shader program.
 * \param name The name of the loaded program.
 * \param file A stream containing the shader code.
 * \pre There is a shader program called \a name.
 *
 * This method is useful when the screen goes dirty, to re-initialize the
 * shaders whithout losing references to them.
 */
void bear::visual::image_manager::restore_shader_program
( std::string name, std::istream& file )
{
  CLAW_PRECOND( has_shader_program(name) );

  m_shader_program[ name ].restore( file );
} // image_manager::restore_shader_program()

/*---------------------------------------------------------------------------*/
/**
 * \brief Gets an existing shader program.
 * \param name The name of the program to get.
 * \pre There is an image called "name".
 */
bear::visual::shader_program
bear::visual::image_manager::get_shader_program( std::string name ) const
{
  CLAW_PRECOND( has_shader_program(name) );

  return m_shader_program.find( name )->second;
} // image_manager::get_shader_program()

/*---------------------------------------------------------------------------*/
/**
 * \brief Gets the names of the shader programs.
 * \param names (out) The names of the programs.
 */
void bear::visual::image_manager::get_shader_program_names
( std::vector<std::string>& names ) const
{
  names.resize( m_shader_program.size() );
  std::transform
    ( m_shader_program.begin(), m_shader_program.end(), names.begin(),
      claw::const_first<std::string, shader_program>() );
} // image_manager::get_shader_program_names()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tells if there is a shader program with a given name.
 * \param name The name of the program to find.
 */
bool bear::visual::image_manager::has_shader_program( std::string name ) const
{
  return m_shader_program.find(name) != m_shader_program.end();
} // image_manager::has_shader_program()
