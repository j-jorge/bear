/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A color fading in and out.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_FADE_EFFECT_HPP__
#define __BEAR_ENGINE_FADE_EFFECT_HPP__

#include "engine/transition_effect/transition_effect.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A color fading in and out.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT fade_effect:
      public transition_effect
    {
    public:
      /** \brief The type of a list of scene elements retrieved from the
          layer. */
      typedef std::list<visual::scene_element> scene_element_list;

    public:
      fade_effect();

      bool is_finished() const;

      universe::time_type
      progress( bear::universe::time_type elapsed_time );
      void render( scene_element_list& e ) const;

      void set_duration
      ( universe::time_type in, universe::time_type full,
        universe::time_type out );
      void set_color( double r, double g, double b );
      void set_opacity( double o );

    private:
      void adjust_opacity();

    private:
      /** \brief How long the fade in is. */
      universe::time_type m_fade_in_duration;

      /** \brief How long the full intensity is kept. */
      universe::time_type m_full_duration;

      /** \brief How long the fade out is. */
      universe::time_type m_fade_out_duration;

      /** \brief Elapsed time since the effect was created. */
      universe::time_type m_elapsed_time;

      /** \brief The color displayed. */
      visual::color_type m_color;

      /** \brief The opacity of at the maximum intensity. */
      double m_opacity;

    }; // class fade_effect
  } // namepspace engine
} // namespace bear

#endif // __BEAR_ENGINE_FADE_EFFECT_HPP__
