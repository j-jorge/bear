/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::method_caller_implement
 *        class.
 * \author Julien Jorge.
 */

#include "text_interface/argument_converter.hpp"
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
template
< typename SelfClass, typename ParentClass, typename R,
  R (ParentClass::*Member)() >
const typename
bear::text_interface::method_caller_implement_0
<SelfClass, ParentClass, R, Member>::caller_type
bear::text_interface::method_caller_implement_0
<SelfClass, ParentClass, R, Member>::s_caller;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template
< typename SelfClass, typename ParentClass, typename R,
  R (ParentClass::*Member)() >
bear::text_interface::method_caller_implement_0
<SelfClass, ParentClass, R, Member>::caller_type::caller_type()
{
  // nothing to do
} // method_caller_implement_0::caller_type::caller_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method of a given instance.
 * \param self The instance on which the method is called.
 * \param args The string representation of the value of the arguments passed to
 *        the method.
 * \param c The converter used to convert the arguments.
 */
template
< typename SelfClass, typename ParentClass, typename R,
  R (ParentClass::*Member)() >
void bear::text_interface::method_caller_implement_0
<SelfClass, ParentClass, R, Member>::caller_type::explicit_execute
( SelfClass& self, const std::vector<std::string>& args,
  const argument_converter& c ) const
{
  CLAW_PRECOND( args.size() == 0 );

  const mem_fun_type member(Member);
  (self.*member)();
} // method_caller_implement_0::caller_type::explicit_execute()





/*----------------------------------------------------------------------------*/
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  R (ParentClass::*Member)(A0) >
const typename
bear::text_interface::method_caller_implement_1
<SelfClass, ParentClass, R, A0, Member>::caller_type
bear::text_interface::method_caller_implement_1
<SelfClass, ParentClass, R, A0, Member>::s_caller;

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method of a given instance.
 * \param self The instance on which the method is called.
 * \param args The string representation of the value of the arguments passed to
 *        the method.
 * \param c The converter used to convert the arguments.
 */
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  R (ParentClass::*Member)(A0) >
void bear::text_interface::method_caller_implement_1
<SelfClass, ParentClass, R, A0, Member>::caller_type::explicit_execute
( SelfClass& self, const std::vector<std::string>& args,
  const argument_converter& c ) const
{
  CLAW_PRECOND( args.size() == 1 );

  const mem_fun_type member(Member);
  (self.*member)
    ( c.convert_argument<A0>(args[0]) );
} // method_caller_implement_1::caller_type::explicit_execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  R (ParentClass::*Member)(A0) >
bear::text_interface::method_caller_implement_1
<SelfClass, ParentClass, R, A0, Member>::caller_type::caller_type()
{
  // nothing to do
} // method_caller_implement_1::caller_type::caller_type()





/*----------------------------------------------------------------------------*/
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  typename A1, R (ParentClass::*Member)(A0, A1) >
const typename
bear::text_interface::method_caller_implement_2
<SelfClass, ParentClass, R, A0, A1, Member>::caller_type
bear::text_interface::method_caller_implement_2
<SelfClass, ParentClass, R, A0, A1, Member>::s_caller;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  typename A1, R (ParentClass::*Member)(A0, A1) >
bear::text_interface::method_caller_implement_2
<SelfClass, ParentClass, R, A0, A1, Member>::caller_type::caller_type()
{
  // nothing to do
} // method_caller_implement_2::caller_type::caller_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method of a given instance.
 * \param self The instance on which the method is called.
 * \param args The string representation of the value of the arguments passed to
 *        the method.
 * \param c The converter used to convert the arguments.
 */
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  typename A1, R (ParentClass::*Member)(A0, A1) >
void bear::text_interface::method_caller_implement_2
<SelfClass, ParentClass, R, A0, A1, Member>::caller_type::explicit_execute
( SelfClass& self, const std::vector<std::string>& args,
  const argument_converter& c ) const
{
  CLAW_PRECOND( args.size() == 2 );

  const mem_fun_type member(Member);
  (self.*member)
    ( c.template convert_argument<A0>(args[0]),
      c.template convert_argument<A1>(args[1]) );
} // method_caller_implement_2::caller_type::explicit_execute()





/*----------------------------------------------------------------------------*/
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  typename A1, typename A2, R (ParentClass::*Member)(A0, A1, A2) >
const typename
bear::text_interface::method_caller_implement_3
<SelfClass, ParentClass, R, A0, A1, A2, Member>::caller_type
bear::text_interface::method_caller_implement_3
<SelfClass, ParentClass, R, A0, A1, A2, Member>::s_caller;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  typename A1, typename A2, R (ParentClass::*Member)(A0, A1, A2) >
bear::text_interface::method_caller_implement_3
<SelfClass, ParentClass, R, A0, A1, A2, Member>::caller_type::caller_type()
{
  // nothing to do
} // method_caller_implement_3::caller_type::caller_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method of a given instance.
 * \param self The instance on which the method is called.
 * \param args The string representation of the value of the arguments passed to
 *        the method.
 * \param c The converter used to convert the arguments.
 */
template
< typename SelfClass, typename ParentClass, typename R, typename A0,
  typename A1, typename A2, R (ParentClass::*Member)(A0, A1, A2) >
void bear::text_interface::method_caller_implement_3
<SelfClass, ParentClass, R, A0, A1, A2, Member>::caller_type::explicit_execute
( SelfClass& self, const std::vector<std::string>& args,
  const argument_converter& c ) const
{
  CLAW_PRECOND( args.size() == 3 );

  const mem_fun_type member(Member);
  (self.*member)
    ( c.template convert_argument<A0>(args[0]),
      c.template convert_argument<A1>(args[1]),
      c.template convert_argument<A2>(args[2]) );
} // method_caller_implement_3::caller_type::explicit_execute()
