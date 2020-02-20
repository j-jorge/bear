/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Read an xml node "sound".
 * \author Julien Jorge
 */
#ifndef __BF_XML_MODEL_SOUND_NODE_HPP__
#define __BF_XML_MODEL_SOUND_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class sound_description;

  namespace xml
  {
    /**
     * \brief Read an xml node "sound".
     * \author Julien Jorge
     */
    class model_sound_node
    {
    public:
      void read( sound_description& s, const wxXmlNode* node ) const;
      void write( const sound_description& s, std::ostream& os ) const;

    }; // class model_sound_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_MODEL_SOUND_NODE_HPP__
