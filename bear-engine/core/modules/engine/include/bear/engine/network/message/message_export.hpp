/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Macros and function useful for class exportation.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MESSAGE_EXPORT_HPP__
#define __ENGINE_MESSAGE_EXPORT_HPP__

#include "bear/engine/network/message_factory.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Prepare a class inheriting from net::message.
 *
 * \param message_name The name of the message.
 *
 * Add this macro at the beginning of your class, like:
 *
 *   <tt>namespace my_game
 *       {
 *         class my_class: public bear::net::message
 *         {
 *           DECLARE_MESSAGE( my_class );
 *
 *           // some stuff...
 *
 *         }; // class my_class
 *       } // namespace my_game</tt>
 *
 * This macro adds the methods and members to get the class name of your class:
 *  - <tt>static const std::string& static_get_name();</tt>
 *  - <tt>static const std::string s_my_class_message_name;</tt>
 *
 * Also, this macro redefines the virtual method
 *  - <tt>std::string do_get_name() const;</tt>
 * of bear::net::message.
 */
#define DECLARE_MESSAGE( message_name )                                 \
  private:                                                              \
  std::string do_get_name() const { return static_get_name(); }         \
                                                                        \
  public:                                                               \
  static const std::string& static_get_name()                           \
  {                                                                     \
    return s_ ## message_name ## _message_name;                         \
  }                                                                     \
                                                                        \
  private:                                                              \
  static const std::string s_ ## message_name ## _message_name

/*----------------------------------------------------------------------------*/
/**
 * \brief Implement common members of a class inheriting from net::message.
 *
 * \param message_name The name of the class to instanciate.
 * \param env The full namespace of this class.
 *
 * Call this macro in the .cpp file, <b>only if you don't call
 * MESSAGE_EXPORT</b>.
 */
#define MESSAGE_IMPLEMENT( message_name, env )                          \
  const std::string env::message_name::s_ ## message_name ## _message_name = \
    #env "::" #message_name;

/*----------------------------------------------------------------------------*/
/**
 * \brief Implement common members of a class inheriting from net::message
 *        and declared outside any namespace.
 *
 * \param message_name The name of the class to instanciate.
 *
 * Call this macro in the .cpp file, <b>only if you don't call
 * MESSAGE_EXPORT_NO_NAMESPACE</b>.
 *
 * \sa MESSAGE_IMPLEMENT
 */
#define MESSAGE_IMPLEMENT_NO_NAMESPACE( message_name )                  \
  const std::string message_name::s_ ## message_name ## _message_name = \
    #message_name;

/*----------------------------------------------------------------------------*/
/**
 * \brief Register a message derived class in bear::engine::message_factory.
 *
 * \param message_name The name of the class to instanciate.
 * \param env The full namespace of this class.
 *
 * The env::message_name class must have a constructor without parameters.
 */
#define MESSAGE_EXPORT( message_name, env )                             \
  MESSAGE_IMPLEMENT( message_name, env )                                \
                                                                        \
  static bool message_name ## _registered =                             \
    bear::engine::message_factory::get_instance()                       \
    .register_type<env::message_name>                                   \
    ( env::message_name::static_get_name() );

/*----------------------------------------------------------------------------*/
/**
 * \brief Export a function instanciating a class inheriting from
 *        net::message and declared outside any namespace.
 *
 * \param message_name The name of the class to instanciate.
 *
 * The message_name class must have a constructor without parameters.
 *
 * The signature of the function declared by this macro is
 *
 *   <tt>net::message* create_<i>message_name</i>()</tt>
 *
 * The name of the function can be obtained by a call to
 * net::export_function_name( message_name ).
 */
#define MESSAGE_EXPORT_NO_NAMESPACE( message_name )                     \
  MESSAGE_IMPLEMENT_NO_NAMESPACE( message_name )                        \
                                                                        \
  static bool message_name ## _registered =                             \
    bear::engine::message_factory::get_instance().register_type<message_name> \
    ( #message_name );

#endif // __ENGINE_MESSAGE_EXPORT_HPP__
