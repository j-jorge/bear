/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_action_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_action_node.hpp"

#include "bf/gui_model.hpp"
#include "bf/sound_description.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/model_marks_node.hpp"
#include "bf/xml/model_snapshots_node.hpp"
#include "bf/xml/model_sound_node.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "action".
 * \param mdl (out) The model for which we read the item.
 * \param node The item node.
 * \param env The workspace environment used.
 */
void
bf::xml::model_action_node::read
( gui_model& mdl, const wxXmlNode* node, workspace_environment * env ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("action") );

  std::string name( reader_tool::read_string(node, wxT("name")) );
  double duration( reader_tool::read_real(node, wxT("duration")) );

  action* a = new action(name, duration);
  model_sound_node sound_node;
  model_marks_node marks_node;
  model_snapshots_node snapshots_node;

  a->set_auto_next
    ( reader_tool::read_string_opt(node, wxT("auto_next"), std::string()) );

  node = reader_tool::skip_comments(node->GetChildren());

  try
    {
      if ( node != NULL )
        if ( node->GetName() == wxT("sound") )
          {
            sound_description s;
            sound_node.read(s, node);
            a->set_sound(s);
            node = reader_tool::skip_comments(node->GetNext());
          }

      if ( node != NULL )
        if ( node->GetName() == wxT("marks") )
          {
            marks_node.read(*a, node, env);
            node = reader_tool::skip_comments(node->GetNext());
          }

      if ( node != NULL )
        if ( node->GetName() == wxT("snapshots") )
          {
            snapshots_node.read(*a, node);
            node = reader_tool::skip_comments(node->GetNext());
          }
    }
  catch ( ... )
    {
      delete a;
      throw;
    }

  mdl.add_action(a);
} // model_action_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "action".
 * \param a The action to write.
 * \param os The stream in which we write.
 */
void
bf::xml::model_action_node::write( const action& a, std::ostream& os ) const
{
  os << "<action name=\"" << xml::util::replace_special_characters(a.get_name())
     << "\" duration=\"" << a.get_duration()
     << "\" auto_next=\""
     << xml::util::replace_special_characters(a.get_auto_next())
     << "\">\n";

  model_sound_node sound_node;
  model_marks_node marks_node;
  model_snapshots_node snapshot_node;

  sound_node.write(a.get_sound(), os);
  marks_node.write(a, os);
  snapshot_node.write(a, os);

  os << "</action>\n";
} // model_action_node::write()
