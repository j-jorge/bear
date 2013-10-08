/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::item_instance_node class.
 * \author Julien Jorge
 */
#include "bf/xml/item_instance_node.hpp"

#include "bf/item_class_pool.hpp"
#include "bf/item_instance.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/item_instance_fields_node.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/workspace_environment.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a node parser.
 * \param env The workspace environment used.
 */
bf::xml::item_instance_node::item_instance_node( workspace_environment* env )
  : m_workspace( env )
{

} // item_instance_node::item_instance_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "item".
 * \param node The node.
 * \return A dynamically allocated item_instance as described by the XML node.
 */
bf::item_instance* bf::xml::item_instance_node::read
( const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("item") );

  wxString val;

  if ( !node->GetPropVal( wxT("class_name"), &val ) )
    throw xml::missing_property( "class_name" );

  std::string class_name( wx_to_std_string(val) );
  item_instance* item(NULL);

  try
    {
      item =
        new item_instance
        ( m_workspace->get_item_class_pool().get_item_class_ptr(class_name) );
      item->set_fixed
        ( xml::reader_tool::read_bool_opt(node, wxT("fixed"), false) );
      item->set_id
        ( wx_to_std_string(node->GetPropVal( wxT("id"), wxEmptyString )) );

      load_fields( *item, node->GetChildren() );
    }
  catch( std::exception& e )
    {
      claw::logger << claw::log_error << e.what() << std::endl;
      delete item;
      item = NULL;
    }

  return item;
} // item_instance_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "item".
 * \param item The item instance to write.
 * \param os The stream in which we write.
 */
void bf::xml::item_instance_node::write
( const item_instance& item, std::ostream& os ) const
{
  os << "    <item class_name='" << item.get_class().get_class_name()
     << "' fixed='";

  if ( item.get_fixed() )
    os << "true'";
  else
    os << "false'";

  if ( !item.get_id().empty() )
    os << " id='" << util::replace_special_characters( item.get_id() ) << "'";

  os << ">\n";

  item_instance_fields_node field_node( m_workspace );
  field_node.write(item, os);

  os << "    </item><!-- " << item.get_class().get_class_name() << " -->\n\n";
} // item_instance_node::write()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the fields of an item.
 * \param item The item in which we set the fields.
 * \param node The node to parse.
 */
void bf::xml::item_instance_node::load_fields
( item_instance& item, const wxXmlNode* node ) const
{
  node = xml::reader_tool::skip_comments(node);

  if ( node != NULL )
    {
      if ( node->GetName() == wxT("fields") )
        {
          xml::item_instance_fields_node reader( m_workspace );
          reader.read(item, node);
        }
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;
    }
} // item_instance_fields_node::load_fields()
