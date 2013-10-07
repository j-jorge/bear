/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_file_xml_reader class.
 * \author Julien Jorge
 */
#include "bf/animation_file_xml_reader.hpp"

#include "bf/wx_facilities.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/xml_to_value.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a animation.
 * \param file_path The path to the animation file.
 * \param env The worksapce environment to use.
 */
bf::animation
bf::animation_file_xml_reader::load
( const wxString& file_path, workspace_environment* env ) const
{
  wxXmlDocument doc;

  if ( !doc.Load(file_path) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + wx_to_std_string(file_path) + "'" );

  wxXmlNode* node = doc.GetRoot();

  if ( node == NULL )
    throw xml::missing_node("animation");

  return load( node, env );
} // animation_file_xml_reader::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a node of type "animation".
 * \param node The node to parse.
 * \param env The workspace environment to use.
 */
bf::animation bf::animation_file_xml_reader::load
( wxXmlNode* node, workspace_environment* env ) const
{
  CLAW_PRECOND( node != NULL );

  if ( node->GetName() != wxT("animation") )
    throw xml::bad_node( wx_to_std_string(node->GetName()) );

  animation anim;
  xml::xml_to_value<animation> xml_conv;

  xml_conv( anim, node, env );

  return anim;
} // animation_file_xml_reader::load_animation()
