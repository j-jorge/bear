/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::sdc::node_parser class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an error on the log stream.
 * \param first Begining of the incorrect input.
 * \param last End of the incorrect input.
 * \param msg Informative message.
 */
void sdc::node_parser::error
( data_iterator first, data_iterator last, const std::string& msg ) const
{
  boost::spirit::classic::file_position pos = first.get_position();

  std::cerr << pos.file << ": " << pos.line << ": "
            << pos.column << ": " << msg << std::endl;
} // node_parser::error()
