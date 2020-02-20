/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::run_arguments_node class.
 * \author Julien Jorge
 */
#include "bf/xml/run_arguments_node.hpp"

#include "bf/run_configuration.hpp"

#include "bf/xml/run_argument_node.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "arguments".
 * \param config (out) The run configuration whose attributes are read.
 * \param node The item node.
 */
void bf::xml::run_arguments_node::read
( run_configuration& config, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("arguments") );

  run_argument_node argument_node;

  node = reader_tool::skip_comments(node->GetChildren());

  while ( node != NULL )
    if ( node->GetName() == wxT("argument") )
      {
        config.append_argument( argument_node.read( node ) );
        node = reader_tool::skip_comments(node->GetNext());
      }
} // run_arguments_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "arguments".
 * \param config The configuration to write.
 * \param os The stream in which we write.
 */
void bf::xml::run_arguments_node::write
( const run_configuration& config, std::ostream& os ) const
{
  os << "<arguments>\n";

  run_argument_node argument_node;

  for ( std::size_t i=0; i!=config.get_arguments_count(); ++i )
    argument_node.write( config.get_argument(i), os );

  os << "</arguments>\n";
} // run_arguments_node::write()
