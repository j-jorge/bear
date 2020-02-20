/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Read an xml node "argument".
 * \author Julien Jorge
 */
#ifndef __BF_XML_RUN_ARGUMENT_NODE_HPP__
#define __BF_XML_RUN_ARGUMENT_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>
#include <string>

namespace bf
{
  namespace xml
  {
    /**
     * \brief Read an xml node "argument".
     * \author Julien Jorge
     */
    class run_argument_node
    {
    public:
      std::string read( const wxXmlNode* node ) const;
      void write( const std::string& arg, std::ostream& os ) const;

    }; // class run_argument_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_RUN_ARGUMENT_NODE_HPP__
