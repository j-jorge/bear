/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Add a strip at the top and the bottom of the screen.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_STRIP_EFFECT_HPP__
#define __BEAR_ENGINE_STRIP_EFFECT_HPP__

#include "engine/transition_effect/transition_effect.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Add a strip at the top and the bottom of the screen.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT strip_effect:
      public transition_effect
    {
    public:
      /** \brief The type of a list of scene elements retrieved from the
          layer. */
      typedef std::list<visual::scene_element> scene_element_list;

    public:
      strip_effect();

      bool is_finished() const;

      universe::time_type progress( bear::universe::time_type elapsed_time );
      void render( scene_element_list& e ) const;

      void set_duration
      ( universe::time_type in, universe::time_type full,
        universe::time_type out );
      void set_color( double r, double g, double b );
      void set_opacity( double o );
      void set_strip_height( visual::coordinate_type h );

    private:
      /** \brief How long does it take to the strips to appear. */
      universe::time_type m_strip_in_duration;

      /** \brief How long the strips are fully visible. */
      universe::time_type m_full_duration;

      /** \brief How long does it take to the strips to disapear. */
      universe::time_type m_strip_out_duration;

      /** \brief Elapsed time since the effect was created. */
      universe::time_type m_elapsed_time;

      /** \brief The color displayed. */
      visual::color_type m_color;

      /** \brief The opacity of at the maximum intensity. */
      double m_opacity;

      /** \brief The height of the strips. */
      visual::coordinate_type m_height;

    }; // class strip_effect
  } // namepspace engine
} // namespace bear

#endif // __BEAR_ENGINE_STRIP_EFFECT_HPP__
