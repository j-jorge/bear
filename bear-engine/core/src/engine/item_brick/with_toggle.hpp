/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class describes the interface of a two states item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_TOGGLE_HPP__
#define __ENGINE_WITH_TOGGLE_HPP__

#include "universe/types.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief This class describes the interface of a two states item.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_toggle
    {
    public:
      void toggle( base_item* activator );
      void toggle( bool b, base_item* activator );

      virtual universe::time_type get_delay() const;

      virtual void toggle_on( base_item* activator );
      virtual void toggle_off( base_item* activator );
      virtual bool is_on() const;

    }; // class with_toggle
  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_TOGGLE_HPP__
