/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implemenation of the bear::engine::script_grammar class.
 * \author Julien Jorge.
 */

#include <boost/spirit/include/classic_ast.hpp>

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template <typename ScannerT>
bear::engine::script_grammar::definition<ScannerT>::
error_report_parser::error_report_parser()
{
  // nothing to do.
} // definition::error_report_parser::error_report_parser()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param msg A text explaining the error.
 */
template <typename ScannerT>
bear::engine::script_grammar::definition<ScannerT>::
error_report_parser::error_report_parser( const std::string msg )
  : m_msg(msg)
{

} // definition::error_report_parser::error_report_parser()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the message on standard error output.
 * \param scan The scanner detecting the error.
 */
template <typename ScannerT>
template <typename LocalScanner>
int
bear::engine::script_grammar::definition<ScannerT>::
error_report_parser::operator()
  (const LocalScanner& scan, result_t& /*result*/) const
{
  boost::spirit::classic::file_position fpos = scan.first.get_position();

  claw::logger << claw::log_error << fpos.file << ": " << fpos.line << ": "
               << fpos.column << ": " << m_msg << std::endl;

  return -1;
} // definition::operator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template <typename ScannerT>
bear::engine::script_grammar::definition<ScannerT>::
char_error_report_parser::char_error_report_parser()
{

} // definition::char_error_report_parser::char_error_report_parser()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The missing character.
 */
template <typename ScannerT>
bear::engine::script_grammar::definition<ScannerT>::
char_error_report_parser::char_error_report_parser( char c )
  : error_report_parser( std::string("Missing character '") + c + "'." )
{

} // definition::char_error_report_parser::char_error_report_parser()





/*----------------------------------------------------------------------------*/
/**
 * \brief Definition of the rules.
 * \param self The concerned bear::engine::script_grammar instance.
 */
template<typename ScannerT>
bear::engine::script_grammar::definition<ScannerT>::definition
( const script_grammar& self )
{
  initialize_error_parsers();

  // Whole file
  m_file = *m_call_entry;

  m_call_entry =
    m_date >> ( m_call | m_call_group );

  m_call =
    m_identifier
    >> ( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('.') ]
         | m_error_dot )
    >> ( m_identifier | m_error_identifier )
    >> ( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('(') ]
         | m_error_left_parenthesis )
    >> !m_argument_list
    >> ( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p(')') ]
         | m_error_right_parenthesis )
    >> ( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p(';') ]
         | m_error_semicolon )
    ;

  m_call_group =
    ( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('{') ]
      | m_error_left_brace )
    >> +m_call
    >> ( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('}') ]
         | m_error_right_brace )
    ;

  m_identifier =
    boost::spirit::classic::no_node_d[ *boost::spirit::classic::blank_p ]
    >>    boost::spirit::classic::token_node_d
    [ boost::spirit::classic::lexeme_d[ (boost::spirit::classic::alpha_p | '_')
                               >> *(boost::spirit::classic::alnum_p | '_') ]
      ];

  m_date = boost::spirit::classic::real_p;

  m_argument_list =
    boost::spirit::classic::list_p
    ( m_argument,
      ( boost::spirit::classic::no_node_d
        [ boost::spirit::classic::ch_p(',') ] ) );

  m_string =
    boost::spirit::classic::lexeme_d
    [ boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('"') ]
      >> boost::spirit::classic::token_node_d
      [ *(boost::spirit::classic::strlit<>("\\\"")
          | (boost::spirit::classic::anychar_p - '"') ) ]
      >> boost::spirit::classic::no_node_d
      [ ( '"' | m_error_not_terminated_string ) ]
    ];

  m_argument =
    m_string
    | m_identifier
    | boost::spirit::classic::real_p;
} // script_grammar::definition::definition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the error parsers
 */
template<typename ScannerT>
void
bear::engine::script_grammar::definition<ScannerT>::initialize_error_parsers()
{
  m_error_date = error_report_parser( "Date (real number) expected." );
  m_error_identifier = error_report_parser( "Identifier expected." );
  m_error_not_terminated_string =
    error_report_parser( "Not terminated string." );
  m_error_missing_argument = error_report_parser( "Missing argument." );

  m_error_dot = char_error_report_parser( '.' );
  m_error_comma = char_error_report_parser( ',' );
  m_error_semicolon = char_error_report_parser( ';' );
  m_error_right_parenthesis = char_error_report_parser( ')' );
  m_error_left_parenthesis = char_error_report_parser( '(' );
  m_error_right_brace = char_error_report_parser( '}' );
  m_error_left_brace = char_error_report_parser( '{' );
} // script_grammar::definition::initialize_error_parsers()
