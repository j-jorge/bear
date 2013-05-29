/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the bf::arithmetic_grammar
 *        class.
 * \author Julien Jorge
 */

#include <boost/spirit/include/classic_ast.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the definition of the grammar.
 * \param self The grammar of which we are the definition.
 */
template<typename Scanner>
bf::arithmetic_grammar::definition<Scanner>::definition
( const arithmetic_grammar& self )
{
  m_expression =
    m_term
    >> *( ( boost::spirit::classic::root_node_d
            [ boost::spirit::classic::ch_p('+') ]
            | boost::spirit::classic::root_node_d
            [ boost::spirit::classic::ch_p('-') ] )
          >> m_term )
    ;
  
  m_term =
    m_factor
    >> *( ( boost::spirit::classic::root_node_d
            [ boost::spirit::classic::ch_p('*') ]
            | boost::spirit::classic::root_node_d
            [ boost::spirit::classic::ch_p('/') ]
            )
          >> m_factor )
    ;

  m_factor =
    m_real
    | boost::spirit::classic::inner_node_d[ '(' >> m_expression >> ')' ];
       
  m_real =
    boost::spirit::classic::leaf_node_d[ boost::spirit::classic::real_p ];
} // definition::definition()
 
/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the main rule of the grammar.
 */
template<typename Scanner>
const boost::spirit::classic::rule
< Scanner,
  boost::spirit::classic::parser_tag<bf::arithmetic_grammar::id_expression>
>&
bf::arithmetic_grammar::definition<Scanner>::start() const
{
  return m_expression;
} // definition::start()
