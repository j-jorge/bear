/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A selection of marks.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_SELECTION_HPP__
#define __BF_MODEL_SELECTION_HPP__

#include <set>

namespace bf
{
  class mark;

  /**
   * \brief A selection of marks.
   * \author Julien Jorge
   */
  class model_selection
  {
  public:
    /** \brief Iterator on the selected marks. */
    typedef std::set<mark*> mark_set;
    
    /** \brief Iterator on the selected marks. */
    typedef mark_set::const_iterator const_iterator;

  public:
    model_selection();

    void insert( mark* mark, bool main_selection = false );
    void insert( const model_selection& s );

    void remove( mark* mark );
    void remove( const model_selection& s );

    bool is_selected( mark* mark ) const;
    bool is_main_selection( mark* mark ) const;

    void set_bounding_box_selection( bool s );
    bool bounding_box_is_selected() const;
    bool bounding_box_is_main_selection() const;

    void clear();

    bool empty() const;
    bool mark_empty() const;

    mark* get_main_mark_selection() const;

    const_iterator begin() const;
    const_iterator end() const;

  private:
    /** \brief The main selection. */
    mark* m_mark;

    /** \brief All the selected marks. */
    mark_set m_group;

    /** \brief Tell if the bounding box is selected. */
    bool m_bounding_box_is_selected;

  }; // class model_selection
} // namespace bf

#endif // __BF_MODEL_SELECTION_HPP__
