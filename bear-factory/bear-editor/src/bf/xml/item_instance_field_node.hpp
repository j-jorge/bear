/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Process an xml "field" node of an item instance.
 * \author Julien Jorge
 */
#ifndef __BF_XML_ITEM_INSTANCE_FIELD_NODE_HPP__
#define __BF_XML_ITEM_INSTANCE_FIELD_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

#include "bf/custom_type.hpp"

namespace bf
{
  class item_instance;
  class type_field;

  namespace xml
  {
    /**
     * \brief Process an xml "field" node of an item instance.
     * \author Julien Jorge
     */
    class item_instance_field_node
    {
    public:
      void read( item_instance& item, const wxXmlNode* node ) const;
      void write
      ( const item_instance& item, const type_field& f,
        std::ostream& os ) const;

    private:
      void load_field
      ( item_instance& item, const type_field& f, const wxXmlNode* node ) const;
      void save_field
      ( const item_instance& item, const type_field& f,
        std::ostream& os ) const;

      template<typename Type>
      void load_value
      ( item_instance& item, const std::string& field_name,
        const wxXmlNode* node ) const;

      template<typename Type>
      void load_value_list
      ( item_instance& item, const std::string& field_name,
        const wxXmlNode* node ) const;

      template<typename T>
      void save_value
      ( std::ostream& os, const std::string& field_name,
        const item_instance& item ) const;

      template<typename Type>
      void save_value
      ( std::ostream& os, const std::string& field_name,
        const item_instance& item, const std::string& node_name ) const;

      template<typename T>
      void save_value_list
      ( std::ostream& os, const std::string& field_name,
        const item_instance& item ) const;

      template<typename Type>
      void save_value_list
      ( std::ostream& os, const std::string& field_name,
        const item_instance& item, const std::string& node_name ) const;

    }; // class item_instance_field_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_ITEM_INSTANCE_FIELD_NODE_HPP__
