/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Process an xml "fields" node of an item instance.
 * \author Julien Jorge
 */
#ifndef __BF_XML_ITEM_INSTANCE_FIELDS_NODE_HPP__
#define __BF_XML_ITEM_INSTANCE_FIELDS_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class workspace_environment;
  class item_class;
  class item_instance;

  namespace xml
  {
    /**
     * \brief Process an xml "fields" node of an item instance.
     * \author Julien Jorge
     */
    class item_instance_fields_node
    {
    public:
      item_instance_fields_node( workspace_environment& env );

      void read( item_instance& item, const wxXmlNode* node ) const;
      void write
      ( const item_instance& item, std::ostream& os ) const;

    private:
      void save_item_by_class
      ( const item_instance& item, const item_class& the_class,
        std::ostream& os ) const;

    private:
      /** \brief The workspace environment used. */
      workspace_environment& m_workspace;

    }; // class item_instance_fields_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_ITEM_INSTANCE_FIELDS_NODE_HPP__
