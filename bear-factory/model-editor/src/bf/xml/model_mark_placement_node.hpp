/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Read an xml node "mark_placement".
 * \author Julien Jorge
 */
#ifndef __BF_XML_MODEL_MARK_PLACEMENT_NODE_HPP__
#define __BF_XML_MODEL_MARK_PLACEMENT_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class snapshot;
  class mark_placement;

  namespace xml
  {
    /**
     * \brief Read an xml node "mark_placement".
     * \author Julien Jorge
     */
    class model_mark_placement_node
    {
    public:
      void read( snapshot& s, const wxXmlNode* node ) const;
      void write( const mark_placement& s, std::ostream& os ) const;

    }; // class model_mark_placement_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_MODEL_MARK_PLACEMENT_NODE_HPP__
