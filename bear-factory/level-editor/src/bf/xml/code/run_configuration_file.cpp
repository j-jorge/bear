/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::run_configuration_file class.
 * \author Julien Jorge
 */
#include "bf/xml/run_configuration_file.hpp"

#include "bf/run_configuration.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/run_configuration_node.hpp"

#include <wx/xml/xml.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a run configuration.
 * \param pool The pool containing the known classes for the item instances.
 * \param file_path The path to the run configuration file.
 */
bf::run_configuration bf::xml::run_configuration_file::load
( const item_class_pool& pool, const wxString& file_path ) const
{
  wxXmlDocument doc;

  if ( !doc.Load(file_path) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + wx_to_std_string(file_path) + "'" );

  wxXmlNode* node = doc.GetRoot();

  if ( node == NULL )
    throw xml::missing_node("run");

  run_configuration_node reader;
  run_configuration result;

  reader.read( result, pool, node );

  return result;
} // run_configuration_file::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a run configuration.
 * \param config The run configuration to save.
 * \param os The stream in which we save it.
 */
void bf::xml::run_configuration_file::save
( const run_configuration& config, std::ostream& os ) const
{
  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";

  run_configuration_node node;
  node.write( config, os );
} // run_configuration_file::save()
