/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::reader_tool class.
 * \author Julien Jorge
 */
#include "bf/xml/reader_tool.hpp"

#include "bf/xml/exception.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/xml/xml.h>
#include <claw/assert.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Skip the comment nodes.
 * \param node Start from this node.
 */
const wxXmlNode* bf::xml::reader_tool::skip_comments( const wxXmlNode* node )
{
  bool stop = false;

  while ( !stop && (node!=NULL) )
    if ( node->GetName() == wxT("comment") )
      node = node->GetNext();
    else
      stop = true;

  return node;
} // reader_tool::skip_comments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in an integer.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 */
int
bf::xml::reader_tool::read_int( const wxXmlNode* node, const wxString& prop )
{
  CLAW_PRECOND(node!=NULL);

  int result;
  wxString val;

  if ( !node->GetAttribute( prop, &val ) )
    throw xml::missing_property( wx_to_std_string(prop) );

  std::istringstream iss( wx_to_std_string(val) );

  if ( !(iss >> result) )
    throw xml::bad_value( "integer", wx_to_std_string(val) );

  return result;
} // reader_tool::read_int()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in an unsigned integer.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 */
unsigned int
bf::xml::reader_tool::read_uint( const wxXmlNode* node, const wxString& prop )
{
  CLAW_PRECOND(node!=NULL);

  unsigned int result;
  wxString val;

  if ( !node->GetAttribute( prop, &val ) )
    throw xml::missing_property( wx_to_std_string(prop) );

  std::istringstream iss( wx_to_std_string(val) );

  if ( !(iss >> result) )
    throw xml::bad_value( "unsigned integer", wx_to_std_string(val) );

  return result;
} // reader_tool::read_uint()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in a string.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 */
std::string
bf::xml::reader_tool::read_string( const wxXmlNode* node, const wxString& prop )
{
  CLAW_PRECOND(node!=NULL);

  wxString val;

  if ( !node->GetAttribute( prop, &val ) )
    throw xml::missing_property( wx_to_std_string(prop) );

  return wx_to_std_string(val);
} // reader_tool::read_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in a real number.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 */
double
bf::xml::reader_tool::read_real( const wxXmlNode* node, const wxString& prop )
{
  CLAW_PRECOND(node!=NULL);

  double result;
  wxString val;

  if ( !node->GetAttribute( prop, &val ) )
    throw xml::missing_property( wx_to_std_string(prop) );

  std::istringstream iss( wx_to_std_string(val) );

  if ( !(iss >> result) )
    throw xml::bad_value( "unsigned integer", wx_to_std_string(val) );

  return result;
} // reader_tool::read_real()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in an integer.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 * \param def The default value.
 */
int bf::xml::reader_tool::read_int_opt
( const wxXmlNode* node, const wxString& prop, int def )
{
  CLAW_PRECOND(node!=NULL);

  try
    {
      return read_int(node, prop);
    }
  catch ( ... )
    {
      return def;
    }
} // reader_tool::read_int_opt()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in an unsigned integer.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 * \param def The default value.
 */
unsigned int bf::xml::reader_tool::read_uint_opt
( const wxXmlNode* node, const wxString& prop, unsigned int def )
{
  CLAW_PRECOND(node!=NULL);

  try
    {
      return read_uint(node, prop);
    }
  catch ( ... )
    {
      return def;
    }
} // reader_tool::read_uint_opt()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of a property in a string.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 * \param def The default value.
 */
std::string bf::xml::reader_tool::read_string_opt
( const wxXmlNode* node, const wxString& prop, const std::string& def )
{
  CLAW_PRECOND(node!=NULL);

  try
    {
      return read_string(node, prop);
    }
  catch ( ... )
    {
      return def;
    }
} // reader_tool::read_string_opt()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of an optional property in a real.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 * \param def The default value.
 */
double bf::xml::reader_tool::read_real_opt
( const wxXmlNode* node, const wxString& prop, double def )
{
  CLAW_PRECOND(node!=NULL);

  try
    {
      return read_real(node, prop);
    }
  catch ( ... )
    {
      return def;
    }
} // reader_tool::read_real_opt()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of an optional property in a boolean.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 * \param def The default value.
 */
bool bf::xml::reader_tool::read_bool_opt
( const wxXmlNode* node, const wxString& prop, bool def )
{
  CLAW_PRECOND(node!=NULL);

  bool result(def);
  wxString val;

  if ( node->GetAttribute( prop, &val ) )
    {
      if ( (val == wxT("true")) || (val == wxT("1")) )
        result = true;
      else if ( (val == wxT("false")) || (val == wxT("0")) )
        result = false;
    }

  return result;
} // reader_tool::read_bool_opt()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the value of an optional property in a trinary logic.
 * \param node The node in which we take the property.
 * \param prop The name of the property to get.
 * \param def The default value.
 */
bf::trinary_logic::value_type bf::xml::reader_tool::read_trinary_logic_opt
( const wxXmlNode* node, const wxString& prop,
  bf::trinary_logic::value_type def )
{
  CLAW_PRECOND(node!=NULL);

  bf::trinary_logic::value_type result(def);
  wxString val;

  if ( node->GetAttribute( prop, &val ) )
    {
      if ( ( val ==
             std_to_wx_string( trinary_logic::to_string
                               (trinary_logic::boolean_true))) ||
           (val == wxT("1")) || (val == wxT("true")) )
           result = trinary_logic::boolean_true;
      else if ( ( val ==
                  std_to_wx_string
                  ( trinary_logic::to_string
                    (trinary_logic::boolean_false))) ||
                (val == wxT("0")) || (val == wxT("false")) )
        result = trinary_logic::boolean_false;
      else
        result = trinary_logic::boolean_random;
    }

  return result;
} // reader_tool::read_trinary_logic_opt()
