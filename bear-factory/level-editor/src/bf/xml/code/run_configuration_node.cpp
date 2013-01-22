/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::run_configuration_node class.
 * \author Julien Jorge
 */
#include "bf/xml/run_configuration_node.hpp"

#include "bf/run_configuration.hpp"
#include "bf/xml/run_arguments_node.hpp"
#include "bf/xml/run_items_node.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "configuration".
 * \param config (out) The run configuration whose attributes are read.
 * \param pool The pool of the known item classes.
 * \param node The item node.
 */
void bf::xml::run_configuration_node::read
( run_configuration& config, const item_class_pool& pool,
  const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("configuration") );

  config.set_program_path
    ( reader_tool::read_string_opt(node, wxT("program_path"), "") );

  node = reader_tool::skip_comments(node->GetChildren());

  if ( node != NULL )
    if ( node->GetName() == wxT("arguments") )
      {
        run_arguments_node arguments_node;
        arguments_node.read( config, node );
        node = reader_tool::skip_comments(node->GetNext());
      }

  if ( node != NULL )
    if ( node->GetName() == wxT("items") )
      {
        run_items_node items_node;
        items_node.read( config, pool, node );
        node = reader_tool::skip_comments(node->GetNext());
      }
} // run_configuration_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "configuration".
 * \param config The configuration to write.
 * \param os The stream in which we write.
 */
void bf::xml::run_configuration_node::write
( const run_configuration& config, std::ostream& os ) const
{
  os << "<configuration program_path=\""
     << xml::util::replace_special_characters(config.get_program_path())
     << "\">\n";

  run_arguments_node arguments_node;
  run_items_node items_node;

  arguments_node.write(config, os);
  items_node.write(config, os);

  os << "</configuration>\n";
} // run_configuration_node::write()
