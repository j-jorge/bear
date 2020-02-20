/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A trigger to control some toggles.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_WITH_TRIGGER_HPP__
#define __ENGINE_ITEM_WITH_TRIGGER_HPP__

#include "bear/engine/item_brick/with_trigger.hpp"
#include "bear/universe/types.hpp"

#include <list>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A trigger to control some toggles.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item.
     *
     * The custom fields of this class are:
     * - toggle: item list, the toggles controlled by this trigger
     *   (default = none),
     * - check_on_progress: bool, check the condition at each progress
     *   (default = true),
     * - check_on_collision: bool, check the condition at each collision
     *   (default = false),
     * - mode: string in {one_way, switch, condition}, defines the behaviour of
     *   the trigger (see item_with_trigger::mode) (default = one_way),
     * - any field supported by the parent classes.
     *
     * \author Julien Jorge
     */
    template<class Base>
    class item_with_trigger:
      public Base,
      public with_trigger
    {
      typedef Base super;

    public:
      item_with_trigger();

      bool set_bool_field( const std::string& name, bool value );

      void progress( universe::time_type elapsed_time );

    protected:
      void check_and_toggle( base_item* activator );
      void progress_trigger( universe::time_type elapsed_time );
      void collision_trigger( base_item& that, universe::collision_info& info );

    private:
      void collision( base_item& that, universe::collision_info& info );

    private:
      /** \brief Tell if the condition is checked at each progress. */
      bool m_check_on_progress;

      /** \brief Tell if the condition is checked at each collision. */
      bool m_check_on_collision;

    }; // class item_with_trigger
  } // namespace engine
} // namespace bear

#include "bear/engine/item_brick/impl/item_with_trigger.tpp"

#endif // __ENGINE_ITEM_WITH_TRIGGER_HPP__
