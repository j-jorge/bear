/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A factory to instanciate classes inheriting from base_message.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MESSAGE_FACTORY_HPP__
#define __ENGINE_MESSAGE_FACTORY_HPP__

#include "bear/engine/class_export.hpp"

#include "bear/net/message_factory.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A factory to instanciate classes inheriting from net::message.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT message_factory
    {
    public:
      /** \brief The type of the factory. */
      typedef net::message_factory factory_type;

    public:
      static factory_type& get_instance();

    private:
      /** \brief A factory that creates messages. */
      static factory_type* s_factory_instance;

    }; // class message_factory

  } // namespace engine
} // namespace bear

#endif // __ENGINE_MESSAGE_FACTORY_HPP__
