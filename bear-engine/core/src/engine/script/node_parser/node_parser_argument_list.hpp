/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "argument_list" rule.
 * \author Julien Jorge
 */
#ifndef __ENGINE_NODE_PARSER_ARGUMENT_LIST_HPP__
#define __ENGINE_NODE_PARSER_ARGUMENT_LIST_HPP__

#include "engine/script/node_parser/node_parser.hpp"

#include "engine/class_export.hpp"
#include "engine/i18n/translator.hpp"

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
