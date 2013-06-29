/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement made of a sequence of forced movements.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_SEQUENCE_HPP__
#define __UNIVERSE_FORCED_SEQUENCE_HPP__

#include "universe/forced_movement/base_forced_movement.hpp"
#include "universe/forced_movement/forced_movement.hpp"

#include "universe/class_export.hpp"

#include <vector>

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement made of a sequence of forced movements.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_sequence:
      public base_forced_movement
    {
    public:
      forced_sequence();

      base_forced_movement* clone() const;

      void push_back( const forced_movement& m );
      void set_loops( unsigned int loops );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );
      void next_sequence();

      void init_current_subsequence();

    private:
      /** \brief The sub sequences. */
      std::vector<forced_movement> m_sub_sequence;

      /** \brief Index of the current sub sequence. */
      unsigned int m_index;

      /** \brief How many times the sequence will be done. */
      unsigned int m_loops;

      /** \brief How many full play have we done ? */
      unsigned int m_play_count;

      /** \brief Tells if the movement is finished for another reason that the
          end of the loops. */
      bool m_finished;

      /** \brief Tells if the last loop has produced a change. */
      bool m_has_changed;

    }; // class forced_sequence
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_SEQUENCE_HPP__
