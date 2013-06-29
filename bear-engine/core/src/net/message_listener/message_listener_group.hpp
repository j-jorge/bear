/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The message_listener group manages several message listeners.
 * \author Julien Jorge
 */
#ifndef __NET_MESSAGE_LISTENER_GROUP_HPP__
#define __NET_MESSAGE_LISTENER_GROUP_HPP__

#include "net/message_listener/message_listener.hpp"

#include "net/class_export.hpp"

#include <list>

namespace bear
{
  namespace net
  {
    class message;

    /**
     * \brief A class that process messages.
     * \author Julien Jorge
     */
    class NET_EXPORT message_listener_group
    {
    private:
      /** \brief The type of the list containing the listeners. */
      typedef std::list<message_listener> listener_list_type;

    public:
      void process_message( const message& m ) const;
      void add( const message_listener& listener );

    private:
      /** \brief The listeners in this group. */
      listener_list_type m_listener;

    }; // class message_listener_group

  } // namespace net
} // namespace bear

#endif // __NET_MESSAGE_LISTENER_GROUP_HPP__
