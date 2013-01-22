/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_class_xml_parser.
 * \author Julien Jorge
 */
#include "bf/item_class_xml_parser.hpp"

#include "bf/type_field_set.hpp"
#include "bf/type_field_interval.hpp"
#include "bf/wx_facilities.hpp"
#include "bf/xml/exception.hpp"
#include "bf/xml/item_class_inherit_node.hpp"

#include <iostream>
#include <limits>
#include <claw/logger.hpp>
#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the item class described in a file.
 * \param file_path The path of the XML file to parse.
 */
std::string bf::item_class_xml_parser::get_item_class_name
( const std::string& file_path )
{
  wxXmlDocument doc;

  if ( !doc.Load( std_to_wx_string(file_path) ) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + file_path + "'" );

  wxXmlNode* node = doc.GetRoot();

  if ( node->GetName() != wxT("item") )
    throw xml::bad_node( wx_to_std_string(node->GetName()) );

  wxString val;

  if ( !node->GetPropVal( wxT("class"), &val ) )
    throw xml::missing_property("class");

  return wx_to_std_string(val);
} // item_class_xml_parser::get_item_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read an item class XML file.
 * \param pool The pool of item classes in which we take the parent classes.
 * \param file_path The path of the XML file to parse.
 */
bf::item_class* bf::item_class_xml_parser::read
( const item_class_pool& pool, const std::string& file_path ) const
{
  wxXmlDocument doc;

  if ( !doc.Load( std_to_wx_string(file_path) ) )
    throw std::ios_base::failure
      ( "Cannot load the XML file '" + file_path + "'" );

  item_class* result = new item_class;

  try
    {
      parse_item_node( *result, pool, doc.GetRoot() );
    }
  catch(...)
    {
      delete result;
      throw;
    }

  return result;
} // item_class_xml_parser::read()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of the file.
 * \param item (out) The resulting item.
 * \param pool The pool of item classes in which we take the parent classes.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::parse_item_node
( item_class& item, const item_class_pool& pool, const wxXmlNode* node ) const
{
  if ( node->GetName() != wxT("item") )
    throw xml::bad_node( wx_to_std_string(node->GetName()) );

  read_item_properties( item, node );

  for ( node=node->GetChildren(); node!=NULL; node=node->GetNext() )
    if ( node->GetName() == wxT("fields") )
      read_item_fields( item, node );
    else if ( node->GetName() == wxT("inherit") )
      {
        xml::item_class_inherit_node reader;
        reader.read(pool, item, node);
      }
    else if ( node->GetName() == wxT("description") )
      read_description(item, node);
    else if ( node->GetName() == wxT("new_default_value") )
      read_new_default_value( item, node );
    else if ( node->GetName() == wxT("remove_field") )
      read_removed_field( item, node );
    else
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "' "
                   << "\n" << wx_to_std_string(node->GetNodeContent())
                   << std::endl;
} // item_class_xml_parser::parse_item_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the properties of the item.
 * \param item (out) The resulting item.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::read_item_properties
( item_class& item, const wxXmlNode* node ) const
{
  wxString val;

  if ( !node->GetPropVal( wxT("class"), &val ) )
    throw xml::missing_property("class");

  item.set_class_name( wx_to_std_string(val) );

  if ( !node->GetPropVal( wxT("category"), &val ) )
    throw xml::missing_property("category");

  item.set_category( wx_to_std_string(val) );

  item.set_color
    ( wx_to_std_string(node->GetPropVal( wxT("box_color"), wxT("#00FF00") )) );

  item.set_url
    ( wx_to_std_string(node->GetPropVal( wxT("url"), wxEmptyString )) );

  item.set_fixable
    ( node->GetPropVal( wxT("fixable"), wxT("true") ) == wxT("true") );
} // item_class_xml_parser::read_item_properties()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the fields of the item.
 * \param item (out) The resulting item.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::read_item_fields
( item_class& item, const wxXmlNode* node ) const
{
  for ( node=node->GetChildren(); node!=NULL; node=node->GetNext() )
    if ( node->GetName() == wxT("field") )
      read_field_type( item, node );
    else
      throw xml::bad_node( wx_to_std_string(node->GetName()) );
} // item_class_xml_parser::read_item_fields()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a new default value for a field of the item.
 * \param item (out) The resulting item.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::read_new_default_value
( item_class& item, const wxXmlNode* node ) const
{
  wxString val;

  if ( !node->GetPropVal( wxT("name"), &val ) )
    throw xml::missing_property("name");

  item.new_default_value
    ( wx_to_std_string(val), wx_to_std_string(node->GetNodeContent()) );
} // item_class_xml_parser::read_new_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the name of a field of the parent classes whose value is defined
 *        by this class.
 * \param item (out) The resulting item.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::read_removed_field
( item_class& item, const wxXmlNode* node ) const
{
  const std::string val = wx_to_std_string(node->GetNodeContent());

  if ( !item.has_field(val) )
    throw xml::bad_value
      ( '\'' + val + "' is not a field of the parent classes of '"
        + item.get_class_name() + "'." );

  item.add_removed_field( val );
} // item_class_xml_parser::read_removed_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the description of the item.
 * \param item (out) The resulting item.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::read_description
( item_class& item, const wxXmlNode* node ) const
{
  std::string desc( wx_to_std_string(node->GetNodeContent()) );

  claw::text::replace( desc, std::string("\t\n"), std::string(" ") );
  claw::text::squeeze( desc, " " );
  claw::text::trim( desc, " " );

  item.set_description
    ( wx_to_std_string(wxGetTranslation(std_to_wx_string(desc))) );
} // item_class_xml_parser::read_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the definition of a field of the item.
 * \param item (out) The resulting item.
 * \param node The node to explore.
 */
void bf::item_class_xml_parser::read_field_type
( item_class& item, const wxXmlNode* node ) const
{
  wxString val;
  std::string name;

  if ( !node->GetPropVal( wxT("name"), &val ) )
    throw xml::missing_property("name");

  name = wx_to_std_string(val);

  if ( !node->GetPropVal( wxT("type"), &val ) )
    throw xml::missing_property("type");

  type_field* field;

  if ( val == wxT("integer") )
    field = add_integer_field( name, node->GetChildren() );
  else if ( val == wxT("u_integer") )
    field = add_u_integer_field( name, node->GetChildren() );
  else if ( val == wxT("real") )
    field = add_real_field( name, node->GetChildren() );
  else if ( val == wxT("string") )
    field = add_string_field( name, node->GetChildren() );
  else if ( val == wxT("boolean") )
    field = add_boolean_field( name, node->GetChildren() );
  else if ( val == wxT("sprite") )
    field = add_sprite_field( name, node->GetChildren() );
  else if ( val == wxT("animation") )
    field = add_animation_field( name, node->GetChildren() );
  else if ( val == wxT("item_reference") )
    field = add_item_reference_field( name, node->GetChildren() );
  else if ( val == wxT("font") )
    field = add_font_field( name, node->GetChildren() );
  else if ( val == wxT("color") )
    field = add_color_field( name, node->GetChildren() );
  else if ( val == wxT("sample") )
    field = add_sample_field( name, node->GetChildren() );
  else
    throw xml::bad_value( wx_to_std_string(val) );

  field->set_required
    ( node->GetPropVal( wxT("required"), wxT("false") ) == wxT("true") );
  field->set_is_list
    ( node->GetPropVal( wxT("list"), wxT("false") ) == wxT("true") );

  item.add_field(name, *field);
  delete field;
} // item_class_xml_parser::read_field_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "integer" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_integer_field
( const std::string& name, const wxXmlNode* node ) const
{
  return
    create_field<int, true, true>(name, type_field::integer_field_type, node);
} // item_class_xml_parser::add_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "u_integer" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_u_integer_field
( const std::string& name, const wxXmlNode* node ) const
{
  return create_field<unsigned int, true, true>
    (name, type_field::u_integer_field_type, node);
} // item_class_xml_parser::add_u_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "real" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_real_field
( const std::string& name, const wxXmlNode* node ) const
{
  return
    create_field<double, true, true>(name, type_field::real_field_type, node);
} // item_class_xml_parser::add_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "string" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_string_field
( const std::string& name, const wxXmlNode* node ) const
{
  return
    create_field<int, true, false>(name, type_field::string_field_type, node);
} // item_class_xml_parser::add_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "boolean" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_boolean_field
( const std::string& name, const wxXmlNode* node ) const
{
  return
    create_field<int, false, false>(name, type_field::boolean_field_type, node);
} // item_class_xml_parser::add_boolean_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "sprite" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_sprite_field
( const std::string& name, const wxXmlNode* node ) const
{
  return
    create_field<int, false, false>(name, type_field::sprite_field_type, node);
} // item_class_xml_parser::add_sprite_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "animation" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_animation_field
( const std::string& name, const wxXmlNode* node ) const
{
  return create_field<int, false, false>
    (name, type_field::animation_field_type, node);
} // item_class_xml_parser::add_animation_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "item_reference" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_item_reference_field
( const std::string& name, const wxXmlNode* node ) const
{
  return create_field<int, true, false>
    (name, type_field::item_reference_field_type, node);
} // item_class_xml_parser::add_item_reference_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "font" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_font_field
( const std::string& name, const wxXmlNode* node ) const
{
  return create_field<int, false, false>
    (name, type_field::font_field_type, node);
} // item_class_xml_parser::add_font_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "color" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_color_field
( const std::string& name, const wxXmlNode* node ) const
{
  return create_field<int, false, false>
    (name, type_field::color_field_type, node);
} // item_class_xml_parser::add_color_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field of type "sample" to the item.
 * \param name The name of the field.
 * \param node The node to explore.
 */
bf::type_field* bf::item_class_xml_parser::add_sample_field
( const std::string& name, const wxXmlNode* node ) const
{
  return create_field<int, false, false>
    (name, type_field::sample_field_type, node);
} // item_class_xml_parser::add_sample_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the "field" property of a node "after".
 * \param node The node to explore.
 */
std::string
bf::item_class_xml_parser::read_after( const wxXmlNode* node ) const
{
  wxString val;

  if ( !node->GetPropVal( wxT("field"), &val ) )
    throw xml::missing_property("field");

  return wx_to_std_string(val);
} // item_class_xml_parser::read_after()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the elements of a set of values.
 * \param node The node to explore.
 * \param set The resulting set.
 */
void bf::item_class_xml_parser::read_set
( const wxXmlNode* node, std::list<std::string>& set ) const
{
  for ( node=node->GetChildren(); node!=NULL; node = node->GetNext() )
    if ( node->GetName() == wxT("element") )
      {
        wxString val;

        if ( !node->GetPropVal( wxT("value"), &val ) )
          throw xml::missing_property("value");

        set.push_back( wx_to_std_string(val) );
      }
    else
      throw xml::bad_node( wx_to_std_string(node->GetName()) );
} // item_class_xml_parser::read_set()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a field for the item.
 * \param name The name of the field.
 * \param ft The type of the field.
 * \param node The node to explore.
 *
 * \b Template \b parameters
 * - \a T The type of the bounds of the interval. Ignored if
 *   \a RangeAvailable == false.
 * - \a SetAvailable Tell if the field takes its value in a set of values.
 * - \a RangeAvailable Tell if the field takes its value in an interval.
 */
template<typename T, bool SetAvailable, bool RangeAvailable>
bf::type_field* bf::item_class_xml_parser::create_field
( const std::string& name, type_field::field_type ft,
  const wxXmlNode* node ) const
{
  type_field* result(NULL);
  std::list<std::string> prec;
  std::string desc;
  std::string default_value;

  for ( ; node!=NULL; node = node->GetNext() )
    if ( node->GetName() == wxT("after") )
      prec.push_front( read_after(node) );
    else if ( node->GetName() == wxT("description") )
      desc = wx_to_std_string( node->GetNodeContent() );
    else if ( node->GetName() == wxT("default_value") )
      default_value = wx_to_std_string( node->GetNodeContent().Trim() );
    else if ( result == NULL )
      {
        if ( node->GetName() == wxT("set") )
          {
            if ( SetAvailable )
              {
                std::list<std::string> set;
                read_set( node, set );
                result = new type_field_set( name, ft, set );
              }
            else
              claw::logger << claw::log_warning << "Ignored node '"
                           << wx_to_std_string(node->GetName()) << "' " << name
                           << "\n" << wx_to_std_string(node->GetNodeContent())
                           << std::endl;
          }
        else if ( node->GetName() == wxT("interval") )
          {
            if ( RangeAvailable )
              {
                std::pair<T, T> range = read_interval<T>( node );
                result =
                  new type_field_interval<T>( name, range.first, range.second );
              }
            else
              claw::logger << claw::log_warning << "Ignored node '"
                           << wx_to_std_string(node->GetName()) << "' " << name
                           << "\n" << wx_to_std_string(node->GetNodeContent())
                           << std::endl;
          }
        else if ( node->GetName() != wxT("comment") )
          claw::logger << claw::log_warning << "Ignored node '"
                       << wx_to_std_string(node->GetName()) << "' " << name
                       << "\n" << wx_to_std_string(node->GetNodeContent())
                       << std::endl;
      }
    else if ( node->GetName() != wxT("comment") )
      claw::logger << claw::log_warning << "Ignored node '"
                   << wx_to_std_string(node->GetName()) << "' " << name << "\n"
                   << wx_to_std_string(node->GetNodeContent()) << std::endl;

  if ( result == NULL )
    result = new type_field( name, ft );

  for ( std::size_t p=desc.find_first_of("\t\n"); p!=std::string::npos;
        p=desc.find_first_of("\t\n", p) )
    desc[p] = ' ';

  claw::text::replace( desc, std::string("\t\n"), std::string(" ") );
  claw::text::squeeze( desc, " " );
  claw::text::trim( desc, " " );

  result->set_preceding( prec );
  result->set_description
    ( wx_to_std_string(wxGetTranslation(std_to_wx_string(desc))) );
  result->set_default_value( default_value );

  return result;
} // item_class_xml_parser::create_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the bounds of a node "interval".
 * \param node The node to explore.
 */
template<typename T>
std::pair<T, T>
bf::item_class_xml_parser::read_interval( const wxXmlNode* node ) const
{
  std::pair<T, T> result;
  wxString val;
  std::istringstream iss;

  if ( !node->GetPropVal( wxT("from"), &val ) )
    result.first = std::numeric_limits<T>::min();
  else
    {
      iss.str( wx_to_std_string(val) );

      if ( !(iss >> result.first) )
        throw xml::bad_value( wx_to_std_string(val) );
    }

  if ( !node->GetPropVal( wxT("to"), &val ) )
    result.second = std::numeric_limits<T>::max();
  else
    {
      iss.clear();
      iss.str( wx_to_std_string(val) );

      if ( !(iss >> result.second) )
        throw xml::bad_value( wx_to_std_string(val) );
    }

  return result;
} // item_class_xml_parser::read_interval()
