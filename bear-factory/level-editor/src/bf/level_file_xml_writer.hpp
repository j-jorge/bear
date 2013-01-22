/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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

  /**
   * \brief A class for writing source level files.
   * \author Julien Jorge
   */
  class level_file_xml_writer
  {
  public:
    void save( std::ostream& f, const level& lvl ) const;

  private:
    void save_layer( std::ostream& os, const layer& the_layer ) const;
    void save_items( std::ostream& os, const layer& item ) const;
    void save_priority( std::ostream& os, const layer& item ) const;
    void save_item( std::ostream& os, const item_instance& item ) const;

  }; // class level_file_xml_writer
} // namespace bf

#endif // __BF_LEVEL_FILE_XML_WRITER_HPP__
