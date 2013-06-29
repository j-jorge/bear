/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_marks_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_marks_node.hpp"

#include "bf/action.hpp"
#include "bf/mark.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/model_mark_node.hpp"
#include "bf/xml/reader_tool.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "marks".
 * \param a (out) The action in which we add the marks.
 * \param node The item node.
 */
void bf::xml::model_marks_node::read( action& a, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("marks") );

  node = reader_tool::skip_comments(node->GetChildren());

  model_mark_node mark_reader;
  mark* m;

  while( node != NULL )
    {
      m = NULL;
      if ( node->GetName() == wxT("mark") )
        try
          {
            m = new mark;
            mark_reader.read(*m, node);
            a.add_mark(m);
          }
        catch( std::exception& e )
          {
            delete m;
            claw::logger << claw::log_error
                         << "Error reading node of type 'mark': "
                         << e.what() << std::endl;
          }
      else
        claw::logger << claw::log_warning << "Ignored node '"
                     << wx_to_std_string(node->GetName()) << "'" << std::endl;

      node = reader_tool::skip_comments(node->GetNext());
    }
} // model_marks_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "marks".
 * \param a The action in which we take the marks.
 * \param os The stream in which we write.
 */
void
bf::xml::model_marks_node::write( const action& a, std::ostream& os ) const
{
  os << "<marks>\n";

  model_mark_node mark_writer;
  action::const_mark_iterator it;

  for ( it=a.mark_begin(); it!=a.mark_end(); ++it )
    mark_writer.write(*it, os);

  os << "</marks>\n";
} // model_marks_node::write()
