/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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
( const std::string& name, std::istream& file )
{
  CLAW_PRECOND( !exists(name) );

  claw::graphic::png img(file);
  m_images[name] = image(img);
} // image_manager::load_image()

/*---------------------------------------------------------------------------*/
/**
 * \brief Clear all images.
 *
 * The images are not deleted.
 */
void bear::visual::image_manager::clear_images()
{
  std::map<std::string, image>::iterator it;

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
( const std::string& name, std::istream& file )
{
  CLAW_PRECOND( exists(name) );

  claw::graphic::image img(file);
  m_images[name].restore(img);
} // image_manager::restore_image()

/*---------------------------------------------------------------------------*/
/**
 * \brief Get an image from the cache.
 * \param name The name of the image to get.
 * \pre There is an image called "name".
 */
const bear::visual::image& bear::visual::image_manager::get_image
(const std::string& name) const
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
bool bear::visual::image_manager::exists( const std::string& name ) const
{
  return m_images.find(name) != m_images.end();
} // image_manager::exists()
