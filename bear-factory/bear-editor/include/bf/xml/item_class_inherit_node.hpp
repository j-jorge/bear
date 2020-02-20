/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Read an xml "inherit" node of an item class.
 * \author Julien Jorge
 */
#ifndef __BF_XML_ITEM_CLASS_INHERIT_NODE_HPP__
#define __BF_XML_ITEM_CLASS_INHERIT_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class item_class;
  class item_class_pool;

  namespace xml
  {
    /**
     * \brief Read an xml "inherit" node of an item class.
     * \author Julien Jorge
     */
    class item_class_inherit_node
    {
    public:
      void read( const item_class_pool& pool, item_class& item,
                 const wxXmlNode* node ) const;
      void write( const item_class& item, std::ostream& os ) const;

    }; // class item_class_inherit_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_ITEM_CLASS_INHERIT_NODE_HPP__
