/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::run_argument_node class.
 * \author Julien Jorge
 */
#include "bf/xml/run_argument_node.hpp"

#include "bf/wx_facilities.hpp"
#include "bf/xml/run_argument_node.hpp"
#include "bf/xml/util.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "argument".
 * \param node The item node.
 */
std::string bf::xml::run_argument_node::read( const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("argument") );

  return wx_to_std_string( node->GetNodeContent() );
} // run_argument_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "arguments".
 * \param arg The argument to write.
 * \param os The stream in which we write.
 */
void bf::xml::run_argument_node::write
( const std::string& arg, std::ostream& os ) const
{
  os << "<argument>" << util::replace_special_characters(arg)
     << "</argument>\n";
} // run_argument_node::write()
