/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for writing source level files.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_FILE_XML_WRITER_HPP__
#define __BF_LEVEL_FILE_XML_WRITER_HPP__

#include <iostream>
#include <wx/xml/xml.h>

namespace bf
{
  class item_class;
  class item_instance;
  class layer;
  class level;
  class workspace_environment;

  /**
   * \brief A class for writing source level files.
   * \author Julien Jorge
   */
  class level_file_xml_writer
  {
  public:
    void save
    ( std::ostream& f, const level& lvl, workspace_environment& env ) const;

  private:
    void save_layer
    ( std::ostream& os, const layer& the_layer, 
      workspace_environment& env ) const;
    void save_items( std::ostream& os, const layer& item, 
                     workspace_environment& env ) const;
    void save_priority( std::ostream& os, const layer& item ) const;
    void save_item
    ( std::ostream& os, const item_instance& item, 
      workspace_environment& env ) const;

  }; // class level_file_xml_writer
} // namespace bf

#endif // __BF_LEVEL_FILE_XML_WRITER_HPP__
