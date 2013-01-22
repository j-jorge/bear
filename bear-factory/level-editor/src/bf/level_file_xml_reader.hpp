/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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
  class item_class_pool;
  class item_instance;
  class layer;
  class type_field;

  /**
   * \brief A class for reading source level files.
   * \author Julien Jorge
   */
  class level_file_xml_reader
  {
  public:
    gui_level* load
    ( const item_class_pool& pool, const wxString& file_path ) const;

  private:
    gui_level*
    load_level( const item_class_pool& pool, const wxXmlNode* node ) const;
    void load_layers
    ( const item_class_pool& pool, gui_level& lvl,
      const wxXmlNode* node ) const;
    void load_layer
    ( const item_class_pool& pool, gui_level& lvl,
      const wxXmlNode* node ) const;

    void load_layer_content
    ( const item_class_pool& pool, layer& lay, const wxXmlNode* node ) const;

    void load_items
    ( const item_class_pool& pool, layer& lay, const wxXmlNode* node ) const;
    void load_priorities( layer& lay, const wxXmlNode* node ) const;

    void load_item
    ( const item_class_pool& pool, layer& lay, const wxXmlNode* node ) const;
    void load_fields( item_instance& item, const wxXmlNode* node ) const;

  }; // class level_file_xml_reader
} // namespace bf

#endif // __BF_LEVEL_FILE_XML_READER_HPP__
