/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item loader for speaker_item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SPEAKER_ITEM_LOADER_HPP__
#define __ENGINE_SPEAKER_ITEM_LOADER_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/engine/loader/item_loader_base.hpp"

namespace bear
{
  namespace engine
  {
    class speaker_item;

    /**
     * \brief The item loader for the speaker_item.
     *
     * The valid fields for this item are
     *  - \a speeches: \c The speeches,
     *  - \a persistent_balloon: \c Tells that the balloon is visible even if
     *    the item is off screen.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT speaker_item_loader:
      public item_loader_base
    {
    public:
      speaker_item_loader( std::string prefix, speaker_item& item );

      virtual speaker_item_loader* clone() const;

      virtual bool set_field
        ( const std::string& name, const std::vector<std::string>& value );
      virtual bool set_field( const std::string& name, bool value );

    private:
      /** \brief The item loaded by this loader. */
      speaker_item& m_item;

    }; // class speaker_item_loader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SPEAKER_ITEM_LOADER_HPP__
