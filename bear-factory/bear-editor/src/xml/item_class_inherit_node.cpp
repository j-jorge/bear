/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::item_class_inherit_node class.
 * \author Julien Jorge
 */
#include "bf/xml/item_class_inherit_node.hpp"

#include "bf/item_class.hpp"
#include "bf/item_class_pool.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "inherit".
 * \param pool The pool of item classes in which we take the parent classes.
 * \param item (out) The item class for we read the hierarchy.
 * \param node The node.
 */
void bf::xml::item_class_inherit_node::read
( const item_class_pool& pool, item_class& item, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("inherit") );

  node = reader_tool::skip_comments(node->GetChildren());

  while ( node!=NULL )
    {
      if ( node->GetName() == wxT("class") )
        item.add_super_class
          ( pool.get_item_class_ptr(wx_to_std_string(node->GetNodeContent())) );
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;

      node = reader_tool::skip_comments(node->GetNext());
    }
} // item_class_inherit_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "inherit".
 * \param item The item class to write.
 * \param os The stream in which we write.
 */
void bf::xml::item_class_inherit_node::write
( const item_class& item, std::ostream& os ) const
{
  item_class::const_super_class_iterator it;
  const item_class::const_super_class_iterator eit( item.super_class_end() );

  os << "<inherit>\n";

  for ( it=item.super_class_begin(); it!=eit; ++it)
    os << "<class>" << it->get_class_name() << "</class>\n";

  os << "</inherit>\n";
} // item_class_inherit_node::write()
