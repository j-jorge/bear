/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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
 * \param pool The pool of known item classes.
 * \param node The item node.
 */
void bf::xml::run_items_node::read
( run_configuration& config, const item_class_pool& pool,
  const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("items") );

  item_instance_node item_node;

  node = reader_tool::skip_comments(node->GetChildren());

  while ( node != NULL )
    if ( node->GetName() == wxT("item") )
      {
        const item_instance* item = item_node.read(pool, node);

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
( const run_configuration& config, std::ostream& os ) const
{
  os << "<items>\n";

  item_instance_node item_node;

  for ( std::size_t i=0; i!=config.get_items_count(); ++i )
    item_node.write( config.get_item(i), os );

  os << "</items>\n";
} // run_items_node::write()
