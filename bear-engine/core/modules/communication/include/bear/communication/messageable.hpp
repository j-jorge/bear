/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for items that can receaive message events.
 * \author Julien Jorge
 */
#ifndef __COMMUNICATION_MESSAGEABLE_HPP__
#define __COMMUNICATION_MESSAGEABLE_HPP__

#include "bear/communication/message.hpp"
#include "bear/concept/item_container.hpp"

#include "bear/communication/class_export.hpp"

#include <string>

namespace bear
{
  namespace communication
  {
    /**
     * \brief A class for items that can receaive message events.
     * \author Julien Jorge
     */
    class COMMUNICATION_EXPORT messageable:
      private concept::item_container<message*>
    {
    public:
      /** \brief The type of the parent class. */
      typedef concept::item_container<message*> super;

    public:
      messageable();
      messageable( const std::string& name );
      messageable( const messageable& that );
      virtual ~messageable();

      void set_name(const std::string& name);
      const std::string& get_name() const;

      void post_message( message& msg );
      bool send_message( message& msg );
      void process_messages();

    private:
      virtual bool process_message( message& msg );

      virtual void add( message* const& who);
      virtual void remove( message* const& who);

    private:
      /** \brief The (unique) name of the item. */
      std::string m_name;

      /** \brief Message queue. */
      std::list<message*> m_message_queue;

    }; // class messageable
  } // namespace communication
} // namespace bear

#endif // __COMMUNICATION_MESSAGEABLE_HPP__
