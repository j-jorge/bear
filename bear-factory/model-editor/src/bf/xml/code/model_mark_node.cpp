/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_mark_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_mark_node.hpp"

#include "bf/mark.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"
#include "bf/xml/value_to_xml.hpp"
#include "bf/xml/xml_to_value.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "mark".
 * \param m (out) The mark to read.
 * \param node The item node.
 */
void bf::xml::model_mark_node::read( mark& m, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("mark") );

  m.set_label( reader_tool::read_string(node, wxT("label")) );
  m.apply_angle_to_animation
    ( reader_tool::read_bool_opt
      ( node, wxT("apply_angle_to_animation"), m.apply_angle_to_animation() ) );
  m.pause_animation_when_hidden
    ( reader_tool::read_bool_opt
      ( node, wxT("pause_animation_when_hidden"),
        m.pause_animation_when_hidden() ) );

  const wxXmlNode* child = reader_tool::skip_comments(node->GetChildren());

  if ( child!=NULL )
    {
      if ( xml_to_value<any_animation>::supported_node(child->GetName()) )
        {
          any_animation a;
          xml_to_value<any_animation> reader;

          reader( a, child );
          a.reload();
          m.set_animation( a );
          child = reader_tool::skip_comments(child);
        }
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;
    }
} // model_mark_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "mark".
 * \param m The mark to write.
 * \param os The stream in which we write.
 */
void
bf::xml::model_mark_node::write( const mark& m, std::ostream& os ) const
{
  os << "<mark label=\"" << xml::util::replace_special_characters(m.get_label())
     << "\" apply_angle_to_animation=\"" << m.apply_angle_to_animation()
     << "\" pause_animation_when_hidden=\"" << m.pause_animation_when_hidden()
     << "\">\n";

  if ( m.has_animation() )
    value_to_xml<any_animation>::write( os, m.get_animation() );

  os << "</mark>\n";

} // model_mark_node::write()
