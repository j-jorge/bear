/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Macros and function useful for class exportation.
 * \author Julien Jorge
 */
#ifndef __ENGINE_EXPORT_HPP__
#define __ENGINE_EXPORT_HPP__

#ifndef DLL_EXPORT
#  ifdef _WIN32
#    define DLL_EXPORT __declspec(dllexport)
#  else // def _WIN32
#    define DLL_EXPORT
#  endif // def _WIN32
#endif // ndef EXPORT

#include "engine/item_factory.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Prepare a class inheriting from engine::base_item.
 *
 * \param class_name The name of the class.
 *
 * Add this macro at the beginning of your class, like:
 *
 *   <tt>namespace my_game
 *       {
 *         class my_class: public bear::engine::base_item
 *         {
 *           DECLARE_BASE_ITEM( my_class );
 *
 *           // some stuff...
 *
 *         }; // class my_class
 *       } // namespace my_game</tt>
 *
 * This macro adds the methods and members to get the class name of your class:
 *  - <tt>const char* get_class_name() const;</tt>
 *  - <tt>static const char* static_class_name();</tt>
 *  - <tt>static const char* s_my_class_class_name;</tt>
 *
 * Also, this macro redefines the virtual method
 *  - <tt>base_item* clone() const</tt>
 * of base_item with
 *  - <tt>my_class* clone() const</tt>
 */
#define DECLARE_BASE_ITEM( class_name )                                 \
  public:                                                               \
  class_name* clone() const { return new class_name(*this); }           \
                                                                        \
  const char* get_class_name() const { return static_class_name(); }    \
  std::size_t size_of() const { return sizeof(class_name); }            \
                                                                        \
  static const char* static_class_name()                                \
  {                                                                     \
    return s_ ## class_name ## _class_name;                             \
  }                                                                     \
                                                                        \
  private:                                                              \
  static const char* s_ ## class_name ## _class_name

/*----------------------------------------------------------------------------*/
/**
 * \brief Implement common members of a class inheriting from engine::base_item.
 *
 * \param class_name The name of the class to instanciate.
 * \param env The full namespace of this class.
 *
 * Call this macro in the .cpp file, <b>only if you don't call
 * BASE_ITEM_EXPORT</b>.
 */
#define BASE_ITEM_IMPLEMENT( class_name, env )                          \
  const char* env::class_name::s_ ## class_name ## _class_name =        \
    #env "::" #class_name;

/*----------------------------------------------------------------------------*/
/**
 * \brief Implement common members of a class inheriting from engine::base_item
 *        and declared outside any namespace.
 *
 * \param class_name The name of the class to instanciate.
 *
 * Call this macro in the .cpp file, <b>only if you don't call
 * BASE_ITEM_EXPORT_NO_NAMESPACE</b>.
 *
 * \sa BASE_ITEM_IMPLEMENT
 */
#define BASE_ITEM_IMPLEMENT_NO_NAMESPACE( class_name )                  \
  const char* class_name::s_ ## class_name ## _class_name = #class_name;

/*----------------------------------------------------------------------------*/
/**
 * \brief Register a base_item derived class in bear::engine::item_factory.
 *
 * \param class_name The name of the class to instanciate.
 * \param env The full namespace of this class.
 *
 * The env::class_name class must have a constructor without parameters.
 */
#define BASE_ITEM_EXPORT( class_name, env )                             \
  BASE_ITEM_IMPLEMENT( class_name, env )                                \
                                                                        \
  static bool class_name ## _registered =                               \
    bear::engine::item_factory::get_instance().register_type<env::class_name> \
    ( env::class_name::static_class_name() );

/*----------------------------------------------------------------------------*/
/**
 * \brief Export a function instanciating a class inheriting from
 *        engine::base_item and declared outside any namespace.
 *
 * \param class_name The name of the class to instanciate.
 *
 * The class_name class must have a constructor without parameters.
 *
 * The signature of the function declared by this macro is
 *
 *   <tt>engine::base_item* create_<i>class_name</i>()</tt>
 *
 * The name of the function can be obtained by a call to
 * engine::export_function_name( class_name ).
 */
#define BASE_ITEM_EXPORT_NO_NAMESPACE( class_name )                     \
  BASE_ITEM_IMPLEMENT_NO_NAMESPACE( class_name )                        \
                                                                        \
  static bool class_name ## _registered =                               \
    bear::engine::item_factory::get_instance().register_type<class_name> \
    ( #class_name );

#endif // __ENGINE_EXPORT_HPP__
