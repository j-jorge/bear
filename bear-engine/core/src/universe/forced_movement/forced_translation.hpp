/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement that will make the current item translating.
 * \author Sébastien Angibaud
 */
#ifndef __UNIVERSE_FORCED_TRANSLATION_HPP__
#define __UNIVERSE_FORCED_TRANSLATION_HPP__

#include <climits>
#include <limits>

#include "universe/forced_movement/base_forced_movement.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will make the current item translating.
     *
     * The positions considered for the items are their center of mass.
     *
     * \author Sébastien Angibaud
     */
    class UNIVERSE_EXPORT forced_translation:
      public base_forced_movement
    {
    public:
      forced_translation
      ( time_type length = std::numeric_limits<time_type>::infinity() );
      forced_translation
      ( const speed_type& speed,
        time_type length = std::numeric_limits<time_type>::infinity() );

      base_forced_movement* clone() const;

      void set_angle( double angle );
      void set_force_angle( bool f );
      void set_speed( const speed_type& speed );
      void set_total_time( time_type length );

      const speed_type& get_speed() const;

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

    private:
      /** \brief Speed of the item. */
      speed_type m_speed;

      /** \brief Total time of the movement. */
      time_type m_total_time;

      /** \brief Remaining time. */
      time_type m_remaining_time;

      /** \brief The angle added to the initial angle of the item. */
      double m_angle;

      /** \brief Tell if we force the angle of the item (otherwise the angles
          are added). */
      bool m_force_angle;

    }; // class forced_translation
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_TRANSLATION_HPP__
