/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for reading source level files.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_FILE_XML_READER_HPP__
#define __BF_LEVEL_FILE_XML_READER_HPP__

#include <iostream>
#include <wx/xml/xml.h>

namespace bf
{
  class gui_level;
  class item_instance;
  class layer;
  class type_field;
  class workspace_environment;

  /**
   * \brief A class for reading source level files.
   * \author Julien Jorge
   */
  class level_file_xml_reader
  {
  public:
    gui_level* load
    ( const wxString& file_path, workspace_environment& env ) const;

  private:
    gui_level*
    load_level( const wxXmlNode* node,
      workspace_environment& env ) const;
    void load_layers
    ( gui_level& lvl, const wxXmlNode* node, workspace_environment& env ) const;
    void load_layer
    ( gui_level& lvl, const wxXmlNode* node, workspace_environment& env ) const;

    void load_layer_content
    ( layer& lay, const wxXmlNode* node, workspace_environment& env ) const;

    void load_items
    ( layer& lay, const wxXmlNode* node, workspace_environment& env ) const;
    void load_priorities( layer& lay, const wxXmlNode* node ) const;

    void load_item
    ( layer& lay, const wxXmlNode* node, workspace_environment& env ) const;
    void load_fields( item_instance& item, const wxXmlNode* node ) const;

  }; // class level_file_xml_reader
} // namespace bf

#endif // __BF_LEVEL_FILE_XML_READER_HPP__
