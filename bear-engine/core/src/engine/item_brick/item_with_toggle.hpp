/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that can be toggled on or off.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_WITH_TOGGLE_HPP__
#define __ENGINE_ITEM_WITH_TOGGLE_HPP__

#include "audio/sample.hpp"
#include "engine/item_brick/with_toggle.hpp"
#include "universe/derived_item_handle.hpp"
#include "universe/types.hpp"

#include <list>
#include <vector>

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item that can be toggled on or off.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from base_item.
     *
     * The custom fields of this class are:
     * - initial_state: bool, the initial state (default = off/false),
     * - delay: real, the delay, in seconds, before turning off automatically
     *   (default = infinity),
     * - linked_toggles: list of toggles, a list of toggles to which the status
     *   of this toggle is transmitted (default = none),
     * - any field supported by the parent classes.
     *
     * \author Julien Jorge
     */
    template<class Base>
    class item_with_toggle:
      public Base,
      public with_toggle
    {
      typedef Base super;

    private:
      /** \brief The type of the handles on the toggles to which the state of
          this toggle is transmitted. */
      typedef universe::derived_item_handle<with_toggle> toggle_handle;

      /** \brief The type of the list of the handles to the toggles to which the
          state of this toggle is transmitted. */
      typedef std::list<toggle_handle> handle_list;

    public:
      item_with_toggle();
      item_with_toggle( const item_with_toggle<Base>& that );
      ~item_with_toggle();

      bool set_real_field( const std::string& name, double value );
      bool set_bool_field( const std::string& name, bool value );
      bool set_sample_field( const std::string& name, audio::sample* value );
      bool set_item_list_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

      void build();
      void progress( universe::time_type elapsed_time );

      void add_linked_toggle( with_toggle* t );
      std::vector<with_toggle*> get_linked_toggles() const;

      universe::time_type get_delay() const;
      void set_delay( universe::time_type d );

      void set_sample( audio::sample* s );
      void set_fadeout( universe::time_type d );

      bool is_on() const;
      void toggle_on( base_item* activator );
      void toggle_off( base_item* activator );

      virtual
      void get_dependent_items( typename super::item_list& d ) const;

    protected:
      
      void progress_toggle( universe::time_type elapsed_time );

      virtual void build_on();
      virtual void build_off();
      virtual void on_toggle_on( base_item* activator );
      virtual void on_toggle_off( base_item* activator );
      virtual void progress_on( universe::time_type elapsed_time );
      virtual void progress_off( universe::time_type elapsed_time );

    private:
      // not implemented
      item_with_toggle<Base>& operator=( const item_with_toggle<Base>& that );

      void play_sound() const;
      void stop_sound() const;
      void progress_sound() const;

      void toggle_linked( base_item* activator );

    private:
      /** \brief Tell if the item is turned on. */
      bool m_is_on;

      /** \brief Time elapsed since the item has been turned on. */
      universe::time_type m_elapsed_time;

      /** \brief Delay before turning the item off. */
      universe::time_type m_delay;

      /** \brief Fade out the sample during this duration when stopping. */
      universe::time_type m_fadeout;

      /** \brief The sample played when the toggle is on. */
      audio::sample* m_sample;

      /**
       * \brief A list of toggles to which the state of this toggle is
       *        transmitted.
       */
      handle_list m_linked;

    }; // class item_with_toggle
  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/item_with_toggle.tpp"

#endif // __ENGINE_ITEM_WITH_TOGGLE_HPP__
