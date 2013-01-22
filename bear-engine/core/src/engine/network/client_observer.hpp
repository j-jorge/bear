/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A client_observer is an object that dispatch the messages received by
 *        a client.
 * \author Sebastien Angibaud
 */
#ifndef __NET_CLIENT_OBSERVER_HPP__
#define __NET_CLIENT_OBSERVER_HPP__

#include "engine/class_export.hpp"

#include "net/message_listener/message_listener_group.hpp"

#include <map>
#include <string>
#include <boost/function.hpp> 

namespace bear
{
  namespace engine
  {
    class client_connection;

    /**
     * \brief A client_observer is an object that dispatch the messages 
     *        received by a client.
     * \author Sebastien Angibaud
     */
    class ENGINE_EXPORT client_observer
    {
    private:
      /** \brief The type of the map in which the listeners are stored. */
      typedef std::map<std::string, net::message_listener_group> listener_map;

    public:
      explicit client_observer( client_connection* c );

      void process_message();

    public:
      template<typename MessageType>
      void subscribe( boost::function<void (const MessageType&)> f );
    
    private:
      /** \brief The client of which we listen the messages. */
      client_connection* m_client;

      /** \brief The map containing all messages to dispatch. */
      listener_map m_listeners;
    }; // class client_observer

  } // namespace engine
} // namespace bear

#include "engine/network/impl/client_observer.tpp"

#endif // __NET_CLIENT_OBSERVER_HPP__
