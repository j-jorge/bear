/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::xcf_map class.
 * \author Julien Jorge
 */
#include "xcf_map.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param xcf_directory The directory where the xcf files are taken.
 */
sdc::xcf_map::xcf_map( std::string xcf_directory )
  : m_xcf_directory( xcf_directory )
{

} // xcf_map::xcf_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads the description of an xcf file.
 * \param name The name of the xcf file.
 */
void sdc::xcf_map::load( std::string name )
{

} // xcf_map::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the map contains a given xcf file.
 * \param name The name of the xcf file.
 */
bool sdc::xcf_map::has_info( std::string name ) const
{
  return m_xcf_info.find( name ) != m_xcf_info.end();
} // xcf_map::has_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the informations of a given xcf file.
 * \param name The name of the xcf file.
 */
sdc::xcf_info sdc::xcf_map::get_info( std::string name ) const
{
  return m_xcf_info.find( name )->second;
} // xcf_map::get_info()
