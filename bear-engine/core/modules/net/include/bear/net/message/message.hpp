/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The message objects carry some data to be exchanged between the
 *        servers and the clients.
 * \author Julien Jorge
 */
#ifndef __NET_MESSAGE_HPP__
#define __NET_MESSAGE_HPP__

#include "bear/net/class_export.hpp"

#include <iosfwd>
#include <string>

namespace bear
{
  namespace net
  {
    class message;
  } // namespace net
} // namespace bear

NET_EXPORT std::ostream& operator<<
( std::ostream& os, const bear::net::message& m );
NET_EXPORT std::istream& operator>>( std::istream& is, bear::net::message& m );

namespace bear
{
  namespace net
  {
    /**
     * \brief The message objects carry some data to be exchanged between the
     *        servers and the clients.
     * \author Julien Jorge
     */
    class NET_EXPORT message
    {
      friend NET_EXPORT std::ostream& ::operator<<
        ( std::ostream& os, const message& m );
      friend NET_EXPORT std::istream& ::operator>>
        ( std::istream& is, message& m );

    public:
      message();
      virtual ~message();

      std::string get_name() const;

      std::size_t get_date() const;
      void set_date( std::size_t date );

    private:
      virtual std::ostream& formatted_output( std::ostream& os ) const;
      virtual std::istream& formatted_input( std::istream& is );
      virtual std::string do_get_name() const = 0;

    private:
      /** The date at which the message has to be considered. */
      std::size_t m_date;

    }; // class message

  } // namespace net
} // namespace bear

#endif // __NET_MESSAGE_HPP__
