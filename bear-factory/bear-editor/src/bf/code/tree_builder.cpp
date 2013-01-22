/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::tree_builder class.
 * \author Julien Jorge
 */
#include "bf/tree_builder.hpp"

#include "bf/wx_facilities.hpp"

#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a hierarchy in the tree.
 * \param entries The elements in the hierarchy.
 * \param sep The character separing the entries in \a entries.
 */
void bf::tree_builder::add_entries( const std::string& entries, char sep )
{
  std::list<std::string> ent;
  claw::text::split( ent, entries, sep );
  insert_entries( m_tree, ent );
} // tree_builder::add_entries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a wxTreeCtrl from the tree.
 * \param result The resulting tree.
 */
void bf::tree_builder::create_wxTree( wxTreeCtrl& result ) const
{
  tree_type::const_iterator it;

  for (it=m_tree.begin(); it!=m_tree.end(); ++it )
    create_wxTree( result, result.GetRootItem(), *it );

  result.SortChildren(result.GetRootItem());
} // tree_builder::create_wxTree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the subtrees of the wxTreeCtrl.
 * \param result The resulting tree.
 * \param parent The identifier of the parent node.
 * \param t The tree to convert.
 */
void bf::tree_builder::create_wxTree
( wxTreeCtrl& result, wxTreeItemId parent, const tree_type& t ) const
{
  wxTreeItemId node = result.AppendItem( parent, std_to_wx_string(t.value) );

  if ( !t.is_leaf() )
    {
      result.SetItemBackgroundColour(node, *wxLIGHT_GREY);
      result.SetItemBold(node, true);

      tree_type::const_iterator it;

      for (it=t.begin(); it!=t.end(); ++it )
        create_wxTree( result, node, *it );
    }

  result.SortChildren(node);
} // tree_builder::create_wxTree()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a hierarchy in the tree and avoid duplicates.
 * \param node (out) The node in which the hierarchy is inserted.
 * \param entries The elements in the hierarchy. The last element is the leaf.
 */
void bf::tree_builder::insert_entries
( tree_type& node, std::list<std::string>& entries ) const
{
  if ( !entries.empty() )
    {
      std::string ent( entries.front() );
      entries.pop_front();

      tree_type::iterator it = node.find( ent );

      if ( it == node.end() )
        insert_entries( node.add_child(ent), entries );
      else
        insert_entries( *it, entries );
    }
} // tree_builder::insert_entries()
