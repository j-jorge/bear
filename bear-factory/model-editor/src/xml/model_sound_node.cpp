/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_sound_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_sound_node.hpp"

#include "bf/sound_description.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "sound".
 * \param s (out) The sound read in the node.
 * \param node The item node.
 */
void bf::xml::model_sound_node::read
( sound_description& s, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("sound") );

  s.set_played_globally
    ( reader_tool::read_bool_opt(node, wxT("global"), false) );

  const wxXmlNode* file =
    xml::reader_tool::skip_comments( node->GetChildren() );

  while ( file != NULL )
    {
      const std::string sound_name
        ( reader_tool::read_string( file, wxT("path") ) );
      s.insert_file( sound_name );
      file = xml::reader_tool::skip_comments( file->GetNext() );
    }
} // model_sound_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "sound".
 * \param s The sound to write.
 * \param os The stream in which we write.
 */
void bf::xml::model_sound_node::write
( const sound_description& s, std::ostream& os ) const
{
  if ( !s.get_files().empty() )
    {
      os << "<sound global=\"" << s.is_played_globally() << "\">\n";

      const sound_description::sound_set sounds( s.get_files() );

      for ( sound_description::sound_set::const_iterator it=sounds.begin();
            it!=sounds.end(); ++it )
        os << "<file path=\""
           << xml::util::replace_special_characters(*it) << "\"/>\n";

      os << "</sound>\n";
    }
} // model_sound_node::write()
