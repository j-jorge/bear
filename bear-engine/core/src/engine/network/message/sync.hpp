/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The sync message is used to synchronize the clients and the servers.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SYNC_HPP__
#define __ENGINE_SYNC_HPP__

#include "net/message/message.hpp"

#include "engine/network/message/message_export.hpp"
#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The sync message is used to synchronize the clients and the
     *        servers.
     * 
     * A server would send a sync message to its clients to tell them to prepare
     * for a new  wave of messages.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT sync:
      public net::message
    {
      DECLARE_MESSAGE(sync);

    public:
      sync();
      sync( std::size_t id, bool active_sync );

      std::size_t get_id() const;
      bool is_active_sync() const;

    private:
      virtual std::ostream& formatted_output( std::ostream& os ) const;
      virtual std::istream& formatted_input( std::istream& is );

    private:
      /** \brief An identifier associated with the sync message to not confuse
          with an other one. */
      std::size_t m_id;

      /** \brief Tell if this synchronization is the result of an active
          iteration. */
      bool m_active_sync;

    }; // class sync

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SYNC_HPP__
