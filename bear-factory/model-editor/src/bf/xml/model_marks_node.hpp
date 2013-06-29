/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Read an xml node "marks".
 * \author Julien Jorge
 */
#ifndef __BF_XML_MODEL_MARKS_NODE_HPP__
#define __BF_XML_MODEL_MARKS_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class action;

  namespace xml
  {
    /**
     * \brief Read an xml node "marks".
     * \author Julien Jorge
     */
    class model_marks_node
    {
    public:
      void read( action& a, const wxXmlNode* node ) const;
      void write( const action& a, std::ostream& os ) const;

    }; // class model_marks_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_MODEL_MARKS_NODE_HPP__
