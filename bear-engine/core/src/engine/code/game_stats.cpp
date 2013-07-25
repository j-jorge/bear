/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game_stats class.
 * \author Julien Jorge
 */
#include "engine/game_stats.hpp"

#include "engine/version.hpp"

#include <claw/logger.hpp>
#include <claw/socket_stream.hpp>
#include <claw/system_info.hpp>

#include <iomanip>
#include <sstream>
#include <fstream>

#include <boost/preprocessor/stringize.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/thread.hpp>


/*----------------------------------------------------------------------------*/
/**
 * \brief Send the stats to a given server via HTTP POST.
 */
void bear::engine::game_stats::statistic_sender::operator()() const
{
  std::string page;
  std::string server(address);

  std::size_t pos = server.find_first_of('/');

  if ( pos != std::string::npos )
    {
      page = server.substr( pos );
      server = server.substr( 0, pos );
    }

  const int port(4080);
  claw::net::socket_stream os( server.c_str(), port );
  
  if ( !os )
    {
#ifdef _DEBUG
    claw::logger << claw::log_error << "Cannot connect to " << server
                 << " on port " << port << ".";
#endif
    }
  else
    { 
      #ifdef _DEBUG
      claw::logger << claw::log_verbose << "Connected to " << server
                   << ", requesting " << page << '\n'
                   << xml_string << std::endl;
#endif

      os << "POST " << page << " HTTP/1.1\n"
         << "Host: " << server << "\n"
         << "From: stats@gamned.org\n"
         << "Content-Length: " << xml_string.size() << '\n'
         << "Content-Type: application/xml\n"
         << '\n'
         << xml_string
         << std::flush;
    }
} // game_stats::statistic_sender()





/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::game_stats::game_stats()
  : m_init_date(systime::get_unix_time())
{
  std::ostringstream oss;
  oss << boost::uuids::random_generator()();
  m_launch_id = oss.str();
} // game_stats::game_stats()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the start.
 * \param name The name of the game.
 * \param id The identifier of the player.
 */
void bear::engine::game_stats::start( std::string name, std::string id ) const
{
  std::list<stat_variable> params;
  params.push_back( stat_variable( "name", name ) );
  params.push_back( stat_variable( "player-id", id ) );

  std::string tag;

#ifdef _WIN32
  tag = "win32";
#else
  tag = "unix";
#endif

  if ( !m_tag.empty() )
    tag += '-' + m_tag;

  params.push_back( stat_variable( "tag", tag ) );

  send_data( "start-game", params );
} // game_stats::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finalizes the statistics.
 */
void bear::engine::game_stats::end() const
{
  std::list<stat_variable> params;
  send_data( "end-game", params );
} // game_stats::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Note that a level is launched.
 * \param name The name of the level.
 */
void bear::engine::game_stats::start_level( std::string name ) const
{
  send_data( "start-level", "level", name );
} // game_stats::start_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Note that a level is restored.
 * \param name The name of the level.
 */
void bear::engine::game_stats::pop_level( std::string name ) const
{
  send_data( "pop-level", "level", name );
} // game_stats::pop_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the tag of the build.
 * \param tag The new tag.
 */
void bear::engine::game_stats::set_tag( std::string tag )
{
  m_tag = tag;
} // game_stats::set_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the URL of the page to which the statistics are sent.
 * \param destination The URL of a php script that can receive the statistics.
 */
void bear::engine::game_stats::set_destination( std::string destination )
{
  if ( m_destination.empty() )
    m_destination = destination;
} // game_stats::set_destination()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send a variable to the destination page.
 * \param operation The name of the operation stored in the stats.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
void bear::engine::game_stats::send_data
( std::string operation, std::string name, std::string value ) const
{
  std::list<stat_variable> vars;
  vars.push_back( stat_variable( name, value ) );

  send_data( operation, vars );
} // game_stats::send_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send some variables to the destination.
 * \param operation The name of the operation stored in the stats.
 * \param vars The variables to send.
 */
void bear::engine::game_stats::send_data
( std::string operation, std::list<stat_variable> vars ) const
{
  send( generate_xml_stats(operation, vars) );
} // game_stats::send_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the stats to the destination set with set_destination.
 * \param xml_string The data to send.
 */
void bear::engine::game_stats::send( std::string xml_string ) const
{
  const std::string prefix("http://");

  if ( m_destination.find(prefix) != 0 )
    return;

  http_post( m_destination.substr(prefix.length()), xml_string );
} // game_stats::send()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send the stats to a given server via HTTP POST.
 * \param address The address of the server to which the stats are sent.
 * \param xml_string The xml data to post at the given address.
 */
void bear::engine::game_stats::http_post
( std::string address, std::string xml_string ) const
{
  statistic_sender sender;
  sender.address = address;
  sender.xml_string = xml_string;

  boost::thread t(sender);
} // game_stats::http_post()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generates an XML representation of the stats.
 * \param operation The name of the operation stored in the stats.
 * \param vars The variables to put in the XML.
 */
std::string bear::engine::game_stats::generate_xml_stats
( std::string operation, std::list<stat_variable> vars ) const
{
  std::ostringstream result;

  result << "<?xml version=\"1.0\"?>"
         << "<bear-stats launch-id='" << m_launch_id << "' operation='"
         << operation << "'>";

  for ( std::list<stat_variable>::const_iterator it=vars.begin(); 
        it != vars.end(); ++it )
    result << "<variable " << it->name << "='" << it->value << "'/>";

  result << "</bear-stats>";

  return result.str();
} // game_stats::generate_xml_stats()
