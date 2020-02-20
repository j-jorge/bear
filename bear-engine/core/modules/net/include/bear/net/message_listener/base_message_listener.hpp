/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for the message listener classes.
 * \author Julien Jorge
 */
#ifndef __NET_BASE_MESSAGE_LISTENER_HPP__
#define __NET_BASE_MESSAGE_LISTENER_HPP__

#include "bear/net/class_export.hpp"

namespace bear
{
  namespace net
  {
    class message;

    /**
     * \brief The base class for the message listener classes.
     * \author Julien Jorge
     */
    class NET_EXPORT base_message_listener
    {
    public:
      virtual ~base_message_listener();

      virtual base_message_listener* clone() const = 0;

      void process_message( const message& m ) const;

    private:
      virtual void do_process_message( const message& m ) const = 0;

    }; // class base_message_listener

  } // namespace net
} // namespace bear

#endif // __NET_BASE_MESSAGE_LISTENER_HPP__
