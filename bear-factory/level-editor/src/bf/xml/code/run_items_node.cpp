/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::run_items_node class.
 * \author Julien Jorge
 */
#include "bf/xml/run_items_node.hpp"

#include "bf/run_configuration.hpp"

#include "bf/xml/item_instance_node.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "items".
 * \param config (out) The run configuration whose attributes are read.
 * \param env workspace environment env.
 * \param node The item node.
 */
void bf::xml::run_items_node::read
( run_configuration& config, workspace_environment& env,
  const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("items") );

  item_instance_node item_node(env);

  node = reader_tool::skip_comments(node->GetChildren());

  while ( node != NULL )
    if ( node->GetName() == wxT("item") )
      {
        const item_instance* item = item_node.read(node);

        if ( item != NULL ) 
          {
            config.append_item( *item );
            delete item;
          }

        node = reader_tool::skip_comments(node->GetNext());
      }
} // run_items_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "items".
 * \param config The configuration to write.
 * \param os The stream in which we write.
 */
void bf::xml::run_items_node::write
( const run_configuration& config, workspace_environment& env, 
  std::ostream& os ) const
{
  os << "<items>\n";

  item_instance_node item_node(env);

  for ( std::size_t i=0; i!=config.get_items_count(); ++i )
    item_node.write( config.get_item(i), os );

  os << "</items>\n";
} // run_items_node::write()
