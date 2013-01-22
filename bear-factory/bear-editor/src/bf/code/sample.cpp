/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::sample class.
 * \author Julien Jorge
 */
#include "bf/sample.hpp"

#include "bf/compiled_file.hpp"
#include "bf/path_configuration.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::sample::sample()
  : m_loops(1), m_volume(1)
{

} // sample::sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the path to the resource to use.
 * \param name The new value.
 */
void bf::sample::set_path( const std::string& name )
{
  m_path = name;
} // sample::set_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the number of times the sample will be played.
 * \param loops The new value.
 */
void bf::sample::set_loops( const unsigned int loops )
{
  m_loops = loops;
} // sample::set_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief The volume at which the sample is played.
 * \param v The new value, in the interval (0, 1).
 */
void bf::sample::set_volume( const double v )
{
  if (v > 1)
    m_volume = 1;
  else if ( v < 0 )
    m_volume = 0;
  else
    m_volume = v;
} // sample::set_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get how many times the sample will be played.
 */
unsigned int bf::sample::get_loops() const
{
  return m_loops;
} // sample::get_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume at which the sample is played.
 */
double bf::sample::get_volume() const
{
  return m_volume;
} // sample::get_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the path to the resource to use.
 */
const std::string& bf::sample::get_path() const
{
  return m_path;
} // sample::get_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two samples are the same.
 * \param that The instance to compare to.
 */
bool bf::sample::operator==( const sample& that ) const
{
  return (m_path == that.m_path)
    && (m_loops == that.m_loops)
    && (m_volume == that.m_volume);
} // sample::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two samples are different.
 * \param that The instance to compare to.
 */
bool bf::sample::operator!=( const sample& that ) const
{
  return !(*this == that);
} // sample::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the sample.
 * \param f The stream in which we write the compiled sample.
 */
void bf::sample::compile( compiled_file& f ) const
{
  std::string path(m_path);

  if ( path_configuration::get_instance().expand_file_name(path) )
    path_configuration::get_instance().get_relative_path(path);

  f << path << m_loops << m_volume;
} // sample::compile()
