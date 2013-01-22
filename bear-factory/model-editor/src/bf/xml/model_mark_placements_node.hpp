/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Read an xml node "mark_placements".
 * \author Julien Jorge
 */
#ifndef __BF_XML_MODEL_MARK_PLACEMENTS_NODE_HPP__
#define __BF_XML_MODEL_MARK_PLACEMENTS_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class snapshot;

  namespace xml
  {
    /**
     * \brief Read an xml node "mark_placements".
     * \author Julien Jorge
     */
    class model_mark_placements_node
    {
    public:
      void read( snapshot& s, const wxXmlNode* node ) const;
      void write( const snapshot& s, std::ostream& os ) const;

    }; // class model_mark_placements_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_MODEL_MARK_PLACEMENTS_NODE_HPP__
