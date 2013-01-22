/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::model_file class.
 * \author Julien Jorge
 */
#include "bf/xml/model_file.hpp"

#include "bf/gui_model.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/model_action_node.hpp"
#include "bf/xml/reader_tool.hpp"

#include <wx/xml/xml.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a model.
 * \param file_path The path to the model file.
 */
bf::gui_model* bf::xml::model_file::load( const wxString& file_path ) const
{
  wxXmlDocument doc;

  if ( !doc.Load(file_path) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + wx_to_std_string(file_path) + "'" );

  wxXmlNode* node = doc.GetRoot();

  if ( node == NULL )
    throw xml::missing_node("model");

  return load_model( node );
} // model_file::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a model.
 * \param mdl The model to save.
 * \param os The stream in which we save it.
 */
void bf::xml::model_file::save( const gui_model& mdl, std::ostream& os ) const
{
  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
     << "<model>\n";

  xml::model_action_node action;
  model::const_action_iterator it;

  for ( it=mdl.action_begin(); it!=mdl.action_end(); ++it )
    action.write(*it, os);

  os << "</model>\n" << std::endl;
} // model_file::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a node of type "model".
 * \param node The node to parse.
 */
bf::gui_model* bf::xml::model_file::load_model( const wxXmlNode* node ) const
{
  if ( node->GetName() != wxT("model") )
    throw xml::bad_node( wx_to_std_string(node->GetName()) );

  gui_model* mdl = new gui_model;
  node = reader_tool::skip_comments(node->GetChildren());
  xml::model_action_node action;

  while ( node!=NULL )
    {
      if ( node->GetName() == wxT("action") )
        action.read(*mdl, node);

      node = reader_tool::skip_comments(node->GetNext());
    }

  return mdl;
} // model_file::load_model()
