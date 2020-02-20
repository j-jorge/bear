/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A structure that manages all the values that can be interpolated in a
 *        snapshot.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_TWEENER_HPP__
#define __BF_MODEL_TWEENER_HPP__

#include <claw/tween/tweener_group.hpp>

#include <vector>

namespace bf
{
  class action;
  class mark;
  class mark_placement;
  class snapshot;

  /**
   * \brief A structure that manages all the values that can be interpolated
   *        in a snapshot.
   * \author Julien Jorge
   */
  class model_snapshot_tweener
  {
  public:
    typedef std::vector<mark_placement>::const_iterator
    const_mark_placement_iterator;

  public:
    explicit model_snapshot_tweener( const snapshot& init );
    model_snapshot_tweener
    ( const snapshot& init, const snapshot& end,
      const action& init_action, const action& end_action,
      double d );

    mark_placement& get_mark_placement( std::size_t i );
    const mark_placement& get_mark_placement( std::size_t i ) const;

    const_mark_placement_iterator mark_placement_begin() const;
    const_mark_placement_iterator mark_placement_end() const;

    void update( double elapsed_time );

  private:
    mark_placement get_mark_in_local_coordinates
    ( const snapshot& init, const snapshot& end, const mark* m ) const;

    void insert_tweener
    ( mark_placement& m, const mark_placement& end, double d );

    // not implemented
    model_snapshot_tweener( const model_snapshot_tweener& that );
    model_snapshot_tweener& operator=( const model_snapshot_tweener& that );

  private:
    /** \brief The placement of the marks in this snapshot. */
    std::vector<mark_placement> m_placement;

    /** \brief The tweeners for the values of the mark placements. */
    claw::tween::tweener_group m_tweeners;

  }; // class model_snapshot_tweener

} // namespace bf

#endif // __BF_MODEL_TWEENER_HPP__
