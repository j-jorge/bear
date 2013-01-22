/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "argument" rule.
 * \author Julien Jorge
 */
#ifndef __ENGINE_NODE_PARSER_ARGUMENT_HPP__
#define __ENGINE_NODE_PARSER_ARGUMENT_HPP__

#include "engine/class_export.hpp"

#include "engine/script/node_parser/node_parser.hpp"
#include "engine/i18n/translator.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Compile the node of the "argument" rule.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT node_parser_argument:
      public node_parser
    {
    public:
      void parse_node
        ( std::string& val, const tree_node& node, translator t ) const;

    }; // class node_parser_argument
  } // namespace engine
} // namespace bear

#endif // __ENGINE_NODE_PARSER_ARGUMENT_HPP__
