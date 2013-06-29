/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item loader for forced_movement_repeater.
 * \author Julien Jorge
 */
#ifndef __ENGINE_FORCED_MOVEMENT_REPEATER_LOADER_HPP__
#define __ENGINE_FORCED_MOVEMENT_REPEATER_LOADER_HPP__

#include <string>

#include "engine/class_export.hpp"

#include "engine/item_brick/loader/forced_movement_loader.hpp"
#include "universe/types.hpp"

namespace bear
{
  namespace engine
  {
    class forced_movement_repeater;

    /**
     * \brief The item loader for the forced_movement_repeater.
     *
     * The custom fields of this class are:
     *
     *  - \a actor: (item) the item to which and to the clones of which
     *    the movement is applied (default = none),
     *  - \a count: (unsigned int) the number of clones to create (default = 0),
     *  - \a delay: (real) the time interval between the placement of two
     *    clones,
     *  - any field supported by the parent classes.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT forced_movement_repeater_loader:
      public forced_movement_loader
    {
    public:
      explicit forced_movement_repeater_loader
        ( forced_movement_repeater& item );

      virtual forced_movement_repeater_loader* clone() const;

      bool set_field( const std::string& name, unsigned int value );
      bool set_field( const std::string& name, double value );
      bool set_field( const std::string& name, base_item* value );

    private:
      /** \brief The item loaded by this loader. */
      forced_movement_repeater& m_item;

    }; // class forced_movement_repeater_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_FORCED_MOVEMENT_REPEATER_LOADER_HPP__
