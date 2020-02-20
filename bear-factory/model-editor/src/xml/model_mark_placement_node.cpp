/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_mark_placement_node class.
 * \author Julien Jorge
 */
#include "bf/xml/model_mark_placement_node.hpp"

#include "bf/mark.hpp"
#include "bf/mark_placement.hpp"
#include "bf/snapshot.hpp"
#include "bf/xml/reader_tool.hpp"
#include "bf/xml/util.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an xml node "mark_placement".
 * \param s (out) The snapshot in which we set the mark_placement.
 * \param node The item node.
 */
void bf::xml::model_mark_placement_node::read
( snapshot& s, const wxXmlNode* node ) const
{
  CLAW_PRECOND( node!=NULL );
  CLAW_PRECOND( node->GetName() == wxT("mark_placement") );

  const std::string label( reader_tool::read_string(node, wxT("label")) );

  if ( !s.has_mark(label) )
    claw::logger << claw::log_error << "There is no mark labeled '" << label
                 << "'" << std::endl;
  else
    {
      mark_placement& m = s.get_placement(label);

      m.set_x_position
        ( reader_tool::read_real_opt(node, wxT("x"), m.get_x_position()) );
      m.set_y_position
        ( reader_tool::read_real_opt(node, wxT("y"), m.get_y_position()) );
      m.set_width
        ( reader_tool::read_real_opt(node, wxT("width"), m.get_width()) );
      m.set_height
        ( reader_tool::read_real_opt(node, wxT("height"), m.get_height()) );
      m.set_depth_position
        ( reader_tool::read_int_opt(node, wxT("z"), m.get_depth_position()) );
      m.set_angle
        ( reader_tool::read_real_opt(node, wxT("angle"), m.get_angle()) );
      m.set_visibility
        ( reader_tool::read_bool_opt(node, wxT("visible"), m.is_visible()) );
      m.set_collision_function
        ( reader_tool::read_string_opt(node, wxT("function"), "") );

      bear::easing e;
      const std::string def(e.to_string());
      e.from_string( reader_tool::read_string_opt(node, wxT("x_easing"), def) );
      m.set_x_easing(e);

      e.from_string( reader_tool::read_string_opt(node, wxT("y_easing"), def) );
      m.set_y_easing(e);

      e.from_string
        ( reader_tool::read_string_opt(node, wxT("width_easing"), def) );
      m.set_width_easing(e);

      e.from_string
        ( reader_tool::read_string_opt(node, wxT("height_easing"), def) );
      m.set_height_easing(e);

      e.from_string
        ( reader_tool::read_string_opt(node, wxT("angle_easing"), def) );
      m.set_angle_easing(e);
    }
} // model_mark_placement_node::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an xml node "mark_placement".
 * \param m The mark_placement to write.
 * \param os The stream in which we write.
 */
void bf::xml::model_mark_placement_node::write
( const mark_placement& m, std::ostream& os ) const
{
  os << "<mark_placement label=\""
     << xml::util::replace_special_characters(m.get_mark()->get_label())
     << "\" function=\""
     << xml::util::replace_special_characters(m.get_collision_function())
     << "\" x=\"" << m.get_x_position() << "\" y=\"" << m.get_y_position()
     << "\" width=\"" << m.get_width() << "\" height=\"" << m.get_height()
     << "\" z=\"" << m.get_depth_position() << "\" angle=\"" << m.get_angle()
     << "\" visible=\"" << m.is_visible()
     << "\" x_easing=\"" << m.get_x_easing().to_string()
     << "\" y_easing=\"" << m.get_y_easing().to_string()
     << "\" width_easing=\"" << m.get_width_easing().to_string()
     << "\" height_easing=\"" << m.get_height_easing().to_string()
     << "\" angle_easing=\"" << m.get_angle_easing().to_string()
     << "\"/>\n";
} // model_mark_placement_node::write()
