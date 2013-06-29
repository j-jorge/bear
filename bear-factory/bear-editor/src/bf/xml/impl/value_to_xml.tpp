/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of the bf::xml::value_to_xml
 *        class.
 * \author Julien Jorge
 */
#include "bf/xml/util.hpp"
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a node for a simple value.
 * \param os The stream in which we save the value.
 * \param node_name The name of the xml node.
 * \param v The value to write.
 */
template<typename Type>
void bf::xml::value_to_xml<Type>::write
( std::ostream& os, const std::string& node_name, const Type& v )
{
  std::ostringstream oss;
  oss << v.get_value();

  os << "<" << util::replace_special_characters(node_name) << " value='"
     << util::replace_special_characters(oss.str()) << "'/>\n";
} // value_to_xml::write()
