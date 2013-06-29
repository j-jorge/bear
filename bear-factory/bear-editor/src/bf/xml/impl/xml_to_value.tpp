/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of the bf::xml::xml_to_value
 *        class.
 * \author Julien Jorge
 */

#include "bf/stream_conv.hpp"
#include "bf/xml/exception.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a xml value node.
 * \param v (out) The value we have read.
 * \param node The node from which we read the value.
 */
template<typename Type>
void bf::xml::xml_to_value<Type>::operator()
  ( Type& v, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node != NULL );
  wxString val;

  if ( !node->GetPropVal( wxT("value"), &val ) )
    throw missing_property( "value" );

  const std::string std_val( wx_to_std_string(val) );
  std::istringstream iss( std_val );

  if ( !stream_conv<Type>::read(iss, v) )
    throw bad_value( wx_to_std_string(node->GetName()), std_val );
} // xml_to_value::operator()()
