/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml exceptions classes.
 */
#include "bf/xml/exception.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param node_name The name of the unexpected node.
 */
bf::xml::bad_node::bad_node( const std::string& node_name )
  : m_msg( "Unexpected node name '" + node_name + "'")
{

} // bad_node::bad_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::xml::bad_node::~bad_node() throw()
{

} // bad_node::bad_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an explanation of the problem.
 */
const char* bf::xml::bad_node::what() const throw()
{
  return m_msg.c_str();
} // bad_node::what()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param node_name The name of the needed node.
 */
bf::xml::missing_node::missing_node( const std::string& node_name )
  : m_msg( "Missing node '" + node_name + "'")
{

} // missing_node::missing_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::xml::missing_node::~missing_node() throw()
{

} // missing_node::missing_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an explanation of the problem.
 */
const char* bf::xml::missing_node::what() const throw()
{
  return m_msg.c_str();
} // missing_node::what()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param property_name The name of the needed property.
 */
bf::xml::missing_property::missing_property
( const std::string& property_name )
  : m_msg( "Missing property '" + property_name + "'")
{

} // missing_property::missing_property()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::xml::missing_property::~missing_property() throw()
{

} // missing_property::missing_property()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an explanation of the problem.
 */
const char* bf::xml::missing_property::what() const throw()
{
  return m_msg.c_str();
} // missing_property::what()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param value The problematic value.
 */
bf::xml::bad_value::bad_value( const std::string& value )
  : m_msg( "Invalid value '" + value + "'")
{

} // bad_value::bad_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param type The expected type.
 * \param value The problematic value.
 */
bf::xml::bad_value::bad_value
( const std::string& type, const std::string& value )
  : m_msg( "Invalid value '" + value + "' (" + type + ")" )
{

} // bad_value::bad_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::xml::bad_value::~bad_value() throw()
{

} // bad_value::bad_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an explanation of the problem.
 */
const char* bf::xml::bad_value::what() const throw()
{
  return m_msg.c_str();
} // bad_value::what()
