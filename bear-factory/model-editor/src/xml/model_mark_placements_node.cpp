/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_mark_placements_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_mark_placements_node.hpp"

#include "bf/snapshot.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/model_mark_placement_node.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "mark_placements".
 * \param s (out) The snapshot in which we add the mark_placements.
 * \param node The item node.
 */
void bf::xml::model_mark_placements_node::read
( snapshot& s, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("mark_placements") );

  node = reader_tool::skip_comments(node->GetChildren());

  model_mark_placement_node mark_placement_node;

  while( node != NULL )
    {
      if ( node->GetName() == wxT("mark_placement") )
        mark_placement_node.read(s, node);
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;

      node = reader_tool::skip_comments(node->GetNext());
    }
} // model_mark_placements_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "mark_placements".
 * \param s The snapshot in which we take the mark_placements.
 * \param os The stream in which we write.
 */
void bf::xml::model_mark_placements_node::write
( const snapshot& s, std::ostream& os ) const
{
  os << "<mark_placements>\n";

  model_mark_placement_node mark_placement_node;
  snapshot::const_mark_placement_iterator it;

  for ( it=s.mark_placement_begin(); it!=s.mark_placement_end(); ++it )
    mark_placement_node.write(*it, os);

  os << "</mark_placements>\n";
} // model_mark_placements_node::write()
