/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A parser for simple arithmetic expressions.
 * \author Julien Jorge
 */
#ifndef __BF_ARITHMETIC_GRAMMAR_HPP__
#define __BF_ARITHMETIC_GRAMMAR_HPP__

#include <boost/spirit/include/classic.hpp>
#include <string>

namespace bf
{
  /**
   * \brief This grammar matches the arithmetic expressions.
   * \author Julien Jorge
   */
  class arithmetic_grammar:
    public boost::spirit::classic::grammar<arithmetic_grammar>
  {
  public:
    /** \brief The identifier of the rule that matches an arithmetic
        expression. */
    static const int id_expression = 10;

    /** \brief The identifier of the rule that matches a term in an
        expression. */
    static const int id_term = 20;

    /** \brief The identifier of the rule that matches a factor in an
        expression. */
    static const int id_factor = 30;

    /** \brief The identifier of the rule that matches a real number. */
    static const int id_real = 40;
 
  public:
    /**
     * \brief This class contains the rules of the grammar.
     */
    template<typename Scanner>
    class definition
    {
    public:
      definition( const arithmetic_grammar& self );
 
      const boost::spirit::classic::rule
      < Scanner, boost::spirit::classic::parser_tag<id_expression> >&
      start() const;
    
    private:
      /** \brief The rule that matches an arithmetic expression. */
      boost::spirit::classic::rule
      < Scanner, boost::spirit::classic::parser_tag<id_expression> >
      m_expression;
      
      /** \brief The rule that matches a term in an expression. */
      boost::spirit::classic::rule
      < Scanner, boost::spirit::classic::parser_tag<id_term> > m_term;
      
      /** \brief The rule that matches a factor in an expression. */
      boost::spirit::classic::rule
      < Scanner, boost::spirit::classic::parser_tag<id_factor> > m_factor;
      
      /** \brief The rule that matches a real number. */
      boost::spirit::classic::rule
      < Scanner, boost::spirit::classic::parser_tag<id_real> > m_real;

    }; // class definition

  }; // class arithmetic_grammar
} // namespace bf

#include "bf/impl/arithmetic_grammar.tpp"

#endif // __BF_ARITHMETIC_GRAMMAR_HPP__
