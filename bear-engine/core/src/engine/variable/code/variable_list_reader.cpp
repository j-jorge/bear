/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::variable_list_reader class.
 * \author Julien Jorge
 */
#include "engine/variable/variable_list_reader.hpp"

#include "engine/variable/type_to_string.hpp"

#include <boost/bind.hpp>
#include <boost/spirit/include/classic.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Read a set of variable assignment from a stream.
 * \param iss The stream from which the assignments are read.
 * \param output The var_map in which the variables are inserted.
 */
void bear::engine::variable_list_reader::operator()
  ( std::istream& iss, var_map& output ) const
{
  std::stringstream data;
  data << iss.rdbuf();

  typedef boost::spirit::classic::phrase_scanner_t scanner_type;
  typedef boost::spirit::classic::parse_info<const char*> parse_info;
  typedef boost::spirit::classic::rule<scanner_type> rule_type;

  rule_type identifier =
    boost::spirit::classic::lexeme_d
    [ (boost::spirit::classic::alpha_p | '_')
      >> *(boost::spirit::classic::alnum_p | '_') ];
  rule_type string =
    boost::spirit::classic::lexeme_d
    [ *(boost::spirit::classic::strlit<>("\\\"")
        | (boost::spirit::classic::anychar_p - '"') )
    ];

  std::string type;
  std::string name;
  std::string value;

  rule_type assignment =
    identifier[boost::spirit::classic::assign_a(type)]
    >> boost::spirit::classic::lexeme_d
       [ boost::spirit::classic::ch_p('"')
         >> string[boost::spirit::classic::assign_a(name)]
         >> boost::spirit::classic::ch_p('"')
       ]
    >> boost::spirit::classic::ch_p('=')
    >> boost::spirit::classic::lexeme_d
       [ boost::spirit::classic::ch_p('"')
         >> string[boost::spirit::classic::assign_a(value)]
         >> boost::spirit::classic::ch_p('"')
       ]
    >> boost::spirit::classic::ch_p(';')
    [boost::bind
     (&variable_list_reader::apply, this, boost::ref(output), boost::ref(type),
      boost::ref(name), boost::ref(value))]
    ;

  parse_info result =
    boost::spirit::classic::parse
    ( data.str().c_str(), *assignment >> boost::spirit::classic::end_p,
      boost::spirit::classic::space_p );

  if ( !result.full )
    claw::logger << claw::log_error
                 << "Failed to read variable assignments from the stream."
                 << " Last variable is '" << name << "'." << std::endl;
} // variable_list_reader::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the game variable in a var_map.
 * \param v The var_map in which the variable is added.
 * \param type The type of the variable.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
void bear::engine::variable_list_reader::apply
( var_map& v, const std::string& type, const std::string& name,
  const std::string& value ) const
{
  if ( type == "bool" )
    add_variable<bool>( v, name, value );
  else if ( type == "int" )
    add_variable<int>( v, name, value );
  else if ( type == "uint" )
    add_variable<unsigned int>( v, name, value );
  else if ( type == "real" )
    add_variable<double>( v, name, value );
  else if ( type == "string" )
    add_string_variable( v, name, value );
  else
    claw::logger << claw::log_error << "Unknown type '" << type
                 << "' (assigning variable '" << name << "')." << std::endl;
} // variable_list_reader::apply()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the escaped sequences from a string.
 * \param s The string to process.
 */
std::string bear::engine::variable_list_reader::unescape
( const std::string& s ) const
{
  bool escape = false;
  std::string result;
  result.reserve(s.size());

  for ( std::size_t i=0; i!=s.length(); ++i )
    if ( !escape && (s[i] == '\\') )
      escape = true;
    else
      {
        result += s[i];
        escape = false;
      }

  return result;
} // variable_list_reader::unescape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the game variable of type string in a var_map.
 * \param v The var_map in which the variable is added.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
void bear::engine::variable_list_reader::add_string_variable
( var_map& v, const std::string& name, const std::string& value ) const
{
  v.set<std::string>( unescape(name), unescape(value) );
} // variable_list_reader::add_string_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the game variable in a var_map.
 * \param v The var_map in which the variable is added.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
template<typename T>
void bear::engine::variable_list_reader::add_variable
( var_map& v, const std::string& name, const std::string& value ) const
{
  std::istringstream iss(value);
  T val;
  iss >> val;

  if ( iss.eof() )
    v.set<T>( unescape(name), val );
  else
    claw::logger << claw::log_error << '\'' << value << "' is not of type '"
                 << type_to_string<T>::value << "' (assigning variable '"
                 << name << "')." << std::endl;
} // variable_list_reader::add_variable()
