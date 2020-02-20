/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for all classes for which we want to be able to call
 *        methods from a text_interface file.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_BASE_EXPORTABLE_HPP__
#define __TEXT_INTERFACE_BASE_EXPORTABLE_HPP__

#include "bear/text_interface/method_caller_implement.hpp"

#include <vector>
#include <string>
#include <map>

#include <typeinfo>
#include <iostream>

/**
 * \brief This macro, used in a base_exportable class definition, defines the
 *        static members used to store the callers of the methods. The call is
 *        done from TEXT_INTERFACE_DECLARE_METHOD_LIST.
 */
#define TEXT_INTERFACE_DECLARE_METHOD_LIST_BASE                         \
  protected:                                                            \
  typedef                                                               \
  std::map                                                              \
  <std::string, bear::text_interface::method_caller const*>                \
  method_list_data_type;                                                \
                                                                        \
  static bear::text_interface::base_exportable::method_list s_method_list; \
                                                                        \
  static void self_methods_set                                          \
  ( const std::string& name,                                            \
    bear::text_interface::method_caller const* m )                        \
  { s_method_list.data[name] = m; }                                     \
                                                                        \
private:                                                                \
  virtual bear::text_interface::base_exportable::method_list const*     \
  get_method_list() const                                               \
  {                                                                     \
    init_method_list();                                                 \
    return &s_method_list;                                              \
  }

/*--------------------------------------------------------------------------*/
/**
 * \brief This macro, used in a base_exportable class definition, defines the
 *        static members used to store the callers of the methods.
 * \param parent_class The type of the parent class. Must inherit from
 *        base_exportable.
 * \param export_func A static method from the class, called to initialise the
 *        method callers.
 */
#define TEXT_INTERFACE_DECLARE_METHOD_LIST(parent_class, export_func)   \
  TEXT_INTERFACE_DECLARE_METHOD_LIST_BASE                               \
protected:                                                                \
  static void init_method_list()                                        \
  {                                                                     \
    if ( s_method_list.parent == NULL )                                 \
      {                                                                 \
        parent_class::init_method_list();                               \
        s_method_list.parent = &parent_class::s_method_list;            \
        export_func();                                                  \
      }                                                                 \
  }

namespace bear
{
  namespace text_interface
  {
    class auto_converter;

    /**
     * \brief The base class for all classes for which we want to be able to
     *        call methods from a text_interface file.
     *
     * Those classes contain a static table of the methods to call, associated
     * with their names.
     *
     * \author Julien Jorge.
     */
    class base_exportable
    {
    protected:
      /** \brief The type of the list of callable methods. */
      struct method_list
      {
      public:
        /** \brief The type of the container where the method callers are
            stored. */
        typedef std::map<std::string, method_caller const*> list_type;

        method_list();

      public:
        /** \brief The list of the parent class. Multiple inheritance is not
            allowed yet. */
        method_list const* parent;

        /** \brief The callers of the methods. */
        list_type data;

      }; // struct method_list

    private:
      TEXT_INTERFACE_DECLARE_METHOD_LIST_BASE

    public:
      /** \remark The desctructor must be virtual so we can use dynamic_cast in
          explicit_method_caller. */
      virtual ~base_exportable();

      void execute
      ( const std::string& n,
        const std::vector<std::string>& args = std::vector<std::string>() );

      void execute( const std::string& n, const auto_converter& c );
      void execute( const std::string& n, const std::vector<std::string>& args,
                    const argument_converter& c );

    protected:
      static void init_method_list();

    private:
      method_caller const* find_function( const std::string& n ) const;

    }; // class base_exportable

  } // namespace text_interface
} // namespace bear

/**
 * \brief This macro is used in the implementation of base_exportable to mark
 *        the end of the method lists in the hierarchy.
 */
#define TEXT_INTERFACE_IMPLEMENT_ROOT_METHOD_LIST                       \
  bear::text_interface::base_exportable::method_list                    \
  bear::text_interface::base_exportable::s_method_list;

/**
 * \brief Use this macro in the implementation file of your class to implement
 *        the data required by base_exportable.
 * \param self_type The type of your class.
 */
#define TEXT_INTERFACE_IMPLEMENT_METHOD_LIST(self_type)                 \
  bear::text_interface::base_exportable::method_list                    \
  self_type::s_method_list;

/**
 * \brief Use this macro in the implementation file of your template class to
 *        implement the data required by base_exportable.
 * \param self_type The type of your class.
 * \param K1 The keyword before T1.
 * \param T1 The first type of the template.
 */
#define TEXT_INTERFACE_IMPLEMENT_TEMPLATE_METHOD_LIST_1(self_type, K1, T1) \
  template<K1 T1>                                                       \
  bear::text_interface::base_exportable::method_list                    \
  self_type<T1>::s_method_list;

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method without arguments.
 * \param self_type The class for which we declare the methods.
 * \param parent_type The parent class of self_type in which the method is
 *        defined.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 */
#define TEXT_INTERFACE_CONNECT_PARENT_METHOD_0( self_type, parent_type, \
                                                method_name, R )        \
  self_methods_set                                                      \
  ( #method_name,                                                       \
    &bear::text_interface::method_caller_implement_0                    \
    <                                                                   \
    self_type, parent_type,                                             \
    R,                                                                  \
    &parent_type::method_name>::s_caller );

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method with a single
 *        argument.
 * \param self_type The class for which we declare the methods.
 * \param parent_type The parent class of self_type in which the method is
 *        defined.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 * \param T1 The type of the first argument of self_type::method_name().
 */
#define TEXT_INTERFACE_CONNECT_PARENT_METHOD_1( self_type, parent_type, \
                                                method_name, R, T1 )    \
  self_methods_set                                                      \
  ( #method_name,                                                       \
    &bear::text_interface::method_caller_implement_1                    \
    <                                                                   \
    self_type, parent_type,                                             \
    R, T1,                                                              \
    &self_type::method_name >::s_caller );

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method with two arguments.
 * \param self_type The class for which we declare the methods.
 * \param parent_type The parent class of self_type in which the method is
 *        defined.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 * \param T1 The type of the first argument of self_type::method_name().
 * \param T2 The type of the second argument of self_type::method_name().
 */
#define TEXT_INTERFACE_CONNECT_PARENT_METHOD_2( self_type, parent_type, \
                                                method_name, R, T1, T2 ) \
  self_methods_set                                                      \
  ( #method_name,                                                       \
    &bear::text_interface::method_caller_implement_2                    \
    <                                                                   \
    self_type, parent_type,                                             \
    R, T1, T2,                                                          \
    &self_type::method_name >::s_caller );

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method with three arguments.
 * \param self_type The class for which we declare the methods.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 * \param T1 The type of the first argument of self_type::method_name().
 * \param T2 The type of the second argument of self_type::method_name().
 * \param T3 The type of the third argument of self_type::method_name().
 */
#define TEXT_INTERFACE_CONNECT_PARENT_METHOD_3( self_type, parent_type, \
                                                method_name, R, T1, T2, \
                                                T3 )                    \
  self_methods_set                                                      \
  ( #method_name,                                                       \
    &bear::text_interface::method_caller_implement_3                    \
    <                                                                   \
    self_type, parent_type,                                             \
    R, T1, T2, T3,                                                      \
    &self_type::method_name >::s_caller );

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method without arguments.
 * \param self_type The class for which we declare the methods.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 */
#define TEXT_INTERFACE_CONNECT_METHOD_0( self_type, method_name, R )    \
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_0(self_type, self_type, method_name, R);

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method with a single
 *        argument.
 * \param self_type The class for which we declare the methods.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 * \param T1 The type of the first argument of self_type::method_name().
 */
#define TEXT_INTERFACE_CONNECT_METHOD_1( self_type, method_name, R, T1 ) \
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_1(self_type, self_type, method_name, R, \
                                         T1);

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method with two arguments.
 * \param self_type The class for which we declare the methods.
 * \param parent_type The parent class of self_type in which the method is
 *        defined.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 * \param T1 The type of the first argument of self_type::method_name().
 * \param T2 The type of the second argument of self_type::method_name().
 */
#define TEXT_INTERFACE_CONNECT_METHOD_2( self_type, method_name, R, T1, T2 ) \
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2(self_type, self_type, method_name, R, \
                                         T1, T2);

/**
 * \brief This macro is used in the implementation of a class inheriting from
 *        base_exportable to define an caller for a method with three arguments.
 * \param self_type The class for which we declare the methods.
 * \param method_name The name of the method to execute.
 * \param R The return type of the method.
 * \param T1 The type of the first argument of self_type::method_name().
 * \param T2 The type of the second argument of self_type::method_name().
 * \param T3 The type of the third argument of self_type::method_name().
 */
#define TEXT_INTERFACE_CONNECT_METHOD_3( self_type, method_name, R, T1, \
                                         T2, T3 )                       \
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_3(self_type, self_type, method_name, R, \
                                         T1, T2, T3);

#endif // __TEXT_INTERFACE_BASE_EXPORTABLE_HPP__
