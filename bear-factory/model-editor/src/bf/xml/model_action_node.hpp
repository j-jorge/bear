/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Read an xml node "action".
 * \author Julien Jorge
 */
#ifndef __BF_XML_MODEL_ACTION_NODE_HPP__
#define __BF_XML_MODEL_ACTION_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class action;
  class gui_model;
  class workspace_environment;

  namespace xml
  {
    /**
     * \brief Read an xml node "action".
     * \author Julien Jorge
     */
    class model_action_node
    {
    public:
      void read
      ( gui_model& mdl, const wxXmlNode* node, 
        workspace_environment& env ) const;
      void write( const action& a, std::ostream& os ) const;

    }; // class model_action_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_MODEL_ACTION_NODE_HPP__
