/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to help building trees (wxTreeCtrl).
 * \author Julien Jorge
 */
#ifndef __BF_TREE_BUILDER_HPP__
#define __BF_TREE_BUILDER_HPP__

#include "bf/libeditor_export.hpp"

#include <wx/treectrl.h>
#include <claw/tree.hpp>

#include <string>

namespace bf
{
  /**
   * \brief A class to help building trees (wxTreeCtrl).
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT tree_builder
  {
  private:
    typedef claw::tree<std::string> tree_type;

  public:
    void add_entries( const std::string& entries, char sep );
    void create_wxTree( wxTreeCtrl& result ) const;

  private:
    void create_wxTree
    ( wxTreeCtrl& result, wxTreeItemId parent, const tree_type& t ) const;

    void
    insert_entries( tree_type& node, std::list<std::string>& entries ) const;

  private:
    /** \brief the tree currently built. */
    tree_type m_tree;

  }; // class tree_builder
} // namespace bf

#endif // __BF_TREE_BUILDER_HPP__
