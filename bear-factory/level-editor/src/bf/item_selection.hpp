/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A selection of items.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_SELECTION_HPP__
#define __BF_ITEM_SELECTION_HPP__

#include <set>

namespace bf
{
  class item_instance;

  /**
   * \brief A selection of items.
   * \author Julien Jorge
   */
  class item_selection
  {
  public:
    /** \brief Iterator on the selected items. */
    typedef std::set<item_instance*>::const_iterator const_iterator;

  public:
    item_selection();

    void insert( item_instance* item, bool main_selection = false );
    void insert( const item_selection& s );

    void remove( item_instance* item );
    void remove( const item_selection& s );

    bool is_selected( item_instance const* item ) const;
    bool is_main_selection( item_instance const* item ) const;

    void clear();
    bool empty() const;

    item_instance* get_main_selection() const;

    const_iterator begin() const;
    const_iterator end() const;

    bool same_group_than( const item_selection& that ) const;

  private:
    /** \brief The main selection. */
    item_instance* m_item;

    /** \brief All the selected items. */
    std::set<item_instance*> m_group;

  }; // class item_selection
} // namespace bf

#endif // __BF_ITEM_SELECTION_HPP__
