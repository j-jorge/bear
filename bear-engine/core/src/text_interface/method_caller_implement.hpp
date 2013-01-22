/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Define the type of a member function of a class, given the class and
 *        the list of the types of the arguments of the fuction.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_METHOD_CALLER_IMPLEMENT_HPP__
#define __TEXT_INTERFACE_METHOD_CALLER_IMPLEMENT_HPP__

#include "text_interface/typed_method_caller.hpp"

namespace bear
{
  namespace text_interface
  {
    /**
     * \brief Define a method caller for a member function of a class, given the
     *        class, the return type of the method, the types of the arguments,
     *        and the adress of the method.
     *
     * There is one class for each number of parameters. If there is no class
     * for the number of parameters of your method,  I suggest that you write
     * the following class, assuming that the method has N+1 parameters:
     *
     * \code
     * template< typename SelfClass, typename ParentClass, typename R,
     *           typename A0, typename A1, ..., typename AN,
     *           R (ParentClass::*Member)(A0, A1, ..., AN) >
     * class method_caller_implement_N+1
     * {
     * public:
     *   // The type of the member function.
     *   typedef R (ParentClass::*mem_fun_type)(A0, A1, ..., AN);
     *
     *   // The caller for SelfClass with the givent argument types
     *   class caller_type:
     *     public typed_method_caller<SelfClass>
     *   {
     *   private:
     *     void explicit_execute
     *     ( SelfClass& self, const std::vector<std::string>& args,
     *       const argument_converter& c ) const
     *     {
     *       const mem_fun_type member(Member);
     *       (self.*member)
     *         ( c.convert_argument<A0>(args[0]),
     *           string_to_arg<context_type, A1>::convert(c, args[1]),
     *           // ...
     *           string_to_arg<context_type, AN>::convert(c, args[N]), );
     *     } // explicit_execute()
     *
     *   }; // class caller_type
     *
     * public:
     *   // The instance of the caller.
     *   // A single instance is sufficient since there is no member variables
     *   // in caller_type (s_caller only does the transition toward the method
     *   // to call). We can use its adress and avoid dynamic allocations.
     *   // <b>Do not forget to implement this member variable.</b>
     *   static const caller_type s_caller;
     *
     * }; // class method_caller_N+1
     * \endcode
     *
     * Then, send us a patch :)
     *
     * \author Julien Jorge.
     */
  template< typename SelfClass, typename ParentClass, typename R,
            R (ParentClass::*Member)() >
  class method_caller_implement_0
  {
  public:
    typedef R (ParentClass::*mem_fun_type)();

  public:
    class caller_type:
      public typed_method_caller<SelfClass>
    {
    public:
      caller_type();

    private:
      void explicit_execute
      ( SelfClass& self, const std::vector<std::string>& args,
        const argument_converter& c ) const;
    }; // class caller_type;

  public:
      /**
       * \brief The instance of the caller.
       *
       * A single instance is sufficient since there is no member variables in
       * caller_type (s_caller only does the transition toward the method to
       * call). We can use its adress and avoid dynamic allocations.
       */
      static const caller_type s_caller;

    }; // class method_caller_implement_0

    /**
     * \brief Same as method_caller_implement_0 but for a method with a single
     *        parameter.
     * \author Julien Jorge.
     */
    template< typename SelfClass, typename ParentClass, typename R, typename A0,
              R (ParentClass::*Member)(A0) >
    class method_caller_implement_1
    {
    public:
      typedef R (ParentClass::*mem_fun_type)(A0);

    public:
      class caller_type:
        public typed_method_caller<SelfClass>
      {
      public:
        caller_type();

      private:
        void explicit_execute
        ( SelfClass& self, const std::vector<std::string>& args,
          const argument_converter& c ) const;
      }; // class caller_type

    public:
      /**
       * \brief The instance of the caller.
       *
       * A single instance is sufficient since there is no member variables in
       * caller_type (s_caller only does the transition toward the method to
       * call). We can use its adress and avoid dynamic allocations.
       */
      static const caller_type s_caller;

    }; // class method_caller_implement_1

    /**
     * \brief Same as method_caller_implement_0 but for a method with two
     *        parameter.
     * \author Julien Jorge.
     */
    template< typename SelfClass, typename ParentClass, typename R, typename A0,
              typename A1, R (ParentClass::*Member)(A0, A1) >
    class method_caller_implement_2
    {
    public:
      typedef R (ParentClass::*mem_fun_type)(A0, A1);

    public:
      class caller_type:
        public typed_method_caller<SelfClass>
      {
      public:
        caller_type();

      private:
        void explicit_execute
        ( SelfClass& self, const std::vector<std::string>& args,
          const argument_converter& c ) const;
      }; // class caller_type

    public:
      /**
       * \brief The instance of the caller.
       *
       * A single instance is sufficient since there is no member variables in
       * caller_type (s_caller only does the transition toward the method to
       * call). We can use its adress and avoid dynamic allocations.
       */
      static const caller_type s_caller;

    }; // class method_caller_implement_2

    /**
     * \brief Same as method_caller_implement_0 but for a method with three
     *        parameter.
     * \author Julien Jorge.
     */
    template< typename SelfClass, typename ParentClass, typename R, typename A0,
              typename A1, typename A2, R (ParentClass::*Member)(A0, A1, A2) >
    class method_caller_implement_3
    {
    public:
      typedef R (ParentClass::*mem_fun_type)(A0, A1, A2);

    public:
      class caller_type:
        public typed_method_caller<SelfClass>
      {
      public:
        caller_type();

      private:
        void explicit_execute
        ( SelfClass& self, const std::vector<std::string>& args,
          const argument_converter& c ) const;
      }; // class caller_type

    public:
      /**
       * \brief The instance of the caller.
       *
       * A single instance is sufficient since there is no member variables in
       * caller_type (s_caller only does the transition toward the method to
       * call). We can use its adress and avoid dynamic allocations.
       */
      static const caller_type s_caller;

    }; // class method_caller_implement_3

  } // namespace text_interface
} // namespace bear

#include "text_interface/impl/method_caller_implement.tpp"

#endif // __TEXT_INTERFACE_METHOD_CALLER_IMPLEMENT_HPP__
