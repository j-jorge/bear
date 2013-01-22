/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_snapshots_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_snapshots_node.hpp"

#include "bf/action.hpp"
#include "bf/snapshot.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/model_snapshot_node.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "snapshots".
 * \param a (out) The action in which we add the snapshots.
 * \param node The item node.
 */
void
bf::xml::model_snapshots_node::read( action& a, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("snapshots") );

  node = reader_tool::skip_comments(node->GetChildren());

  model_snapshot_node snapshot_node;
  snapshot* s;

  while( node != NULL )
    {
      s = NULL;

      if ( node->GetName() == wxT("snapshot") )
        try
          {
            s = a.create_snapshot();
            snapshot_node.read(*s, node);
            a.add_snapshot(s);
          }
        catch( std::exception& e )
          {
            a.remove_snapshot(s);
            delete s;
            claw::logger << claw::log_error
                         << "Error reading node of type 'snapshot': "
                         << e.what() << std::endl;
          }
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;

      node = reader_tool::skip_comments(node->GetNext());
    }
} // model_snapshots_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "snapshots".
 * \param a The action in which we take the snapshots.
 * \param os The stream in which we write.
 */
void
bf::xml::model_snapshots_node::write( const action& a, std::ostream& os ) const
{
  os << "<snapshots>\n";

  model_snapshot_node snapshot_node;
  action::const_snapshot_iterator it;

  for ( it=a.snapshot_begin(); it!=a.snapshot_end(); ++it )
    snapshot_node.write(*it, os);

  os << "</snapshots>\n";
} // model_snapshots_node::write()
