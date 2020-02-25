/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Compile the node of the "argument_list" rule.
 * \author Julien Jorge
 */
#ifndef __ENGINE_NODE_PARSER_ARGUMENT_LIST_HPP__
#define __ENGINE_NODE_PARSER_ARGUMENT_LIST_HPP__

#include "bear/engine/script/node_parser/node_parser.hpp"

#include "bear/engine/class_export.hpp"
#include "bear/engine/i18n/translator.hpp"

namespace bear
{
  namespace engine
  {
    class method_call;

    /**
     * \brief Compile the node of the "argument_list" rule.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT node_parser_argument_list:
      public node_parser
    {
    public:
      void parse_node
        ( method_call& call, const tree_node& node, translator t ) const;

    }; // class node_parser_argument_list
  } // namespace engine
} // namespace bear

#endif // __ENGINE_NODE_PARSER_ARGUMENT_LIST_HPP__
