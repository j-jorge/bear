/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Process an xml "item" node describing an item instance.
 * \author Julien Jorge
 */
#ifndef __BF_XML_ITEM_INSTANCE_NODE_HPP__
#define __BF_XML_ITEM_INSTANCE_NODE_HPP__

#include <wx/xml/xml.h>
#include <iostream>

namespace bf
{
  class image_pool;
  class item_instance;
  class item_class_pool;

  namespace xml
  {
    /**
     * \brief Process an xml "item" node describing an item instance.
     * \author Julien Jorge
     */
    class item_instance_node
    {
    public:
      item_instance_node
      ( const item_class_pool& classes, const image_pool& images );

      item_instance* read( const wxXmlNode* node ) const;
      void write( const item_instance& item, std::ostream& os ) const;

    private:
      void load_fields( item_instance& item, const wxXmlNode* node ) const;

    private:
      /** \brief The pool from which we can access the item classes. */
      const item_class_pool& m_item_class_pool;

      /** \brief The pool from which we can access the images used by the
          item. */
      const image_pool& m_image_pool;

    }; // class item_instance_node
  } // namespace xml
} // namespace bf

#endif // __BF_XML_ITEM_INSTANCE_NODE_HPP__
