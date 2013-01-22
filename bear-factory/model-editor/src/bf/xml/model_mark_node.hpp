/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Read an xml node "mark".
 * \author Julien Jorge
 */
#ifndef __BF_XML_MODEL_MARK_NODE_HPP__
#define __BF_XML_MODEL_MARK_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class mark;

  namespace xml
  {
    /**
     * \brief Read an xml node "mark".
     * \author Julien Jorge
     */
    class model_mark_node
    {
    public:
      void
      read( mark& m, const wxXmlNode* node ) const;
      void write( const mark& m, std::ostream& os ) const;

    }; // class model_mark_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_MODEL_MARK_NODE_HPP__
