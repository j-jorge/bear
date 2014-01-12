/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Read an xml node "items".
 * \author Julien Jorge
 */
#ifndef __BF_XML_RUN_ITEMS_NODE_HPP__
#define __BF_XML_RUN_ITEMS_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class item_class_pool;
  class run_configuration;
  class workspace_environment;

  namespace xml
  {
    /**
     * \brief Read an xml node "items".
     * \author Julien Jorge
     */
    class run_items_node
    {
    public:
      void read
      ( run_configuration& config, workspace_environment& env,
        const wxXmlNode* node ) const;
      void write
      ( const run_configuration& config, workspace_environment& env, 
        std::ostream& os ) const;

    }; // class run_items_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_RUN_ITEMS_NODE_HPP__
