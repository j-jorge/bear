/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_snapshot_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_snapshot_node.hpp"

#include "bf/snapshot.hpp"
#include "bf/sound_description.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/model_sound_node.hpp"
#include "bf/xml/model_mark_placements_node.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "snapshot".
 * \param s (out) The snapshot we are reading.
 * \param node The item node.
 */
void bf::xml::model_snapshot_node::read
( snapshot& s, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );

  if ( node->GetName() != wxT("snapshot") )
    throw bad_node( wx_to_std_string(node->GetName()) );

  s.set_date( reader_tool::read_real(node, wxT("date")) );
  s.set_function( reader_tool::read_string_opt(node, wxT("function"), "") );

  s.set_size
    ( reader_tool::read_real_opt(node, wxT("width"), 100),
      reader_tool::read_real_opt(node, wxT("height"), 100) );
  s.set_x_alignment
    ( reader_tool::read_string_opt(node, wxT("x_alignment"), std::string()) );
  s.set_y_alignment
    ( reader_tool::read_string_opt(node, wxT("y_alignment"), std::string()) );
  s.set_x_alignment_value
    ( reader_tool::read_real_opt(node, wxT("x_alignment_value"), 0) );
  s.set_y_alignment_value
    ( reader_tool::read_real_opt(node, wxT("y_alignment_value"), 0) );

  node = reader_tool::skip_comments(node->GetChildren());

  while ( node!=NULL )
    {
      if ( node->GetName() == wxT("sound") )
        {
          model_sound_node sound_node;
          sound_description snd;
          sound_node.read(snd, node);
          s.set_sound(snd);
        }
      else if ( node->GetName() == wxT("mark_placements") )
        {
          model_mark_placements_node mark_placements_node;
          mark_placements_node.read(s, node);
        }
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;

      node = reader_tool::skip_comments(node->GetNext());
    }
} // model_snapshot_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "snapshot".
 * \param s The snapshot to write.
 * \param os The stream in which we write.
 */
void
bf::xml::model_snapshot_node::write( const snapshot& s, std::ostream& os ) const
{
  os << "<snapshot date=\"" << s.get_date() << '"';

  if ( !s.get_function().empty() )
    os << " function=\""
       << xml::util::replace_special_characters(s.get_function()) << '"';

  os << " width=\"" << s.get_width()
     << "\" height=\"" << s.get_height()
     << "\" x_alignment=\"" << s.get_x_alignment()
     << "\" y_alignment=\"" << s.get_y_alignment()
     << "\" x_alignment_value=\"" << s.get_x_alignment_value()
     << "\" y_alignment_value=\"" << s.get_y_alignment_value()
     << "\">\n";

  model_sound_node sound_node;
  sound_node.write( s.get_sound(), os );

  model_mark_placements_node mark_placements_node;
  mark_placements_node.write(s, os);

  os << "</snapshot>";
} // model_snapshot_node::write()
