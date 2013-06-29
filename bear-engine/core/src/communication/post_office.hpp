/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to transfer message to items.
 * \author Julien Jorge
 */
#ifndef __COMMUNICATION_POST_OFFICE_HPP__
#define __COMMUNICATION_POST_OFFICE_HPP__

#include "communication/messageable.hpp"
#include "concept/item_container.hpp"

#include <map>
#include <string>

namespace bear
{
  namespace communication
  {
    class message;

    /**
     * \brief A class to transfer message to items.
     * \author Julien Jorge
     */
    class COMMUNICATION_EXPORT post_office:
      public concept::item_container<messageable*>
    {
    public:
      bool send_message( const std::string& target, message& msg ) const;
      void process_messages();

      bool exists( const std::string& name ) const;

      void clear();

    protected:
      void add( messageable* const& who );
      void remove( messageable* const& who );

    public:
      /** \brief The name of items that do not have a name... */
      static const std::string no_name;

    private:
      std::map<std::string, messageable*> m_items;

    }; // class post_office

  } // namespace communication
} // namespace bear

#endif // __COMMUNICATION_POST_OFFICE_HPP__
