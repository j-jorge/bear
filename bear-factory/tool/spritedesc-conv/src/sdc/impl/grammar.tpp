/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implemenation of the sdc::grammar class.
 * \author Julien Jorge.
 */

#include <boost/spirit/include/classic_ast.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template <typename ScannerT>
sdc::grammar::definition<ScannerT>::
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
sdc::grammar::definition<ScannerT>::
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
sdc::grammar::definition<ScannerT>::
error_report_parser::operator()
  (const LocalScanner& scan, result_t& /*result*/) const
{
  boost::spirit::classic::file_position fpos = scan.first.get_position();

  std::cerr << fpos.file << ": " << fpos.line << ": "
            << fpos.column << ": " << m_msg << std::endl;

  return -1;
} // definition::operator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
template <typename ScannerT>
sdc::grammar::definition<ScannerT>::
char_error_report_parser::char_error_report_parser()
{

} // definition::char_error_report_parser::char_error_report_parser()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The missing character.
 */
template <typename ScannerT>
sdc::grammar::definition<ScannerT>::
char_error_report_parser::char_error_report_parser( char c )
  : error_report_parser( std::string("Missing character '") + c + "'." )
{

} // definition::char_error_report_parser::char_error_report_parser()





/*----------------------------------------------------------------------------*/
/**
 * \brief Definition of the rules.
 * \param self The concerned sdc::grammar instance.
 */
template<typename ScannerT>
sdc::grammar::definition<ScannerT>::definition( const grammar& self )
{
  initialize_error_parsers();

  // Whole file
  m_file =
    *m_sprite_sheet;

  m_sprite_sheet =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("sprite_sheet") ]
    >> ( m_string | m_error_sprite_sheet )
    >> ( ( boost::spirit::classic::uint_p
           >> boost::spirit::classic::no_node_d
           [ boost::spirit::classic::ch_p('x') ]
           >> boost::spirit::classic::uint_p )
         | m_error_size )
    >> !m_margin
    >> !m_order
    >> *m_xcf_declaration >> *m_sprite_description;

  m_margin =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("margin") ]
    >> boost::spirit::classic::uint_p
    ;

  m_order =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("order") ]
    >> (m_string | m_error_string)
    ;

  m_xcf_declaration =
    m_identifier >> (m_string | m_error_string);

  m_sprite_description =
    m_sprite_declaration | m_layer_to_sprite;

  m_layer_to_sprite =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("layer_to_sprite") ]
    >> ( m_sprite_size | m_error_autosize )
    >> boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("with") ]
    >> m_identifier
    >> *m_string
    >> ( boost::spirit::classic::no_node_d
         [ boost::spirit::classic::ch_p(';') ] | m_error_semicolon )
    ;

  m_sprite_declaration =
    m_string
    >> ( m_sprite_size | m_error_autosize )
    >> boost::spirit::classic::no_node_d
       [ boost::spirit::classic::strlit<>("with") ]
    >> m_identifier
    >> m_layer_list
    >> !m_mask
    >> ( boost::spirit::classic::no_node_d
         [ boost::spirit::classic::ch_p(';') ] | m_error_semicolon )
    ;

  m_mask =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("mask") ]
    >> m_layer_list;
    ;

  m_layer_list = *(m_string | m_glob | m_exclude);

  m_glob =
    boost::spirit::classic::strlit<>("glob")
    >> m_string;
    ;

  m_exclude = 
    boost::spirit::classic::strlit<>("exclude")
    >> (m_string | m_glob);

  m_sprite_size =
    m_autosize | m_image_size | m_layer_size;

  m_image_size =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("image_size")
      >> boost::spirit::classic::ch_p('*') ]
    >> boost::spirit::classic::real_p
    ;

  m_autosize =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("autosize")
      >> boost::spirit::classic::ch_p('*') ]
    >> boost::spirit::classic::real_p
    ;

  m_layer_size =
    m_string
    >> boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('*') ]
    >> boost::spirit::classic::real_p
    ;

  m_identifier =
    boost::spirit::classic::no_node_d[ *boost::spirit::classic::blank_p ]
    >>    boost::spirit::classic::token_node_d
    [ boost::spirit::classic::lexeme_d[ (boost::spirit::classic::alpha_p | '_')
                               >> *(boost::spirit::classic::alnum_p | '_') ]
      ];

  m_string =
    boost::spirit::classic::lexeme_d
    [ boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('"') ]
      >> boost::spirit::classic::token_node_d
      [ *(boost::spirit::classic::strlit<>("\\\"")
          | (boost::spirit::classic::anychar_p - '"') ) ]
      >> boost::spirit::classic::no_node_d
      [ ( '"' | m_error_not_terminated_string ) ]
    ];
} // grammar::definition::definition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the error parsers
 */
template<typename ScannerT>
void sdc::grammar::definition<ScannerT>::initialize_error_parsers()
{
  m_error_autosize =
    error_report_parser( "Size expected, like: autosize * 0.5." );
  m_error_identifier = error_report_parser( "Identifier expected." );
  m_error_string = error_report_parser( "String expected." );
  m_error_not_terminated_string =
    error_report_parser( "Not terminated string." );

  m_error_sprite_sheet = error_report_parser( "sprite_sheet expected" );
  m_error_size = error_report_parser( "Size expected (width x height)." );

  m_error_semicolon = char_error_report_parser( ';' );
} // grammar::definition::initialize_error_parsers()
