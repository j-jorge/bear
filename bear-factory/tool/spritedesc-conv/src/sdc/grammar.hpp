/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The grammar of a .spritedesc file.
 * \author Julien Jorge
 */
#ifndef __SDC_GRAMMAR_HPP__
#define __SDC_GRAMMAR_HPP__

#include <boost/spirit/include/classic.hpp>

namespace sdc
{
  /**
   * \brief The grammar of a .spritedesc file.
   * \author Julien Jorge
   */
  class grammar:
    public boost::spirit::classic::grammar<grammar>
  {
  public:
    static const int id_file = 10;
    static const int id_sprite_sheet = 15;
    static const int id_margin = 17;
    static const int id_xcf_declaration = 20;
    static const int id_sprite_declaration = 40;
    static const int id_layer_to_sprite = 41;
    static const int id_layer = 45;
    static const int id_string = 50;
    static const int id_glob = 55;
    static const int id_exclude = 57;
    static const int id_autosize = 60;
    static const int id_image_size = 70;
    static const int id_layer_size = 80;

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
      definition( const grammar& /*self*/ );

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

      /** \brief Rule matching a sprite sheet. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_sprite_sheet> > m_sprite_sheet;

      /** \brief Rule matching a margin. */
      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_margin> > m_margin;

      /** \brief Rule matching the heuristic of the placement order. */
      boost::spirit::classic::rule< ScannerT > m_order;

      /** \brief Rule matching the use of an xcf file. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_xcf_declaration> >
      m_xcf_declaration;

      /** \brief Rule matching the description of a sprite. */
      boost::spirit::classic::rule<ScannerT> m_sprite_description;

      /** \brief Rule matching the declaration of a sprite. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_sprite_declaration> >
      m_sprite_declaration;

      /** \brief Rule converting each layer into a sprite. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_layer_to_sprite> >
      m_layer_to_sprite;

      /** \brief Rule matching the description of a layer. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_layer> > m_layer;

      /** \brief Rule matching the mask of a sprite. */
      boost::spirit::classic::rule<ScannerT> m_mask;

      /** \brief Rule matching a list of layers. */
      boost::spirit::classic::rule<ScannerT> m_layer_list;

      /** \brief Rule matching the size of a sprite. */
      boost::spirit::classic::rule<ScannerT> m_sprite_size;

      /** \brief Rule matching an autosize declaration. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_autosize> > m_autosize;

      /** \brief Rule matching a declaration of the same size than the image. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_image_size> > m_image_size;

      /** \brief Rule matching a declaration of the same size than a layer. */
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_layer_size> > m_layer_size;

      /** \brief Rule matching an identifier. */
      boost::spirit::classic::rule<ScannerT> m_identifier;

      /** \brief Rule matching a C string. */
      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_string> > m_string;

      /** \brief Rule matching a globalization command. */
      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_glob> > m_glob;

      /** \brief Rule matching an exclusion command. */
      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_exclude> > m_exclude;

      /** \brief Sprite sheet expected. */
      generic_error m_error_sprite_sheet;

      /** \brief Size expected. */
      generic_error m_error_autosize;

      /** \brief Identifier expected. */
      generic_error m_error_identifier;

      /** \brief String expected. */
      generic_error m_error_string;

      /** \brief Not terminated string. */
      generic_error m_error_not_terminated_string;

      /** \brief Size expected. */
      generic_error m_error_size;

      /** \brief Dot expected. */
      char_error m_error_dot;

      /** \brief Semicolon expected. */
      char_error m_error_semicolon;

    }; // class definition
  }; // class grammar
} // namespace sdc

#include "impl/grammar.tpp"

#endif // __SDC_GRAMMAR_HPP__
