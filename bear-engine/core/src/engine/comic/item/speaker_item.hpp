/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The class describing a speaker_item.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_SPEAKER_ITEM_HPP__
#define __BEAR_SPEAKER_ITEM_HPP__

#include "engine/comic/balloon.hpp"

#include "engine/class_export.hpp"

#include <string>
#include <list>
#include <vector>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The class describing a speaker.
     * \author Sébastien Angibaud
     */
    class ENGINE_EXPORT speaker_item
    {
    public:
      speaker_item();
      speaker_item( const speaker_item& that );
      virtual ~speaker_item();

      void progress( universe::time_type elapsed_time );

      bool has_finished_to_speak() const;
      bool has_more_things_to_say() const;

      void stop_speaking();
      void speak( const std::vector<std::string>& speech );
      void speak( const std::string& s );

      balloon& get_balloon();

      void set_persistent_balloon( bool b );
      bool get_persistent_balloon() const;

    private:
      /** \brief The balloon. */
      balloon m_balloon;

      /** \brief The list of speeches. */
      std::list< std::list<std::string> > m_speeches;

      /** \brief Tell if the balloon must stay visible even if the item is off
          screen. */
      bool m_persistent_balloon;

    }; // class speaker_item

  } // namespace engine
} // namespace bear

#endif // __BEAR_SPEAKER_ITEM_HPP__
