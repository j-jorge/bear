/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that stores some statistics about the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_STATS_HPP__
#define __ENGINE_GAME_STATS_HPP__

#include "bear/engine/class_export.hpp"
#include "bear/engine/stat_variable.hpp"
#include "bear/time/time.hpp"

#include <list>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A class that stores some statistics about the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_stats
    {
      private:
      /**
       * \brief A function object that sends a statistic information.
       */
      class statistic_sender
      {
      public:
        void operator()() const;
        
      public:
        /** \brief The address. */
        std::string address;
        
        /** \brief The xml to send. */
        std::string xml_string;
        
      }; // struct statistic_sender

    public:
      game_stats();

      void start( std::string name, std::string id ) const;
      void end() const;

      void start_level( std::string name ) const;
      void pop_level( std::string name ) const;

      void set_tag( std::string tag );
      void set_destination( std::string destination );

      void send_data
        ( std::string operation, std::string name, std::string value ) const;
      void send_data
        ( std::string operation, std::list<stat_variable> vars ) const;

    private:      
      void send( std::string xml_string ) const;
      void http_post( std::string address, std::string xml_string ) const;

      std::string generate_xml_stats
        ( std::string operation, std::list<stat_variable> vars ) const;

    private:
      /** \brief The identifier of the game instance. */
      std::string m_launch_id;

      /** \brief The date at which the game was started. */
      const systime::milliseconds_type m_init_date;

      /** \brief The identifier of the current user. */
      std::string m_user_id;

      /** \brief The tag of the build. */
      std::string m_tag;

      /** \brief The destination to which the stats are sent. */
      std::string m_destination;

    }; // class game_stats
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_STATS_HPP__
