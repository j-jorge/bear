/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for writing source animation files.
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_FILE_XML_WRITER_HPP__
#define __BF_ANIMATION_FILE_XML_WRITER_HPP__

#include "bf/animation.hpp"
#include "bf/libeditor_export.hpp"

#include <iostream>
#include <wx/xml/xml.h>

namespace bf
{
  /**
   * \brief A class for writing source animation files.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation_file_xml_writer
  {
  public:
    void save( std::ostream& f, const animation& anim ) const;

  }; // class animation_file_xml_writer
} // namespace bf

#endif // __BF_ANIMATION_FILE_XML_WRITER_HPP__
