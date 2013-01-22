/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml* exceptions classes.
 */
#include "bf/class_not_found.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param class_name The name of the class.
 */
bf::class_not_found::class_not_found( const std::string& class_name )
  : m_msg( "Can't find item class '" + class_name + "'"),
    m_class_name(class_name)
{

} // class_not_found::class_not_found()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::class_not_found::~class_not_found() throw()
{

} // class_not_found::class_not_found()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an explanation of the problem.
 */
const char* bf::class_not_found::what() const throw()
{
  return m_msg.c_str();
} // class_not_found::what()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the class.
 */
const std::string& bf::class_not_found::class_name() const throw()
{
  return m_class_name;
} // class_not_found::class_name()
