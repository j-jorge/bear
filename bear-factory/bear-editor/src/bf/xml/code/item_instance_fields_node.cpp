/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::item_instance_fields_node class.
 * \author Julien Jorge
 */
#include "bf/xml/item_instance_fields_node.hpp"

#include "bf/item_class.hpp"
#include "bf/item_instance.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/item_instance_field_node.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/workspace_environment.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a node parser.
 * \param env The workspace environment used.
 */
bf::xml::item_instance_fields_node::item_instance_fields_node
( workspace_environment* env )
  : m_workspace( env )
{

} // item_instance_fields_node::item_instance_fields_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "fields".
 * \param item (out) The item class for we read the hierarchy.
 * \param node The node.
 */
void bf::xml::item_instance_fields_node::read
( item_instance& item, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("fields") );

  item_instance_field_node field_node( m_workspace );
  node = reader_tool::skip_comments(node->GetChildren());

  while ( node!=NULL )
    {
      if ( node->GetName() == wxT("field") )
        field_node.read(item, node);
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;

      node = reader_tool::skip_comments(node->GetNext());
    }
} // item_instance_fields_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "fields".
 * \param item The item instance to write.
 * \param os The stream in which we write.
 */
void bf::xml::item_instance_fields_node::write
( const item_instance& item, std::ostream& os ) const
{
  os << "<fields>\n";

  save_item_by_class( item, item.get_class(), os );

  os << "</fields>\n";
} // item_instance_fields_node::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the fields of an item by exploring its parent classes.
 * \param item The item to save.
 * \param the_class The super class of \a item for which we save the fields.
 * \param os The stream in which we write.
 */
void bf::xml::item_instance_fields_node::save_item_by_class
( const item_instance& item,
  const item_class& the_class, std::ostream& os ) const
{
  item_class::const_super_class_iterator itc;

  for ( itc=the_class.super_class_begin();
        itc!=the_class.super_class_end(); ++itc )
    save_item_by_class(item, *itc, os);

  item_class::field_iterator it;
  xml::item_instance_field_node field_node( m_workspace );

  for ( it=the_class.field_begin(); it!=the_class.field_end(); ++it )
    if ( item.has_value(*it) )
      field_node.write( item, *it, os );
} // item_instance_field_node::save_item_by_class()
