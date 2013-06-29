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
#ifndef __ENGINE_MODEL_TWEENER_HPP__
#define __ENGINE_MODEL_TWEENER_HPP__

#include "universe/types.hpp"
#include "engine/class_export.hpp"

#include <claw/tween/tweener_group.hpp>

#include <vector>

namespace bear
{
  namespace engine
  {
    class model_action;
    class model_mark_placement;
    class model_snapshot;

    /**
     * \brief A structure that manages all the values that can be interpolated
     *        in a snapshot.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_snapshot_tweener
    {
    public:
      typedef std::vector<model_mark_placement>::const_iterator
      const_mark_placement_iterator;

    public:
      explicit model_snapshot_tweener( const model_snapshot& init );
      model_snapshot_tweener
      ( const model_snapshot& init, const model_snapshot& end,
        const model_action& init_action, const model_action& end_action,
        universe::time_type d );

      model_mark_placement& get_mark_placement( std::size_t i );
      const model_mark_placement& get_mark_placement( std::size_t i ) const;

      const_mark_placement_iterator mark_placement_begin() const;
      const_mark_placement_iterator mark_placement_end() const;

      void update( universe::time_type elapsed_time );

    private:
      model_mark_placement get_mark_in_local_coordinates
      ( const model_snapshot& init, const model_snapshot& end,
          std::size_t id ) const;

      void insert_tweener
      ( std::size_t id, const model_mark_placement& end,
        universe::time_type d );

      // not implemented
      model_snapshot_tweener( const model_snapshot_tweener& that );
      model_snapshot_tweener& operator=( const model_snapshot_tweener& that );

    private:
      /** \brief The placement of the marks in this snapshot. */
      std::vector<model_mark_placement> m_placement;

      /** \brief The tweeners for the values of the mark placements. */
      claw::tween::tweener_group m_tweeners;

    }; // class model_snapshot_tweener

  } // namespace engine
} // namespace bear

#endif // __ENGINE_MODEL_TWEENER_HPP__
