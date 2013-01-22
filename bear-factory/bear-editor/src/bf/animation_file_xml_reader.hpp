/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class for reading source animation files.
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_FILE_XML_READER_HPP__
#define __BF_ANIMATION_FILE_XML_READER_HPP__

#include "bf/animation.hpp"
#include "bf/libeditor_export.hpp"

#include <iostream>
#include <wx/xml/xml.h>

namespace bf
{
  /**
   * \brief A class for reading source animation files.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation_file_xml_reader
  {
  public:
    animation load( const wxString& file_path ) const;
    animation load( wxXmlNode* node ) const;

  }; // class animation_file_xml_reader
} // namespace bf

#endif // __BF_ANIMATION_FILE_XML_READER_HPP__
