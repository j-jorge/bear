/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The base class for the effects of the transition_layer.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_TRANSITION_EFFECT_HPP__
#define __BEAR_ENGINE_TRANSITION_EFFECT_HPP__

#include "universe/types.hpp"
#include "input/input_listener.hpp"
#include "engine/layer/gui_layer.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class level;

    /**
     * \brief The base class for the effects of the transition_layer.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT transition_effect:
      public input::input_listener
    {
    public:
      /** \brief The type of a list of scene elements retrieved from the
          layer. */
      typedef gui_layer::scene_element_list scene_element_list;

    public:
      transition_effect();
      virtual ~transition_effect();

      void set_layer( gui_layer& lay );

      virtual bool is_finished() const;
      virtual void build();
      virtual universe::time_type progress( universe::time_type elapsed_time );
      virtual void render( scene_element_list& e ) const;

    protected:
      level_globals& get_level_globals() const;
      const gui_layer& get_layer() const;
      const level& get_level() const;

    private:
      /** \brief The layer that displays the effect. */
      gui_layer* m_layer;

    }; // class transition_effect
  } // namespace engine
} // namespace bear

#endif // __BEAR_ENGINE_TRANSITION_EFFECT_HPP__
