/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::node_parser class.
 * \author Julien Jorge
 */
#include "bear/engine/script/node_parser/node_parser.hpp"

#include <claw/logger/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Write an error on the log stream.
 * \param first Begining of the incorrect input.
 * \param last End of the incorrect input.
 * \param msg Informative message.
 */
void bear::engine::node_parser::error
( data_iterator first, const std::string& msg ) const
{
  boost::spirit::classic::file_position pos = first.get_position();

  claw::logger << claw::log_error << pos.file << ": " << pos.line << ": "
               << pos.column << ": " << msg << std::endl;
} // node_parser::error()
