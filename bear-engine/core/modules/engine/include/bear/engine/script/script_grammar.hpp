/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The grammar of a script file.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SCRIPT_GRAMMAR_HPP__
#define __ENGINE_SCRIPT_GRAMMAR_HPP__

#include "bear/engine/class_export.hpp"
#include <boost/spirit/include/classic.hpp>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The grammar of a script file.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT script_grammar:
      public boost::spirit::classic::grammar<script_grammar>
    {
    public:
      static const int id_file = 10;
      static const int id_call_entry = 20;
      static const int id_argument_list = 40;
      static const int id_argument = 41;
      static const int id_string = 50;
      static const int id_call = 60;
      static const int id_call_group = 70;


    public:
      /**
       * \brief Grammar definition.
       */
      template<typename ScannerT>
      class definition
      {
      private:
        /**
         * \brief Report any error.
         */
        class error_report_parser
        {
        public:
          typedef boost::spirit::classic::nil_t result_t;

        public:
          error_report_parser();
          error_report_parser( const std::string msg );

          template<typename LocalScanner>
          int operator()(const LocalScanner& scan, result_t& /*result*/) const;

        private:
          /** \brief A text explaining the error. */
          std::string m_msg;

        }; // class error_report_parser

        /**
         * \brief Report a "missing character" error.
         */
        class char_error_report_parser:
          public error_report_parser
        {
        public:
          char_error_report_parser();
          char_error_report_parser( char c );
        }; // class char_error_report_parser

        typedef
        boost::spirit::classic::functor_parser<error_report_parser>
        generic_error;
        typedef
        boost::spirit::classic::functor_parser<char_error_report_parser>
        char_error;

      public:
        definition( const script_grammar& /*self*/ );

        /**
         * \brief Get the axiom rule.
         */
        const boost::spirit::classic::rule
        < ScannerT, boost::spirit::classic::parser_tag<id_file> >& start() const
        {
          return m_file;
        }

      private:
        void initialize_error_parsers();

      private:
        /** \brief Rule matching the full file. */
        boost::spirit::classic::rule
        < ScannerT,
          boost::spirit::classic::parser_tag<id_file> > m_file;

        /** \brief Rule matching one or more calls after a date. */
        boost::spirit::classic::rule
        < ScannerT,
          boost::spirit::classic::parser_tag<id_call_entry> > m_call_entry;

        /** \brief Rule matching a single call. */
        boost::spirit::classic::rule
        < ScannerT,
          boost::spirit::classic::parser_tag<id_call> > m_call;

        /** \brief Rule matching several calls. */
        boost::spirit::classic::rule
        < ScannerT,
          boost::spirit::classic::parser_tag<id_call_group> > m_call_group;

        /** \brief Rule matching an identifier. */
        boost::spirit::classic::rule<ScannerT> m_identifier;

        /** \brief Rule matching the date of the call. */
        boost::spirit::classic::rule<ScannerT> m_date;

        /** \brief Rule matching the list of the arguments values of a method
            call. */
        boost::spirit::classic::rule
        < ScannerT,
          boost::spirit::classic::parser_tag<id_argument_list> >
        m_argument_list;

        /** \brief Rule matching the value of an argument of a method call. */
        boost::spirit::classic::rule
        < ScannerT, boost::spirit::classic::parser_tag<id_argument> >
        m_argument;

        /** \brief Rule matching a C string. */
        boost::spirit::classic::rule
        < ScannerT, boost::spirit::classic::parser_tag<id_string> > m_string;

        /**
         * \brief Syntaxic errors.
         * \{
         */

        /** \brief Date expected. */
        generic_error m_error_date;

        /** \brief Identifier expected. */
        generic_error m_error_identifier;

        /** \brief Not terminated string. */
        generic_error m_error_not_terminated_string;

        /** \brief Missing argument. */
        generic_error m_error_missing_argument;

        /** \brief Dot expected. */
        char_error m_error_dot;

        /** \brief Comma expected. */
        char_error m_error_comma;

        /** \brief Semicolon expected. */
        char_error m_error_semicolon;

        /** \brief Right parenthesis expected. */
        char_error m_error_right_parenthesis;

        /** \brief Left parenthesis expected. */
        char_error m_error_left_parenthesis;

        /** \brief Right brace expected. */
        char_error m_error_right_brace;

        /** \brief Left brace expected. */
        char_error m_error_left_brace;

      }; // class definition
    }; // class script_grammar
  } // namespace engine
} // namespace bear

#include "bear/engine/script/impl/script_grammar.tpp"

#endif // __ENGINE_SCRIPT_GRAMMAR_HPP__
