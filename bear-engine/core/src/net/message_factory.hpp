/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A message_factory instantiate subclasses of bear::net::message.
 * \author Julien Jorge
 */
#ifndef __NET_MESSAGE_FACTORY_HPP__
#define __NET_MESSAGE_FACTORY_HPP__

#include "net/class_export.hpp"

#include <string>
#include <claw/factory.hpp>

namespace bear
{
  namespace net
  {
    class message;

    /**
     * \brief A message_factory instantiate subclasses of bear::net::message.
     * \author Julien Jorge
     */
    typedef claw::pattern::factory<message, std::string> message_factory;

  } // namespace net
} // namespace bear

#endif // __NET_MESSAGE_FACTORY_HPP__
