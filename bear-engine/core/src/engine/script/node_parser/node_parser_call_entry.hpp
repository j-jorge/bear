/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Compile the node of the "call_entry" rule.
 * \author Julien Jorge
 */
#ifndef __ENGINE_NODE_PARSER_CALL_ENTRY_HPP__
#define __ENGINE_NODE_PARSER_CALL_ENTRY_HPP__

#include "engine/class_export.hpp"

#include "engine/script/node_parser/node_parser.hpp"
#include "engine/i18n/translator.hpp"

namespace bear
{
  namespace engine
  {
    class call_sequence;

    /**
     * \brief Compile the node of the "call" rule.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT node_parser_call_entry:
      public node_parser
    {
    public:
      void parse_node
        ( call_sequence& seq, const tree_node& node,
          translator t ) const;

    }; // class node_parser_call_entry
  } // namespace engine
} // namespace bear

#endif // __ENGINE_NODE_PARSER_CALL_ENTRY_HPP__
