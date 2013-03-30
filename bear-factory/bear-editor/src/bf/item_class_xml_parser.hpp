/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that reads an XML file describing an item class and creates
 *        a corresponding item_class instance.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_CLASS_XML_PARSER_HPP__
#define __BF_ITEM_CLASS_XML_PARSER_HPP__

#include "bf/item_class.hpp"
#include "bf/type_field.hpp"

#include <wx/xml/xml.h>
#include <string>
#include <list>
#include <utility>

namespace bf
{
  class item_class_pool;

  /**
   * \brief A class that reads an XML file describing an item class and creates
   *        a corresponding item_class instance.
   * \author Julien Jorge
   */
  class item_class_xml_parser
  {
  public:
    static std::string get_item_class_name( const std::string& file_path );

    item_class* read
    ( const item_class_pool& pool, const std::string& file_path ) const;

  private:
    void parse_item_node
    ( item_class& item, const item_class_pool& pool,
      const wxXmlNode* node ) const;
    void read_item_properties( item_class& item, const wxXmlNode* node ) const;

    void read_item_fields( item_class& item, const wxXmlNode* node ) const;
    void read_new_default_value
    ( item_class& item, const wxXmlNode* node ) const;
    void read_removed_field( item_class& item, const wxXmlNode* node ) const;
    void read_description
    ( item_class& item, const wxXmlNode* node ) const;
    void read_field_type( item_class& item, const wxXmlNode* node ) const;

    type_field*
    add_integer_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_u_integer_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_real_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_string_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_boolean_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_sprite_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_animation_field( const std::string& name, const wxXmlNode* node ) const;
    type_field* add_item_reference_field
    ( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_font_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_color_field( const std::string& name, const wxXmlNode* node ) const;
    type_field*
    add_sample_field( const std::string& name, const wxXmlNode* node ) const;

    type_field* add_easing_field
    ( const std::string& name, const wxXmlNode* node ) const;

    std::string read_after( const wxXmlNode* node ) const;

    void read_set( const wxXmlNode* node, std::list<std::string>& set ) const;

    template<typename T, bool SetAvailable, bool RangeAvailable>
    type_field*
    create_field( const std::string& name, type_field::field_type ft,
                  const wxXmlNode* node ) const;

    template<typename T>
    std::pair<T, T> read_interval( const wxXmlNode* node ) const;

  }; // class item_class_xml_parser
} // namespace bf

#endif // __BF_ITEM_CLASS_XML_PARSER_HPP__

